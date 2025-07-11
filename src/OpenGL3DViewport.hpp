#ifndef OPENGL3DVIEWPORT_HPP
#define OPENGL3DVIEWPORT_HPP

#include <QElapsedTimer>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QQuaternion>
#include <QQuickFramebufferObject>
#include <QQuickItem>
#include <QTimer>
#include <QVector3D>
#include <QWheelEvent>

class OpenGL3DRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions {
   public:
    OpenGL3DRenderer();
    ~OpenGL3DRenderer();

    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;
    void synchronize(QQuickFramebufferObject* item) override;

   private:
    void initializeGL();
    void renderShape();
    bool setupShaders();
    void generateGeometry();

    // Geometry generation methods for each shape
    void generateCubeGeometry();
    void generateSphereGeometry();
    void generateTorusGeometry();
    void generateTetrahedronGeometry();
    void updateBuffers();

    // NEW: Dual model rendering methods
    void renderReferenceModel();
    void renderMovableModel();
    void renderVertexLabels();
    void calculateCornerVertices();

    // NEW: Vertex marker rendering
    void generateSphereMarkerGeometry();
    void renderVertexMarker(const QVector3D& position, const QVector3D& color, float scale = 0.05f);

    // OpenGL resources
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer* m_vertexBuffer;
    QOpenGLBuffer* m_indexBuffer;
    QOpenGLBuffer* m_normalBuffer;

    // NEW: Sphere marker buffers
    QOpenGLBuffer* m_sphereVertexBuffer;
    QOpenGLBuffer* m_sphereIndexBuffer;
    QOpenGLBuffer* m_sphereNormalBuffer;

    // Shape data
    QVector<float> m_vertices;
    QVector<float> m_normals;
    QVector<unsigned int> m_indices;

    // NEW: Sphere marker data
    QVector<float> m_sphereVertices;
    QVector<float> m_sphereNormals;
    QVector<unsigned int> m_sphereIndices;

    // Transform state
    QMatrix4x4 m_modelMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;

    // Current shape and properties
    int m_currentShape;
    QVector3D m_translation;
    QQuaternion m_rotation;
    float m_scale;

    // NEW: Research display settings
    bool m_showReferenceModel;
    bool m_showMovableModel;
    bool m_showVertexLabels;

    // NEW: Vertex positions for labeling
    QVector<QVector3D> m_referenceVertices;
    QVector<QVector3D> m_movableVertices;

    // State
    bool m_initialized;
    QSize m_viewportSize;
};

class OpenGL3DViewport : public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(int currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY transformChanged)
    Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation NOTIFY transformChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY transformChanged)

    // Mouse state and sensitivity properties
    Q_PROPERTY(bool mousePressed READ mousePressed NOTIFY mousePressedChanged)
    Q_PROPERTY(float rotationSensitivity READ rotationSensitivity WRITE setRotationSensitivity
                   NOTIFY sensitivityChanged)
    Q_PROPERTY(float translationSensitivity READ translationSensitivity WRITE
                   setTranslationSensitivity NOTIFY sensitivityChanged)
    Q_PROPERTY(float scaleSensitivity READ scaleSensitivity WRITE setScaleSensitivity NOTIFY
                   sensitivityChanged)

    // NEW: Research-specific properties
    Q_PROPERTY(bool showReferenceModel READ showReferenceModel WRITE setShowReferenceModel NOTIFY
                   displayChanged)
    Q_PROPERTY(
        bool showMovableModel READ showMovableModel WRITE setShowMovableModel NOTIFY displayChanged)
    Q_PROPERTY(
        bool showVertexLabels READ showVertexLabels WRITE setShowVertexLabels NOTIFY displayChanged)
    Q_PROPERTY(float alignmentAccuracy READ alignmentAccuracy NOTIFY alignmentChanged)
    Q_PROPERTY(bool taskActive READ taskActive NOTIFY taskStateChanged)

   public:
    enum Shape { CUBE = 1, SPHERE = 2, TORUS = 3, TETRAHEDRON = 4 };
    Q_ENUM(Shape)

    explicit OpenGL3DViewport(QQuickItem* parent = nullptr);

    Renderer* createRenderer() const override;

    // Property getters
    int currentShape() const {
        return m_currentShape;
    }
    QVector3D translation() const {
        return m_translation;
    }
    QVector3D rotation() const {
        return m_rotation;
    }
    float scale() const {
        return m_scale;
    }

    // Mouse and sensitivity getters
    bool mousePressed() const {
        return m_mousePressed;
    }
    float rotationSensitivity() const {
        return m_rotationSensitivity;
    }
    float translationSensitivity() const {
        return m_translationSensitivity;
    }
    float scaleSensitivity() const {
        return m_scaleSensitivity;
    }

    // NEW: Research-specific getters
    bool showReferenceModel() const {
        return m_showReferenceModel;
    }
    bool showMovableModel() const {
        return m_showMovableModel;
    }
    bool showVertexLabels() const {
        return m_showVertexLabels;
    }
    float alignmentAccuracy() const {
        return m_alignmentAccuracy;
    }
    bool taskActive() const {
        return m_taskActive;
    }

   public slots:
    void setCurrentShape(int shape);
    void setTranslation(const QVector3D& translation);
    void setRotation(const QVector3D& rotation);
    void setScale(float scale);
    void resetTransform();

    // Mouse interaction methods
    void handleMousePress(QMouseEvent* event);
    void handleMouseMove(QMouseEvent* event);
    void handleMouseRelease(QMouseEvent* event);
    void handleWheelEvent(QWheelEvent* event);

    // Sensitivity setters
    void setRotationSensitivity(float sensitivity);
    void setTranslationSensitivity(float sensitivity);
    void setScaleSensitivity(float sensitivity);

    // NEW: Research-specific setters
    void setShowReferenceModel(bool show);
    void setShowMovableModel(bool show);
    void setShowVertexLabels(bool show);
    void calculateAlignmentAccuracy();

    // NEW: Research task methods
    Q_INVOKABLE void startAlignmentTask();
    Q_INVOKABLE void finishAlignmentTask();
    Q_INVOKABLE void nextInteractionMode();

   signals:
    void currentShapeChanged();
    void transformChanged();
    void mousePressedChanged();
    void sensitivityChanged();

    // NEW: Research-specific signals
    void displayChanged();
    void alignmentChanged();
    void taskStateChanged();
    void alignmentCompleted(float accuracy, int timeMs);

   protected:
    // Override QQuickItem event handlers
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;

   private slots:
    void updateAnimation();

   private:
    // Helper methods for mouse interactions
    void applyRotationDelta(const QPoint& delta);
    void applyTranslationDelta(const QPoint& delta);
    void applyScaleDelta(const QPoint& delta);

    // Shape and transform properties
    int m_currentShape;
    QVector3D m_translation;
    QVector3D m_rotation;
    float m_scale;
    QTimer* m_animationTimer;

    // Mouse interaction state
    bool m_mousePressed;
    QPoint m_lastMousePos;
    Qt::MouseButton m_activeButton;
    float m_rotationSensitivity;
    float m_translationSensitivity;
    float m_scaleSensitivity;

    // NEW: Research data
    bool m_showReferenceModel;
    bool m_showMovableModel;
    bool m_showVertexLabels;
    float m_alignmentAccuracy;
    QElapsedTimer m_taskStartTime;
    bool m_taskActive;
};

#endif  // OPENGL3DVIEWPORT_HPP