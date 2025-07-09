#include "OpenGL3DViewport.hpp"

#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QtMath>

// Enhanced vertex shader with lighting support
static const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "uniform mat4 mvpMatrix;\n"
    "uniform mat4 modelMatrix;\n"
    "uniform mat3 normalMatrix;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"
    "void main()\n"
    "{\n"
    "   FragPos = vec3(modelMatrix * vec4(aPos, 1.0));\n"
    "   Normal = normalMatrix * aNormal;\n"
    "   gl_Position = mvpMatrix * vec4(aPos, 1.0);\n"
    "}\n";

// Enhanced fragment shader with lighting
static const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color;\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 viewPos;\n"
    "void main()\n"
    "{\n"
    "   // Ambient lighting\n"
    "   float ambientStrength = 0.3;\n"
    "   vec3 ambient = ambientStrength * color;\n"
    "   \n"
    "   // Diffuse lighting\n"
    "   vec3 norm = normalize(Normal);\n"
    "   vec3 lightDir = normalize(lightPos - FragPos);\n"
    "   float diff = max(dot(norm, lightDir), 0.0);\n"
    "   vec3 diffuse = diff * color;\n"
    "   \n"
    "   // Specular lighting\n"
    "   float specularStrength = 0.5;\n"
    "   vec3 viewDir = normalize(viewPos - FragPos);\n"
    "   vec3 reflectDir = reflect(-lightDir, norm);\n"
    "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "   vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);\n"
    "   \n"
    "   vec3 result = ambient + diffuse + specular;\n"
    "   FragColor = vec4(result, 1.0);\n"
    "}\n";

// OpenGL3DRenderer Implementation
OpenGL3DRenderer::OpenGL3DRenderer()
    : m_program(nullptr),
      m_vertexBuffer(nullptr),
      m_indexBuffer(nullptr),
      m_normalBuffer(nullptr),
      m_currentShape(4),
      m_translation(0.0f, 0.0f, 0.0f),
      m_scale(1.0f),
      m_initialized(false) {
    // Set initial rotation for better 3D viewing
    m_rotation = QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f);
}

OpenGL3DRenderer::~OpenGL3DRenderer() {
    delete m_program;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_normalBuffer;
}

void OpenGL3DRenderer::render() {
    if (!m_initialized) {
        initializeGL();
        m_initialized = true;
    }

    // Clear with nice background color
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    renderShape();
}

QOpenGLFramebufferObject* OpenGL3DRenderer::createFramebufferObject(const QSize& size) {
    m_viewportSize = size;
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    return new QOpenGLFramebufferObject(size, format);
}

