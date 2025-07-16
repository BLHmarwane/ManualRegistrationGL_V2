#include "SpaceMouseManager.hpp"

#include <QDebug>
#include <QtMath>

// Include HIDAPI
#include <hidapi/hidapi.h>

// 3DConnexion device IDs for popular SpaceMouse models
const SpaceMouseManager::DeviceInfo SpaceMouseManager::s_supportedDevices[] = {
    {0x046d, 0xc626, "SpaceMouse Pro"},
    {0x046d, 0xc627, "SpaceMouse Pro Wireless"},
    {0x046d, 0xc62b, "SpaceMouse Pro Compact"},
    {0x256f, 0xc62e, "SpaceMouse Wireless"},
    {0x256f, 0xc62f, "SpaceMouse Pro Wireless (USB)"},
    {0x046d, 0xc628, "SpaceMouse Enterprise"},
    {0x046d, 0xc629, "SpaceMouse Compact"},
    {0x256f, 0xc650, "SpaceMouse Enterprise"},
    {0x256f, 0xc651, "SpaceMouse Pro Compact"},
    {0x256f, 0xc652, "SpaceMouse Pro"}};

const int SpaceMouseManager::s_deviceCount = sizeof(s_supportedDevices) / sizeof(DeviceInfo);

SpaceMouseManager::SpaceMouseManager(QObject* parent)
    : QObject(parent),
      m_deviceHandle(nullptr),
      m_pollTimer(new QTimer(this)),
      m_enabled(false),
      m_isPolling(false),
      m_currentTranslation(0, 0, 0),
      m_currentRotation(0, 0, 0),
      m_translationSensitivity(1.0f),
      m_rotationSensitivity(1.0f),
      m_deadZoneThreshold(50),
      m_useNonLinearScaling(true),
      m_maxInputValue(32767.0f),
      m_leftButtonPressed(false),
      m_rightButtonPressed(false) {
    // Initialize HID API
    if (hid_init() != 0) {
        qWarning() << "Failed to initialize HID API for SpaceMouse";
        return;
    }

    // Setup polling timer (60 FPS)
    m_pollTimer->setInterval(16);
    connect(m_pollTimer, &QTimer::timeout, this, &SpaceMouseManager::pollDevice);

    qDebug() << "SpaceMouseManager initialized - scanning for devices...";

    // Try to connect to a SpaceMouse device
    if (initializeDevice()) {
        qDebug() << "SpaceMouse connected successfully:" << m_deviceName;
    } else {
        qDebug() << "No SpaceMouse device found - will continue scanning";
    }
}

SpaceMouseManager::~SpaceMouseManager() {
    disconnectDevice();
    hid_exit();
}

bool SpaceMouseManager::initializeDevice() {
    if (m_deviceHandle) {
        qDebug() << "Device already connected";
        return true;
    }

    return scanForDevices();
}

bool SpaceMouseManager::scanForDevices() {
    // Try each supported device
    for (int i = 0; i < s_deviceCount; ++i) {
        const DeviceInfo& device = s_supportedDevices[i];

        if (connectToDevice(device.vendor_id, device.product_id)) {
            m_deviceName = QString(device.name);
            qDebug() << "Connected to" << m_deviceName;
            emit deviceConnected(m_deviceName);
            emit connectionChanged(true);
            return true;
        }
    }

    qDebug() << "No supported SpaceMouse devices found";
    return false;
}

bool SpaceMouseManager::connectToDevice(unsigned short vendor_id, unsigned short product_id) {
    hid_device* handle = hid_open(vendor_id, product_id, nullptr);

    if (!handle) {
        return false;
    }

    // Set non-blocking mode
    hid_set_nonblocking(handle, 1);

    m_deviceHandle = handle;

    qDebug() << "SpaceMouse device opened successfully";
    qDebug() << "Vendor ID:" << QString::number(vendor_id, 16);
    qDebug() << "Product ID:" << QString::number(product_id, 16);

    return true;
}

