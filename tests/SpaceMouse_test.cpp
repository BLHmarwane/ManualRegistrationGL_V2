#include <QCoreApplication>
#include <QDebug>
#include <QSignalSpy>
#include <QTest>
#include <QTimer>

#include "SpaceMouseManager.hpp"

class SpaceMouseTest : public QObject {
    Q_OBJECT

   private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDeviceDetection();
    void testConnectionManagement();
    void testSensitivityAdjustment();
    void testInputProcessing();
    void testSignalEmission();

   private:
    SpaceMouseManager* m_spaceMouseManager;
};

void SpaceMouseTest::initTestCase() {
    qDebug() << "=== SpaceMouse Test Suite ===";
    qDebug() << "Testing SpaceMouse integration for Manual Registration Simulator V2";

    m_spaceMouseManager = new SpaceMouseManager(this);
    QVERIFY(m_spaceMouseManager != nullptr);
}

void SpaceMouseTest::cleanupTestCase() {
    if (m_spaceMouseManager) {
        m_spaceMouseManager->disconnectDevice();
        delete m_spaceMouseManager;
        m_spaceMouseManager = nullptr;
    }

    qDebug() << "=== SpaceMouse Test Suite Complete ===";
}

void SpaceMouseTest::testDeviceDetection() {
    qDebug() << "Testing SpaceMouse device detection...";

    // Test initialization
    bool initialized = m_spaceMouseManager->initializeDevice();

    if (initialized) {
        qDebug() << "✅ SpaceMouse device detected and connected";
        qDebug() << "Device name:" << m_spaceMouseManager->deviceName();
        qDebug() << "Device serial:" << m_spaceMouseManager->deviceSerial();

        QVERIFY(m_spaceMouseManager->isConnected());
        QVERIFY(!m_spaceMouseManager->deviceName().isEmpty());
    } else {
        qDebug() << "⚠️  No SpaceMouse device found (this is expected if no device is connected)";
        qDebug() << "Test will continue with connection management tests";

        QVERIFY(!m_spaceMouseManager->isConnected());
    }
}

void SpaceMouseTest::testConnectionManagement() {
    qDebug() << "Testing connection management...";

    // Test connection state tracking
    QSignalSpy connectionSpy(m_spaceMouseManager, &SpaceMouseManager::connectionChanged);

    bool initialState = m_spaceMouseManager->isConnected();
    qDebug() << "Initial connection state:" << initialState;

    // Test disconnect (if connected)
    if (initialState) {
        m_spaceMouseManager->disconnectDevice();
        QVERIFY(!m_spaceMouseManager->isConnected());

        // Try to reconnect
        bool reconnected = m_spaceMouseManager->initializeDevice();
        if (reconnected) {
            QVERIFY(m_spaceMouseManager->isConnected());
            qDebug() << "✅ Reconnection successful";
        }
    }

    qDebug() << "Connection management tests completed";
}

void SpaceMouseTest::testSensitivityAdjustment() {
    qDebug() << "Testing sensitivity adjustment...";

    // Test translation sensitivity
    float initialTransSensitivity = m_spaceMouseManager->translationSensitivity();
    qDebug() << "Initial translation sensitivity:" << initialTransSensitivity;

    m_spaceMouseManager->setTranslationSensitivity(2.0f);
    QCOMPARE(m_spaceMouseManager->translationSensitivity(), 2.0f);

    // Test boundary conditions
    m_spaceMouseManager->setTranslationSensitivity(-1.0f);  // Should be clamped
    QVERIFY(m_spaceMouseManager->translationSensitivity() >= 0.1f);

    m_spaceMouseManager->setTranslationSensitivity(10.0f);  // Should be clamped
    QVERIFY(m_spaceMouseManager->translationSensitivity() <= 5.0f);

    // Test rotation sensitivity
    float initialRotSensitivity = m_spaceMouseManager->rotationSensitivity();
    qDebug() << "Initial rotation sensitivity:" << initialRotSensitivity;

    m_spaceMouseManager->setRotationSensitivity(1.5f);
    QCOMPARE(m_spaceMouseManager->rotationSensitivity(), 1.5f);

    // Restore initial values
    m_spaceMouseManager->setTranslationSensitivity(initialTransSensitivity);
    m_spaceMouseManager->setRotationSensitivity(initialRotSensitivity);

    qDebug() << "✅ Sensitivity adjustment tests passed";
}

void SpaceMouseTest::testInputProcessing() {
    qDebug() << "Testing input processing...";

    // Test input reset
    m_spaceMouseManager->resetInput();

    QVector3D currentTranslation = m_spaceMouseManager->currentTranslation();
    QVector3D currentRotation = m_spaceMouseManager->currentRotation();

    QCOMPARE(currentTranslation, QVector3D(0, 0, 0));
    QCOMPARE(currentRotation, QVector3D(0, 0, 0));

    qDebug() << "✅ Input reset successful";

    // Test calibration
    m_spaceMouseManager->calibrateDeadZone();
    qDebug() << "✅ Dead zone calibration completed";
}

void SpaceMouseTest::testSignalEmission() {
    qDebug() << "Testing signal emission...";

    // Setup signal spies
    QSignalSpy enabledSpy(m_spaceMouseManager, &SpaceMouseManager::enabledChanged);
    QSignalSpy sensitivitySpy(m_spaceMouseManager, &SpaceMouseManager::sensitivityChanged);
    QSignalSpy inputSpy(m_spaceMouseManager, &SpaceMouseManager::inputChanged);

    // Test enabled state changes
    bool initialEnabled = m_spaceMouseManager->isEnabled();
    m_spaceMouseManager->setEnabled(!initialEnabled);

    QCOMPARE(enabledSpy.count(), 1);
    QCOMPARE(m_spaceMouseManager->isEnabled(), !initialEnabled);

    // Restore initial state
    m_spaceMouseManager->setEnabled(initialEnabled);
    QCOMPARE(enabledSpy.count(), 2);

    // Test sensitivity change signals
    m_spaceMouseManager->setTranslationSensitivity(1.23f);
    QVERIFY(sensitivitySpy.count() >= 1);

    m_spaceMouseManager->setRotationSensitivity(0.87f);
    QVERIFY(sensitivitySpy.count() >= 2);

    qDebug() << "✅ Signal emission tests passed";
}

// Main function for standalone test execution
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    SpaceMouseTest test;
    int result = QTest::qExec(&test, argc, argv);

    if (result == 0) {
        qDebug() << "\n🚀 All SpaceMouse tests passed!";
        qDebug() << "SpaceMouse integration is ready for use.";
    } else {
        qDebug() << "\n❌ Some SpaceMouse tests failed.";
        qDebug() << "Check your SpaceMouse driver installation and device connection.";
    }

    return result;
}

#include "test_spacemouse.moc"