void OpenGL3DRenderer::synchronize(QQuickFramebufferObject* item) {
    OpenGL3DViewport* viewport = qobject_cast<OpenGL3DViewport*>(item);
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

void OpenGL3DRenderer::initializeGL() {
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

bool OpenGL3DRenderer::setupShaders() {
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

void OpenGL3DRenderer::renderShape() {
    if (!m_program || !m_vertexBuffer || !m_indexBuffer || m_vertices.isEmpty()) {
        return;
    }

    m_program->bind();

    // Setup camera and projection
    m_projectionMatrix.setToIdentity();
    float aspect = float(m_viewportSize.width()) / float(m_viewportSize.height());
    m_projectionMatrix.perspective(45.0f, aspect, 0.1f, 100.0f);

    // Position camera for good 3D viewing
    m_viewMatrix.setToIdentity();
    QVector3D cameraPos(4.0f, 3.0f, 6.0f);
    QVector3D target(0.0f, 0.0f, 0.0f);
    QVector3D up(0.0f, 1.0f, 0.0f);
    m_viewMatrix.lookAt(cameraPos, target, up);

    // Apply model transformations
    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_translation);
    m_modelMatrix.rotate(m_rotation);
    m_modelMatrix.scale(m_scale);

    // Calculate matrices
    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
    QMatrix3x3 normalMatrix = m_modelMatrix.normalMatrix();

    // Set uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", m_modelMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", cameraPos);

    // Set color based on shape
    QVector3D color;
    switch (m_currentShape) {
        case 1:
            color = QVector3D(0.8f, 0.2f, 0.2f);
            break;  // Red cube
        case 2:
            color = QVector3D(0.2f, 0.2f, 0.8f);
            break;  // Blue sphere
        case 3:
            color = QVector3D(0.2f, 0.8f, 0.2f);
            break;  // Green torus
        case 4:
            color = QVector3D(0.8f, 0.2f, 0.8f);
            break;  // Magenta tetrahedron
        default:
            color = QVector3D(0.5f, 0.5f, 0.5f);
            break;
    }
    m_program->setUniformValue("color", color);

    // Bind vertex data
    m_vertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Bind normal data if available
    if (m_normalBuffer && !m_normals.isEmpty()) {
        m_normalBuffer->bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    }

    // Bind index buffer and draw
    m_indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup
    glDisableVertexAttribArray(0);
    if (m_normalBuffer && !m_normals.isEmpty()) {
        glDisableVertexAttribArray(1);
        m_normalBuffer->release();
    }

    m_indexBuffer->release();
    m_vertexBuffer->release();
    m_program->release();
}

void OpenGL3DRenderer::generateGeometry() {
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();

    // Generate geometry based on current shape
    switch (m_currentShape) {
        case 1:  // Cube
            generateCubeGeometry();
            break;

        case 2:  // Sphere
            generateSphereGeometry();
            break;

        case 3:  // Torus
            generateTorusGeometry();
            break;

        case 4:  // Tetrahedron
        default:
            generateTetrahedronGeometry();
            break;
    }

    // Update buffers
    updateBuffers();
}

void OpenGL3DRenderer::generateCubeGeometry() {
    // Better cube with unique vertices per face for proper lighting
    m_vertices = {
        // Front face (z = 1.0)
        -1.0f, -1.0f, 1.0f,  // 0
        1.0f, -1.0f, 1.0f,   // 1
        1.0f, 1.0f, 1.0f,    // 2
        -1.0f, 1.0f, 1.0f,   // 3

        // Back face (z = -1.0)
        -1.0f, -1.0f, -1.0f,  // 4
        -1.0f, 1.0f, -1.0f,   // 5
        1.0f, 1.0f, -1.0f,    // 6
        1.0f, -1.0f, -1.0f,   // 7

        // Top face (y = 1.0)
        -1.0f, 1.0f, -1.0f,  // 8
        -1.0f, 1.0f, 1.0f,   // 9
        1.0f, 1.0f, 1.0f,    // 10
        1.0f, 1.0f, -1.0f,   // 11

        // Bottom face (y = -1.0)
        -1.0f, -1.0f, -1.0f,  // 12
        1.0f, -1.0f, -1.0f,   // 13
        1.0f, -1.0f, 1.0f,    // 14
        -1.0f, -1.0f, 1.0f,   // 15

        // Right face (x = 1.0)
        1.0f, -1.0f, -1.0f,  // 16
        1.0f, 1.0f, -1.0f,   // 17
        1.0f, 1.0f, 1.0f,    // 18
        1.0f, -1.0f, 1.0f,   // 19

        // Left face (x = -1.0)
        -1.0f, -1.0f, -1.0f,  // 20
        -1.0f, -1.0f, 1.0f,   // 21
        -1.0f, 1.0f, 1.0f,    // 22
        -1.0f, 1.0f, -1.0f    // 23
    };

    // Proper face normals (one per vertex)
    m_normals = {// Front face normals
                 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

                 // Back face normals
                 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,

                 // Top face normals
                 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                 // Bottom face normals
                 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,

                 // Right face normals
                 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                 // Left face normals
                 -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f};

    // Triangle indices for each face
    m_indices = {// Front face
                 0, 1, 2, 2, 3, 0,
                 // Back face
                 4, 5, 6, 6, 7, 4,
                 // Top face
                 8, 9, 10, 10, 11, 8,
                 // Bottom face
                 12, 13, 14, 14, 15, 12,
                 // Right face
                 16, 17, 18, 18, 19, 16,
                 // Left face
                 20, 21, 22, 22, 23, 20};
}

void OpenGL3DRenderer::generateSphereGeometry() {
    // Generate a proper sphere using latitude/longitude method
    const int stacks = 12;  // Horizontal divisions
    const int slices = 16;  // Vertical divisions
    const float radius = 1.0f;

    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();

    // Generate vertices and normals
    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * float(i) / float(stacks);  // 0 to PI
        float cosPhi = cos(phi);
        float sinPhi = sin(phi);

        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * float(j) / float(slices);  // 0 to 2*PI
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            // Calculate vertex position
            float x = radius * sinPhi * cosTheta;
            float y = radius * cosPhi;
            float z = radius * sinPhi * sinTheta;

            // Add vertex
            m_vertices.append(x);
            m_vertices.append(y);
            m_vertices.append(z);

            // For a sphere, the normal is the same as the normalized position
            m_normals.append(x / radius);
            m_normals.append(y / radius);
            m_normals.append(z / radius);
        }
    }

    // Generate indices for triangles
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            // First triangle
            m_indices.append(first);
            m_indices.append(second);
            m_indices.append(first + 1);

            // Second triangle
            m_indices.append(second);
            m_indices.append(second + 1);
            m_indices.append(first + 1);
        }
    }
}

