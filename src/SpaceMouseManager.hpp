#ifndef SPACEMOUSEMANAGER_HPP
#define SPACEMOUSEMANAGER_HPP

#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QVector3D>

// Forward declare hidapi types to avoid including system headers in header file
typedef struct hid_device_info hid_device_info;
typedef struct hid_device_ hid_device;

/**
 * @brief Professional SpaceMouse (3DConnexion) device manager for 3D interaction research
 *
 * Provides 6DOF input processing with research-grade precision and data logging
 */
class SpaceMouseManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectionChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(float translationSensitivity READ translationSensitivity WRITE
                   setTranslationSensitivity NOTIFY sensitivityChanged)
    Q_PROPERTY(float rotationSensitivity READ rotationSensitivity WRITE setRotationSensitivity
                   NOTIFY sensitivityChanged)
    Q_PROPERTY(QVector3D currentTranslation READ currentTranslation NOTIFY inputChanged)
    Q_PROPERTY(QVector3D currentRotation READ currentRotation NOTIFY inputChanged)

   public:
    explicit SpaceMouseManager(QObject* parent = nullptr);
    ~SpaceMouseManager();

    // Connection management
    bool initializeDevice();
    void disconnectDevice();
    bool isConnected() const {
        return m_deviceHandle != nullptr;
    }

    // Input control
    bool isEnabled() const {
        return m_enabled;
    }
    void setEnabled(bool enabled);

    // Sensitivity control
    float translationSensitivity() const {
        return m_translationSensitivity;
    }
    void setTranslationSensitivity(float sensitivity);

    float rotationSensitivity() const {
        return m_rotationSensitivity;
    }
    void setRotationSensitivity(float sensitivity);

    // Current input state
    QVector3D currentTranslation() const {
        return m_currentTranslation;
    }
    QVector3D currentRotation() const {
        return m_currentRotation;
    }

    // Device information
    QString deviceName() const {
        return m_deviceName;
    }

   public slots:
    void resetInput();
    void calibrateDeadZone();

   signals:
    void connectionChanged(bool connected);
    void enabledChanged(bool enabled);
    void sensitivityChanged();
    void inputChanged();

    // 6DOF input signals
    void translationInput(QVector3D translation);
    void rotationInput(QVector3D rotation);

    // Button signals
    void leftButtonPressed();
    void rightButtonPressed();

    // Device events
    void deviceConnected(const QString& deviceName);
    void deviceDisconnected();
    void deviceError(const QString& error);

   private slots:
    void pollDevice();

   private:
    // Device management
    bool scanForDevices();
    bool connectToDevice(unsigned short vendor_id, unsigned short product_id);
    void processInputData(const unsigned char* data, int length);

    // Input processing
    QVector3D processTranslationData(int16_t x, int16_t y, int16_t z);
    QVector3D processRotationData(int16_t rx, int16_t ry, int16_t rz);
    bool isInDeadZone(int16_t value) const;
    float applyNonLinearScaling(float input) const;

    // Device handle and communication
    hid_device* m_deviceHandle;
    QTimer* m_pollTimer;
    QString m_deviceName;

    // State management
    bool m_enabled;
    bool m_isPolling;

    // Input processing
    QVector3D m_currentTranslation;
    QVector3D m_currentRotation;
    float m_translationSensitivity;
    float m_rotationSensitivity;

    // Calibration and filtering
    int16_t m_deadZoneThreshold;
    bool m_useNonLinearScaling;
    float m_maxInputValue;

    // Button states
    bool m_leftButtonPressed;
    bool m_rightButtonPressed;

    // Supported device information
    struct DeviceInfo {
        unsigned short vendor_id;
        unsigned short product_id;
        const char* name;
    };

    static const DeviceInfo s_supportedDevices[];
    static const int s_deviceCount;
};

#endif  // SPACEMOUSEMANAGER_HPP