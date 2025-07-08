#include "OpenGL3DViewport.hpp"
#include <QOpenGLShaderProgram>
#include <QtMath>
#include <QDebug>

// Simple vertex shader
static const char *vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 mvpMatrix;\n"
"void main()\n"
"{\n"
"   gl_Position = mvpMatrix * vec4(aPos, 1.0);\n"
"}\n";

// Simple fragment shader
static const char *fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(color, 1.0);\n"
"}\n";

// OpenGL3DRenderer Implementation
OpenGL3DRenderer::OpenGL3DRenderer()
    : m_program(nullptr)
    , m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_currentShape(4)
    , m_translation(0.0f, 0.0f, -5.0f)
    , m_scale(1.0f)
    , m_initialized(false)
{
    m_rotation = QQuaternion();
}

OpenGL3DRenderer::~OpenGL3DRenderer()
{
    delete m_program;
    delete m_vertexBuffer;
    delete m_indexBuffer;
}

void OpenGL3DRenderer::render()
{
    if (!m_initialized) {
        initializeGL();
        m_initialized = true;
    }

    // Clear the buffer
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    renderShape();
}

QOpenGLFramebufferObject *OpenGL3DRenderer::createFramebufferObject(const QSize &size)
{
    m_viewportSize = size;
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}

void OpenGL3DRenderer::synchronize(QQuickFramebufferObject *item)
{
    OpenGL3DViewport *viewport = qobject_cast<OpenGL3DViewport*>(item);
    if (viewport) {
        int newShape = viewport->currentShape();
        if (m_currentShape != newShape) {
            m_currentShape = newShape;
            generateGeometry();
        }
        
        m_translation = viewport->translation();
        
        // Convert Euler angles to quaternion
        QVector3D rot = viewport->rotation();
        m_rotation = QQuaternion::fromEulerAngles(rot.x(), rot.y(), rot.z());
        
        m_scale = viewport->scale();
    }
}

void OpenGL3DRenderer::initializeGL()
{
    initializeOpenGLFunctions();
    
    qDebug() << "Initializing OpenGL...";
    qDebug() << "OpenGL version:" << (char*)glGetString(GL_VERSION);
    
    if (!setupShaders()) {
        qDebug() << "Failed to setup shaders!";
        return;
    }
    
    generateGeometry();
    
    qDebug() << "OpenGL 3D Renderer initialized successfully";
}

bool OpenGL3DRenderer::setupShaders()
{
    m_program = new QOpenGLShaderProgram();
    
    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qDebug() << "Failed to compile vertex shader:" << m_program->log();
        return false;
    }
    
    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qDebug() << "Failed to compile fragment shader:" << m_program->log();
        return false;
    }
    
    if (!m_program->link()) {
        qDebug() << "Failed to link shader program:" << m_program->log();
        return false;
    }
    
    qDebug() << "Shaders compiled and linked successfully";
    return true;
}

void OpenGL3DRenderer::generateGeometry()
{
    m_vertices.clear();
    m_indices.clear();
    
    // Generate simple geometry based on current shape
    switch (m_currentShape) {
    case 1: // Cube
        m_vertices = {
            -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f
        };
        m_indices = {
            0,1,2, 2,3,0,   4,5,6, 6,7,4,   0,1,5, 5,4,0,
            2,3,7, 7,6,2,   0,3,7, 7,4,0,   1,2,6, 6,5,1
        };
        break;
        
    case 2: // Sphere (simplified octahedron)
        m_vertices = {
             1.0f,  0.0f,  0.0f,
            -1.0f,  0.0f,  0.0f,
             0.0f,  1.0f,  0.0f,
             0.0f, -1.0f,  0.0f,
             0.0f,  0.0f,  1.0f,
             0.0f,  0.0f, -1.0f
        };
        m_indices = {
            0,2,4, 0,4,3, 0,3,5, 0,5,2,
            1,2,5, 1,5,3, 1,3,4, 1,4,2
        };
        break;
        
    case 3: // Torus (simplified as ring)
        for (int i = 0; i < 8; ++i) {
            float angle = 2.0f * M_PI * i / 8.0f;
            float x1 = 1.5f * cos(angle);
            float z1 = 1.5f * sin(angle);
            float x2 = 0.5f * cos(angle);
            float z2 = 0.5f * sin(angle);
            
            m_vertices.append(x1); m_vertices.append(0.0f); m_vertices.append(z1);
            m_vertices.append(x2); m_vertices.append(0.0f); m_vertices.append(z2);
        }
        for (int i = 0; i < 8; ++i) {
            int next = (i + 1) % 8;
            m_indices.append(i*2); m_indices.append(i*2+1); m_indices.append(next*2);
            m_indices.append(i*2+1); m_indices.append(next*2+1); m_indices.append(next*2);
        }
        break;
        
    case 4: // Tetrahedron
    default:
        m_vertices = {
             0.0f,  1.0f,  0.0f,   // top
            -1.0f, -1.0f,  1.0f,   // front left
             1.0f, -1.0f,  1.0f,   // front right
             1.0f, -1.0f, -1.0f,   // back right
            -1.0f, -1.0f, -1.0f    // back left
        };
        m_indices = {
            0,1,2, 0,2,3, 0,3,4, 0,4,1,  // faces to apex
            1,2,3, 1,3,4  // bottom faces
        };
        break;
    }
    
    // Create or update buffers
    if (!m_vertexBuffer) {
        m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_vertexBuffer->create();
    }
    if (!m_indexBuffer) {
        m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_indexBuffer->create();
    }
    
    m_vertexBuffer->bind();
    m_vertexBuffer->allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));
    m_vertexBuffer->release();
    
    m_indexBuffer->bind();
    m_indexBuffer->allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));
    m_indexBuffer->release();
}