void SpaceMouseManager::disconnectDevice() {
    if (m_pollTimer->isActive()) {
        m_pollTimer->stop();
        m_isPolling = false;
    }

    if (m_deviceHandle) {
        hid_close(m_deviceHandle);
        m_deviceHandle = nullptr;

        emit deviceDisconnected();
        emit connectionChanged(false);

        qDebug() << "SpaceMouse device disconnected";
    }
}

void SpaceMouseManager::setEnabled(bool enabled) {
    if (m_enabled == enabled) {
        return;
    }

    m_enabled = enabled;

    if (enabled && isConnected()) {
        // Start polling
        if (!m_isPolling) {
            m_pollTimer->start();
            m_isPolling = true;
            qDebug() << "SpaceMouse input enabled - polling started";
        }
    } else {
        // Stop polling
        if (m_isPolling) {
            m_pollTimer->stop();
            m_isPolling = false;
            resetInput();
            qDebug() << "SpaceMouse input disabled - polling stopped";
        }
    }

    emit enabledChanged(enabled);
}

void SpaceMouseManager::pollDevice() {
    if (!m_deviceHandle || !m_enabled) {
        return;
    }

    unsigned char buffer[64];
    int bytesRead = hid_read(m_deviceHandle, buffer, sizeof(buffer));

    if (bytesRead > 0) {
        processInputData(buffer, bytesRead);
    } else if (bytesRead < 0) {
        // Device error or disconnection
        qWarning() << "SpaceMouse read error - device may be disconnected";
        emit deviceError("Failed to read from SpaceMouse device");
        disconnectDevice();

        // Try to reconnect after 1 second
        QTimer::singleShot(1000, this, &SpaceMouseManager::initializeDevice);
    }
}

void SpaceMouseManager::processInputData(const unsigned char* data, int length) {
    if (length < 7) {
        return;  // Not enough data for a complete packet
    }

    // Parse SpaceMouse input data
    // Standard format: [Report ID][Data...]

    if (data[0] == 0x01 && length >= 13) {
        // Translation data (16-bit signed integers, little-endian)
        int16_t tx = (int16_t)((data[2] << 8) | data[1]);
        int16_t ty = (int16_t)((data[4] << 8) | data[3]);
        int16_t tz = (int16_t)((data[6] << 8) | data[5]);

        // Rotation data (16-bit signed integers, little-endian)
        int16_t rx = (int16_t)((data[8] << 8) | data[7]);
        int16_t ry = (int16_t)((data[10] << 8) | data[9]);
        int16_t rz = (int16_t)((data[12] << 8) | data[11]);

        // Process translation
        QVector3D translation = processTranslationData(tx, ty, tz);
        if (translation != m_currentTranslation) {
            m_currentTranslation = translation;
            emit translationInput(translation);
            emit inputChanged();
        }

        // Process rotation
        QVector3D rotation = processRotationData(rx, ry, rz);
        if (rotation != m_currentRotation) {
            m_currentRotation = rotation;
            emit rotationInput(rotation);
            emit inputChanged();
        }
    }

    // Button data (if available)
    if (data[0] == 0x03 && length >= 2) {
        uint8_t buttons = data[1];

        bool leftPressed = (buttons & 0x01) != 0;
        bool rightPressed = (buttons & 0x02) != 0;

        // Left button state change
        if (leftPressed != m_leftButtonPressed) {
            m_leftButtonPressed = leftPressed;
            if (leftPressed) {
                emit leftButtonPressed();
                qDebug() << "SpaceMouse left button pressed";
            }
        }

        // Right button state change
        if (rightPressed != m_rightButtonPressed) {
            m_rightButtonPressed = rightPressed;
            if (rightPressed) {
                emit rightButtonPressed();
                qDebug() << "SpaceMouse right button pressed";
            }
        }
    }
}

