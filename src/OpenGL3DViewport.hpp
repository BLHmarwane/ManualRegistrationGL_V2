#ifndef OPENGL3DVIEWPORT_HPP
#define OPENGL3DVIEWPORT_HPP

#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>
#include <QTimer>

class OpenGL3DRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    OpenGL3DRenderer();
    ~OpenGL3DRenderer();

    void render() override;
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item) override;

private:
    void initializeGL();
    void renderShape();
    bool setupShaders();
    void generateGeometry();

    // OpenGL resources
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vertexBuffer;
    QOpenGLBuffer *m_indexBuffer;
    
    // Shape data
    QVector<float> m_vertices;
    QVector<unsigned int> m_indices;
    
    // Transform state
    QMatrix4x4 m_modelMatrix;
    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
    
    // Current shape and properties
    int m_currentShape;
    QVector3D m_translation;
    QQuaternion m_rotation;
    float m_scale;
    
    // State
    bool m_initialized;
    QSize m_viewportSize;
};

class OpenGL3DViewport : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(int currentShape READ currentShape WRITE setCurrentShape NOTIFY currentShapeChanged)
    Q_PROPERTY(QVector3D translation READ translation WRITE setTranslation NOTIFY transformChanged)
    Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation NOTIFY transformChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY transformChanged)

public:
    enum Shape {
        CUBE = 1,
        SPHERE = 2,
        TORUS = 3,
        TETRAHEDRON = 4
    };
    Q_ENUM(Shape)

    explicit OpenGL3DViewport(QQuickItem *parent = nullptr);

    Renderer *createRenderer() const override;

    // Property getters
    int currentShape() const { return m_currentShape; }
    QVector3D translation() const { return m_translation; }
    QVector3D rotation() const { return m_rotation; }
    float scale() const { return m_scale; }

public slots:
    void setCurrentShape(int shape);
    void setTranslation(const QVector3D &translation);
    void setRotation(const QVector3D &rotation);
    void setScale(float scale);
    void resetTransform();

signals:
    void currentShapeChanged();
    void transformChanged();

private slots:
    void updateAnimation();

private:
    int m_currentShape;
    QVector3D m_translation;
    QVector3D m_rotation;
    float m_scale;
    QTimer *m_animationTimer;
};

#endif // OPENGL3DVIEWPORT_HPP
