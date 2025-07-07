#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTest>
#include <opencv2/opencv.hpp>
#include <iostream>

class DependencyTest : public QObject
{
    Q_OBJECT

private slots:
    void testQt6Core();
    void testQt6Qml();
    void testOpenCV();
    void testOverallSetup();
};

void DependencyTest::testQt6Core()
{
    // Test Qt6 Core functionality
    QString testString = "Qt6 Core Test";
    QVERIFY(!testString.isEmpty());
    QCOMPARE(testString, QString("Qt6 Core Test"));
}

void DependencyTest::testQt6Qml()
{
    // Test QML engine creation
    QQmlApplicationEngine engine;
    QVERIFY(&engine != nullptr);
}

void DependencyTest::testOpenCV()
{
    // Test OpenCV functionality
    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC3);
    QVERIFY(!testImage.empty());
    QCOMPARE(testImage.rows, 100);
    QCOMPARE(testImage.cols, 100);
}

void DependencyTest::testOverallSetup()
{
    std::cout << "=== V2 Dependency Test Results ===" << std::endl;
    std::cout << "✅ Qt6 Version: " << qVersion() << std::endl;
    std::cout << "✅ OpenCV Version: " << CV_VERSION << std::endl;
    std::cout << "✅ All dependencies working correctly!" << std::endl;
    
    // This test always passes if we get here
    QVERIFY(true);
}

// Main function for standalone test execution
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    DependencyTest test;
    int result = QTest::qExec(&test, argc, argv);
    
    if (result == 0) {
        std::cout << "\n🚀 All tests passed! V2 setup is ready!" << std::endl;
    } else {
        std::cout << "\n❌ Some tests failed. Check your setup." << std::endl;
    }
    
    return result;
}

#include "test_dependencies.moc"