void OpenGL3DRenderer::generateTorusGeometry() {
    // Generate proper torus geometry
    const int majorSegments = 16;  // Around the major radius
    const int minorSegments = 12;  // Around the minor radius
    const float majorRadius = 1.0f;
    const float minorRadius = 0.4f;

    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();

    // Generate vertices and normals
    for (int i = 0; i <= majorSegments; ++i) {
        float u = 2.0f * M_PI * float(i) / float(majorSegments);
        float cosU = cos(u);
        float sinU = sin(u);

        for (int j = 0; j <= minorSegments; ++j) {
            float v = 2.0f * M_PI * float(j) / float(minorSegments);
            float cosV = cos(v);
            float sinV = sin(v);

            // Calculate vertex position
            float x = (majorRadius + minorRadius * cosV) * cosU;
            float y = minorRadius * sinV;
            float z = (majorRadius + minorRadius * cosV) * sinU;

            // Add vertex
            m_vertices.append(x);
            m_vertices.append(y);
            m_vertices.append(z);

            // Calculate normal
            float nx = cosV * cosU;
            float ny = sinV;
            float nz = cosV * sinU;

            m_normals.append(nx);
            m_normals.append(ny);
            m_normals.append(nz);
        }
    }

    // Generate indices
    for (int i = 0; i < majorSegments; ++i) {
        for (int j = 0; j < minorSegments; ++j) {
            int first = i * (minorSegments + 1) + j;
            int second = first + minorSegments + 1;

            // First triangle
            m_indices.append(first);
            m_indices.append(second);
            m_indices.append(first + 1);

            // Second triangle
            m_indices.append(second);
            m_indices.append(second + 1);
            m_indices.append(first + 1);
        }
    }
}

void OpenGL3DRenderer::generateTetrahedronGeometry() {
    // Regular tetrahedron
    m_vertices = {
        0.0f,  1.2f,  0.0f,  // apex
        -1.0f, -0.4f, 1.0f,  // base front-left
        1.0f,  -0.4f, 1.0f,  // base front-right
        0.0f,  -0.4f, -1.4f  // base back
    };

    // Calculate face normals for tetrahedron
    m_normals = {0.0f,  1.0f,  0.0f,  // apex normal
                 -0.5f, -0.5f, 0.5f,  // base normals
                 0.5f,  -0.5f, 0.5f, 0.0f, -0.5f, -0.7f};

    m_indices = {
        0, 1, 2,  // front face
        0, 2, 3,  // right face
        0, 3, 1,  // left face
        1, 3, 2   // base face
    };
}

void OpenGL3DRenderer::updateBuffers() {
    // Create or update vertex buffer
    if (!m_vertexBuffer) {
        m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_vertexBuffer->create();
    }
    m_vertexBuffer->bind();
    m_vertexBuffer->allocate(m_vertices.constData(), m_vertices.size() * sizeof(float));
    m_vertexBuffer->release();

    // Create or update normal buffer
    if (!m_normals.isEmpty()) {
        if (!m_normalBuffer) {
            m_normalBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
            m_normalBuffer->create();
        }
        m_normalBuffer->bind();
        m_normalBuffer->allocate(m_normals.constData(), m_normals.size() * sizeof(float));
        m_normalBuffer->release();
    }

    // Create or update index buffer
    if (!m_indexBuffer) {
        m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_indexBuffer->create();
    }
    m_indexBuffer->bind();
    m_indexBuffer->allocate(m_indices.constData(), m_indices.size() * sizeof(unsigned int));
    m_indexBuffer->release();
}

// OpenGL3DViewport Implementation
OpenGL3DViewport::OpenGL3DViewport(QQuickItem* parent)
    : QQuickFramebufferObject(parent),
      m_currentShape(4)  // Default to tetrahedron
      ,
      m_translation(0.0f, 0.0f, 0.0f)  // Start at origin
      ,
      m_rotation(15.0f, 25.0f, 0.0f)  // Initial 3D rotation
      ,
      m_scale(1.0f) {
    setMirrorVertically(true);

    // Setup animation timer
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &OpenGL3DViewport::updateAnimation);
    m_animationTimer->start(33);  // 30 FPS
}

QQuickFramebufferObject::Renderer* OpenGL3DViewport::createRenderer() const {
    return new OpenGL3DRenderer();
}

void OpenGL3DViewport::setCurrentShape(int shape) {
    if (m_currentShape != shape) {
        m_currentShape = shape;
        emit currentShapeChanged();
        update();
        qDebug() << "Shape changed to:" << shape;
    }
}

void OpenGL3DViewport::setTranslation(const QVector3D& translation) {
    if (m_translation != translation) {
        m_translation = translation;
        emit transformChanged();
        update();
    }
}

void OpenGL3DViewport::setRotation(const QVector3D& rotation) {
    if (m_rotation != rotation) {
        m_rotation = rotation;
        emit transformChanged();
        update();
    }
}

void OpenGL3DViewport::setScale(float scale) {
    if (qAbs(m_scale - scale) > 0.001f) {
        m_scale = scale;
        emit transformChanged();
        update();
    }
}

void OpenGL3DViewport::resetTransform() {
    // Reset with good 3D viewing angle
    setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    setRotation(QVector3D(15.0f, 25.0f, 0.0f));
    setScale(1.0f);
}

void OpenGL3DViewport::updateAnimation() {
    update();  // Trigger re-render
}

// REMOVED: #include "OpenGL3DViewport.moc" - This was causing the MOC warning