QVector3D SpaceMouseManager::processTranslationData(int16_t x, int16_t y, int16_t z) {
    // Apply dead zone filtering
    if (isInDeadZone(x))
        x = 0;
    if (isInDeadZone(y))
        y = 0;
    if (isInDeadZone(z))
        z = 0;

    // Convert to normalized float values [-1.0, 1.0]
    float fx = static_cast<float>(x) / m_maxInputValue;
    float fy = static_cast<float>(y) / m_maxInputValue;
    float fz = static_cast<float>(z) / m_maxInputValue;

    // Apply non-linear scaling for better control
    if (m_useNonLinearScaling) {
        fx = applyNonLinearScaling(fx);
        fy = applyNonLinearScaling(fy);
        fz = applyNonLinearScaling(fz);
    }

    // Apply sensitivity scaling
    fx *= m_translationSensitivity;
    fy *= m_translationSensitivity;
    fz *= m_translationSensitivity;

    // SpaceMouse coordinate system mapping for natural 3D feel
    return QVector3D(fx, -fy, -fz);
}

QVector3D SpaceMouseManager::processRotationData(int16_t rx, int16_t ry, int16_t rz) {
    // Apply dead zone filtering
    if (isInDeadZone(rx))
        rx = 0;
    if (isInDeadZone(ry))
        ry = 0;
    if (isInDeadZone(rz))
        rz = 0;

    // Convert to normalized float values [-1.0, 1.0]
    float frx = static_cast<float>(rx) / m_maxInputValue;
    float fry = static_cast<float>(ry) / m_maxInputValue;
    float frz = static_cast<float>(rz) / m_maxInputValue;

    // Apply non-linear scaling
    if (m_useNonLinearScaling) {
        frx = applyNonLinearScaling(frx);
        fry = applyNonLinearScaling(fry);
        frz = applyNonLinearScaling(frz);
    }

    // Apply sensitivity scaling
    frx *= m_rotationSensitivity;
    fry *= m_rotationSensitivity;
    frz *= m_rotationSensitivity;

    // Convert to degrees per frame for smooth rotation
    const float rotationScale = 2.0f;  // degrees per frame at full input

    return QVector3D(frx * rotationScale, fry * rotationScale, frz * rotationScale);
}

bool SpaceMouseManager::isInDeadZone(int16_t value) const {
    return qAbs(value) < m_deadZoneThreshold;
}

float SpaceMouseManager::applyNonLinearScaling(float input) const {
    // Apply cubic scaling for fine control near center
    float sign = (input < 0) ? -1.0f : 1.0f;
    float magnitude = qAbs(input);

    // Cubic curve: output = input^3
    float scaledMagnitude = magnitude * magnitude * magnitude;

    return sign * scaledMagnitude;
}

void SpaceMouseManager::resetInput() {
    m_currentTranslation = QVector3D(0, 0, 0);
    m_currentRotation = QVector3D(0, 0, 0);
    emit inputChanged();
}

void SpaceMouseManager::setTranslationSensitivity(float sensitivity) {
    float newSensitivity = qBound(0.1f, sensitivity, 5.0f);
    if (qAbs(m_translationSensitivity - newSensitivity) > 0.01f) {
        m_translationSensitivity = newSensitivity;
        emit sensitivityChanged();
        qDebug() << "SpaceMouse translation sensitivity:" << m_translationSensitivity;
    }
}

void SpaceMouseManager::setRotationSensitivity(float sensitivity) {
    float newSensitivity = qBound(0.1f, sensitivity, 5.0f);
    if (qAbs(m_rotationSensitivity - newSensitivity) > 0.01f) {
        m_rotationSensitivity = newSensitivity;
        emit sensitivityChanged();
        qDebug() << "SpaceMouse rotation sensitivity:" << m_rotationSensitivity;
    }
}

void SpaceMouseManager::calibrateDeadZone() {
    qDebug() << "Calibrating SpaceMouse dead zone...";

    // Set conservative default dead zone
    m_deadZoneThreshold = 80;

    qDebug() << "Dead zone threshold set to:" << m_deadZoneThreshold;
}