void OpenGL3DRenderer::renderShape()
{
    if (!m_program || !m_vertexBuffer || !m_indexBuffer || m_vertices.isEmpty()) {
        return;
    }
    
    m_program->bind();
    
    // Setup matrices
    m_projectionMatrix.setToIdentity();
    float aspect = float(m_viewportSize.width()) / float(m_viewportSize.height());
    m_projectionMatrix.perspective(45.0f, aspect, 0.1f, 100.0f);
    
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translation);
    m_modelMatrix.rotate(m_rotation);
    m_modelMatrix.scale(m_scale);
    
    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    
    // Set color based on shape
    QVector3D color;
    switch (m_currentShape) {
    case 1: color = QVector3D(1.0f, 0.3f, 0.3f); break; // Red cube
    case 2: color = QVector3D(0.3f, 0.3f, 1.0f); break; // Blue sphere
    case 3: color = QVector3D(0.3f, 1.0f, 0.3f); break; // Green torus
    case 4: color = QVector3D(0.6f, 0.2f, 0.8f); break; // Purple tetrahedron
    default: color = QVector3D(0.5f, 0.5f, 0.5f); break;
    }
    m_program->setUniformValue("color", color);
    
    // Bind buffers and render
    m_vertexBuffer->bind();
    m_indexBuffer->bind();
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    
    glDisableVertexAttribArray(0);
    
    m_indexBuffer->release();
    m_vertexBuffer->release();
    m_program->release();
}

// OpenGL3DViewport Implementation
OpenGL3DViewport::OpenGL3DViewport(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
    , m_currentShape(4) // Default to tetrahedron
    , m_translation(0.0f, 0.0f, -5.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f)
{
    setMirrorVertically(true);
    
    // Setup animation timer
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &OpenGL3DViewport::updateAnimation);
    m_animationTimer->start(33); // 30 FPS
}

QQuickFramebufferObject::Renderer *OpenGL3DViewport::createRenderer() const
{
    return new OpenGL3DRenderer();
}

void OpenGL3DViewport::setCurrentShape(int shape)
{
    if (m_currentShape != shape) {
        m_currentShape = shape;
        emit currentShapeChanged();
        update();
        qDebug() << "Shape changed to:" << shape;
    }
}

void OpenGL3DViewport::setTranslation(const QVector3D &translation)
{
    if (m_translation != translation) {
        m_translation = translation;
        emit transformChanged();
        update();
    }
}

void OpenGL3DViewport::setRotation(const QVector3D &rotation)
{
    if (m_rotation != rotation) {
        m_rotation = rotation;
        emit transformChanged();
        update();
    }
}

void OpenGL3DViewport::setScale(float scale)
{
    if (qAbs(m_scale - scale) > 0.001f) {
        m_scale = scale;
        emit transformChanged();
        update();
    }
}

void OpenGL3DViewport::resetTransform()
{
    setTranslation(QVector3D(0.0f, 0.0f, -5.0f));
    setRotation(QVector3D(0.0f, 0.0f, 0.0f));
    setScale(1.0f);
}

void OpenGL3DViewport::updateAnimation()
{
    update(); // Trigger re-render
}

#include "OpenGL3DViewport.moc"
