#include "OpenGL3DViewport.hpp"

#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include <QRandomGenerator>
#include <QtMath>

// Enhanced vertex shader with lighting and alpha support
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

// Enhanced fragment shader with lighting and alpha
static const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec3 FragPos;\n"
    "in vec3 Normal;\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color;\n"
    "uniform vec3 lightPos;\n"
    "uniform vec3 viewPos;\n"
    "uniform float alpha;\n"
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
    "   FragColor = vec4(result, alpha);\n"
    "}\n";

// OpenGL3DRenderer Implementation
OpenGL3DRenderer::OpenGL3DRenderer()
    : m_program(nullptr),
      m_vertexBuffer(nullptr),
      m_indexBuffer(nullptr),
      m_normalBuffer(nullptr),
      m_sphereVertexBuffer(nullptr),
      m_sphereIndexBuffer(nullptr),
      m_sphereNormalBuffer(nullptr),
      m_currentShape(4),
      m_translation(0.0f, 0.0f, 0.0f),
      m_scale(1.0f),
      m_initialized(false),
      m_showReferenceModel(true),
      m_showMovableModel(true),
      m_showVertexLabels(true) {
    // Set initial rotation for better 3D viewing
    m_rotation = QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f);
}

OpenGL3DRenderer::~OpenGL3DRenderer() {
    delete m_program;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_normalBuffer;
    delete m_sphereVertexBuffer;
    delete m_sphereIndexBuffer;
    delete m_sphereNormalBuffer;
}

void OpenGL3DRenderer::render() {
    if (!m_initialized) {
        initializeGL();
        m_initialized = true;
    }

    // Clear with research-appropriate background
    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);  // Dark blue-gray for contrast
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup camera matrices (shared by both models)
    m_projectionMatrix.setToIdentity();
    float aspect = float(m_viewportSize.width()) / float(m_viewportSize.height());
    m_projectionMatrix.perspective(45.0f, aspect, 0.1f, 100.0f);

    m_viewMatrix.setToIdentity();
    QVector3D cameraPos(4.0f, 3.0f, 6.0f);
    QVector3D target(0.0f, 0.0f, 0.0f);
    QVector3D up(0.0f, 1.0f, 0.0f);
    m_viewMatrix.lookAt(cameraPos, target, up);

    // Render both models
    if (m_showReferenceModel) {
        renderReferenceModel();
    }

    if (m_showMovableModel) {
        renderMovableModel();
    }

    // Render vertex labels on top
    if (m_showVertexLabels) {
        renderVertexLabels();
    }

    glDisable(GL_BLEND);
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

        // NEW: Sync research display settings
        m_showReferenceModel = viewport->showReferenceModel();
        m_showMovableModel = viewport->showMovableModel();
        m_showVertexLabels = viewport->showVertexLabels();
    }
}

void OpenGL3DRenderer::initializeGL() {
    initializeOpenGLFunctions();

    qDebug() << "Initializing OpenGL for dual-model research...";
    qDebug() << "OpenGL version:" << (char*)glGetString(GL_VERSION);

    if (!setupShaders()) {
        qDebug() << "Failed to setup shaders!";
        return;
    }

    generateGeometry();
    generateSphereMarkerGeometry();  // NEW: Generate sphere markers

    qDebug() << "Research OpenGL 3D Renderer initialized successfully";
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

void OpenGL3DRenderer::renderReferenceModel() {
    if (!m_program || !m_vertexBuffer || !m_indexBuffer || m_vertices.isEmpty()) {
        return;
    }

    m_program->bind();

    // REFERENCE MODEL: Fixed at origin with good viewing angle
    QMatrix4x4 referenceMatrix;
    referenceMatrix.setToIdentity();
    // Apply a fixed rotation for good 3D visualization
    referenceMatrix.rotate(QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f));

    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * referenceMatrix;
    QMatrix3x3 normalMatrix = referenceMatrix.normalMatrix();

    // Set uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", referenceMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", QVector3D(4.0f, 3.0f, 6.0f));

    // REFERENCE MODEL COLOR: Semi-transparent gray with good visibility
    QVector3D referenceColor(0.7f, 0.7f, 0.8f);  // Light blue-gray
    m_program->setUniformValue("color", referenceColor);
    m_program->setUniformValue("alpha", 0.4f);

    // Bind and render with transparency
    m_vertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    if (m_normalBuffer && !m_normals.isEmpty()) {
        m_normalBuffer->bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    }

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

void OpenGL3DRenderer::renderMovableModel() {
    if (!m_program || !m_vertexBuffer || !m_indexBuffer || m_vertices.isEmpty()) {
        return;
    }

    m_program->bind();

    // MOVABLE MODEL: Apply user transformations
    QMatrix4x4 movableMatrix;
    movableMatrix.setToIdentity();
    movableMatrix.translate(m_translation);
    movableMatrix.rotate(m_rotation);
    movableMatrix.scale(m_scale);

    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * movableMatrix;
    QMatrix3x3 normalMatrix = movableMatrix.normalMatrix();

    // Set uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", movableMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", QVector3D(4.0f, 3.0f, 6.0f));

    // MOVABLE MODEL COLOR: Distinct bright color based on shape
    QVector3D movableColor;
    switch (m_currentShape) {
        case 1:
            movableColor = QVector3D(1.0f, 0.3f, 0.3f);
            break;  // Bright red cube
        case 2:
            movableColor = QVector3D(0.3f, 0.6f, 1.0f);
            break;  // Bright blue sphere
        case 3:
            movableColor = QVector3D(0.3f, 1.0f, 0.3f);
            break;  // Bright green torus
        case 4:
            movableColor = QVector3D(1.0f, 0.3f, 1.0f);
            break;  // Bright magenta tetrahedron
        default:
            movableColor = QVector3D(0.8f, 0.8f, 0.8f);
            break;
    }
    m_program->setUniformValue("color", movableColor);

    // Bind geometry
    m_vertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    if (m_normalBuffer && !m_normals.isEmpty()) {
        m_normalBuffer->bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    }

    m_indexBuffer->bind();

    // First pass: Semi-transparent fill
    m_program->setUniformValue("alpha", 0.3f);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Second pass: Solid wireframe edges
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(3.0f);  // Thick edges for visibility
    m_program->setUniformValue("alpha", 1.0f);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Restore fill mode

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

void OpenGL3DRenderer::generateSphereMarkerGeometry() {
    const int stacks = 8;  // Lower resolution for markers
    const int slices = 12;
    const float radius = 1.0f;  // Will be scaled when rendering

    m_sphereVertices.clear();
    m_sphereNormals.clear();
    m_sphereIndices.clear();

    // Generate vertices and normals
    for (int i = 0; i <= stacks; ++i) {
        float phi = M_PI * float(i) / float(stacks);
        float cosPhi = cos(phi);
        float sinPhi = sin(phi);

        for (int j = 0; j <= slices; ++j) {
            float theta = 2.0f * M_PI * float(j) / float(slices);
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            // Calculate vertex position
            float x = radius * sinPhi * cosTheta;
            float y = radius * cosPhi;
            float z = radius * sinPhi * sinTheta;

            m_sphereVertices.append(x);
            m_sphereVertices.append(y);
            m_sphereVertices.append(z);

            // Normal is same as position for unit sphere
            m_sphereNormals.append(x);
            m_sphereNormals.append(y);
            m_sphereNormals.append(z);
        }
    }

    // Generate indices
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            m_sphereIndices.append(first);
            m_sphereIndices.append(second);
            m_sphereIndices.append(first + 1);

            m_sphereIndices.append(second);
            m_sphereIndices.append(second + 1);
            m_sphereIndices.append(first + 1);
        }
    }

    // Create sphere marker buffers
    if (!m_sphereVertexBuffer) {
        m_sphereVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_sphereVertexBuffer->create();
    }
    m_sphereVertexBuffer->bind();
    m_sphereVertexBuffer->allocate(m_sphereVertices.constData(),
                                   m_sphereVertices.size() * sizeof(float));
    m_sphereVertexBuffer->release();

    if (!m_sphereNormalBuffer) {
        m_sphereNormalBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_sphereNormalBuffer->create();
    }
    m_sphereNormalBuffer->bind();
    m_sphereNormalBuffer->allocate(m_sphereNormals.constData(),
                                   m_sphereNormals.size() * sizeof(float));
    m_sphereNormalBuffer->release();

    if (!m_sphereIndexBuffer) {
        m_sphereIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_sphereIndexBuffer->create();
    }
    m_sphereIndexBuffer->bind();
    m_sphereIndexBuffer->allocate(m_sphereIndices.constData(),
                                  m_sphereIndices.size() * sizeof(unsigned int));
    m_sphereIndexBuffer->release();
}

void OpenGL3DRenderer::renderVertexMarker(const QVector3D& position, const QVector3D& color,
                                          float scale) {
    if (!m_program || !m_sphereVertexBuffer || !m_sphereIndexBuffer) {
        return;
    }

    m_program->bind();

    // Create marker transformation matrix
    QMatrix4x4 markerMatrix;
    markerMatrix.translate(position);
    markerMatrix.scale(scale);

    // Calculate matrices
    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * markerMatrix;
    QMatrix3x3 normalMatrix = markerMatrix.normalMatrix();

    // Set uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", markerMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", QVector3D(4.0f, 3.0f, 6.0f));
    m_program->setUniformValue("color", color);
    m_program->setUniformValue("alpha", 1.0f);

    // Bind sphere geometry
    m_sphereVertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    m_sphereNormalBuffer->bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Render marker
    m_sphereIndexBuffer->bind();
    glDrawElements(GL_TRIANGLES, m_sphereIndices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    m_sphereIndexBuffer->release();
    m_sphereNormalBuffer->release();
    m_sphereVertexBuffer->release();
    m_program->release();
}

void OpenGL3DRenderer::renderVertexLabels() {
    if (!m_showVertexLabels)
        return;

    // Get vertices for current shape
    QVector<QVector3D> baseVertices;

    switch (m_currentShape) {
        case 1:  // Cube - 8 corners
            baseVertices = {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, -1.0f, -1.0f),
                            QVector3D(1.0f, 1.0f, -1.0f),   QVector3D(-1.0f, 1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, 1.0f),  QVector3D(1.0f, -1.0f, 1.0f),
                            QVector3D(1.0f, 1.0f, 1.0f),    QVector3D(-1.0f, 1.0f, 1.0f)};
            break;

        case 4:  // Tetrahedron - 4 corners
        default:
            baseVertices = {
                QVector3D(0.0f, 1.2f, 0.0f),    // apex
                QVector3D(-1.0f, -0.4f, 1.0f),  // base front-left
                QVector3D(1.0f, -0.4f, 1.0f),   // base front-right
                QVector3D(0.0f, -0.4f, -1.4f)   // base back
            };
            break;
    }

    // Render reference model vertex markers (white with prime numbers)
    if (m_showReferenceModel) {
        QMatrix4x4 referenceMatrix;
        referenceMatrix.rotate(QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f));

        for (int i = 0; i < baseVertices.size(); ++i) {
            QVector3D refPos = (referenceMatrix * QVector4D(baseVertices[i], 1.0f)).toVector3D();
            QVector3D whiteColor(1.0f, 1.0f, 1.0f);         // White for reference (1', 2', 3', 4')
            renderVertexMarker(refPos, whiteColor, 0.08f);  // Slightly larger
        }
    }

    // Render movable model vertex markers (colored by shape)
    if (m_showMovableModel) {
        QMatrix4x4 movableMatrix;
        movableMatrix.translate(m_translation);
        movableMatrix.rotate(m_rotation);
        movableMatrix.scale(m_scale);

        QVector3D markerColor;
        switch (m_currentShape) {
            case 1:
                markerColor = QVector3D(1.0f, 0.0f, 0.0f);
                break;  // Red for cube
            case 2:
                markerColor = QVector3D(0.0f, 0.0f, 1.0f);
                break;  // Blue for sphere
            case 3:
                markerColor = QVector3D(0.0f, 1.0f, 0.0f);
                break;  // Green for torus
            case 4:
                markerColor = QVector3D(1.0f, 0.0f, 1.0f);
                break;  // Magenta for tetrahedron
            default:
                markerColor = QVector3D(0.8f, 0.8f, 0.8f);
                break;
        }

        for (int i = 0; i < baseVertices.size(); ++i) {
            QVector3D movPos = (movableMatrix * QVector4D(baseVertices[i], 1.0f)).toVector3D();
            renderVertexMarker(movPos, markerColor, 0.06f);  // Smaller for movable (1, 2, 3, 4)
        }
    }
}

void OpenGL3DRenderer::renderShape() {
    // This method is kept for compatibility but dual model rendering
    // is now handled by renderReferenceModel() and renderMovableModel()
    renderMovableModel();
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
      m_currentShape(4),                // Default to tetrahedron
      m_translation(0.0f, 0.0f, 0.0f),  // Start at origin
      m_rotation(15.0f, 25.0f, 0.0f),   // Initial 3D rotation
      m_scale(1.0f),
      m_mousePressed(false),
      m_activeButton(Qt::NoButton),
      m_rotationSensitivity(0.5f),
      m_translationSensitivity(0.01f),
      m_scaleSensitivity(0.1f),
      m_showReferenceModel(true),  // NEW
      m_showMovableModel(true),    // NEW
      m_showVertexLabels(true),    // NEW
      m_alignmentAccuracy(0.0f),   // NEW
      m_taskActive(false)          // NEW
{
    setMirrorVertically(true);

    // Enable mouse tracking and accept mouse events
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton);
    setAcceptHoverEvents(true);
    setFlag(QQuickItem::ItemAcceptsInputMethod, true);
    setFlag(QQuickItem::ItemIsFocusScope, true);
    setFocus(true);

    // Setup animation timer
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &OpenGL3DViewport::updateAnimation);
    m_animationTimer->start(33);  // 30 FPS

    // Connect transform changes to alignment calculation
    connect(this, &OpenGL3DViewport::transformChanged, this,
            &OpenGL3DViewport::calculateAlignmentAccuracy);
}

// Research-specific setter implementations
void OpenGL3DViewport::setShowReferenceModel(bool show) {
    if (m_showReferenceModel != show) {
        m_showReferenceModel = show;
        emit displayChanged();
        update();
    }
}

void OpenGL3DViewport::setShowMovableModel(bool show) {
    if (m_showMovableModel != show) {
        m_showMovableModel = show;
        emit displayChanged();
        update();
    }
}

void OpenGL3DViewport::setShowVertexLabels(bool show) {
    if (m_showVertexLabels != show) {
        m_showVertexLabels = show;
        emit displayChanged();
        update();
    }
}

// Calculate alignment accuracy for research metrics
void OpenGL3DViewport::calculateAlignmentAccuracy() {
    if (!m_taskActive)
        return;

    // Calculate RMS distance between corresponding vertices
    float totalDistance = 0.0f;
    int vertexCount = 0;

    // Get base vertices for current shape
    QVector<QVector3D> baseVertices;
    switch (m_currentShape) {
        case 1:  // Cube
            baseVertices = {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, -1.0f, -1.0f),
                            QVector3D(1.0f, 1.0f, -1.0f),   QVector3D(-1.0f, 1.0f, -1.0f),
                            QVector3D(-1.0f, -1.0f, 1.0f),  QVector3D(1.0f, -1.0f, 1.0f),
                            QVector3D(1.0f, 1.0f, 1.0f),    QVector3D(-1.0f, 1.0f, 1.0f)};
            break;
        case 4:  // Tetrahedron
        default:
            baseVertices = {
                QVector3D(0.0f, 1.2f, 0.0f),    // apex
                QVector3D(-1.0f, -0.4f, 1.0f),  // base front-left
                QVector3D(1.0f, -0.4f, 1.0f),   // base front-right
                QVector3D(0.0f, -0.4f, -1.4f)   // base back
            };
            break;
    }

    // Transform vertices
    QMatrix4x4 referenceMatrix;
    referenceMatrix.rotate(QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f));

    QMatrix4x4 movableMatrix;
    movableMatrix.translate(m_translation);
    movableMatrix.rotate(
        QQuaternion::fromEulerAngles(m_rotation.x(), m_rotation.y(), m_rotation.z()));
    movableMatrix.scale(m_scale);

    for (const QVector3D& vertex : baseVertices) {
        QVector3D refPos = (referenceMatrix * QVector4D(vertex, 1.0f)).toVector3D();
        QVector3D movPos = (movableMatrix * QVector4D(vertex, 1.0f)).toVector3D();

        float distance = (refPos - movPos).length();
        totalDistance += distance * distance;
        vertexCount++;
    }

    float newAccuracy = vertexCount > 0 ? sqrt(totalDistance / vertexCount) : 100.0f;

    if (qAbs(m_alignmentAccuracy - newAccuracy) > 0.001f) {
        m_alignmentAccuracy = newAccuracy;
        emit alignmentChanged();

        // Check if alignment is very good (research threshold)
        if (m_alignmentAccuracy < 0.1f && m_taskActive) {
            // Task completed!
            qint64 elapsedTime = m_taskStartTime.elapsed();
            emit alignmentCompleted(m_alignmentAccuracy, static_cast<int>(elapsedTime));
            m_taskActive = false;
            emit taskStateChanged();
        }
    }
}

// Start a research task
void OpenGL3DViewport::startAlignmentTask() {
    m_taskStartTime.start();
    m_taskActive = true;
    emit taskStateChanged();

    // Randomize initial position for research consistency using modern random
    QRandomGenerator* rng = QRandomGenerator::global();

    float randX = (rng->bounded(400) - 200) / 100.0f;  // -2.0 to 2.0
    float randY = (rng->bounded(400) - 200) / 100.0f;
    float randZ = (rng->bounded(400) - 200) / 100.0f;

    float randRotX = rng->bounded(360);
    float randRotY = rng->bounded(360);
    float randRotZ = rng->bounded(360);

    float randScale = 0.5f + (rng->bounded(200)) / 200.0f;  // 0.5 to 1.5

    setTranslation(QVector3D(randX, randY, randZ));
    setRotation(QVector3D(randRotX, randRotY, randRotZ));
    setScale(randScale);

    qDebug() << "Alignment task started - Target accuracy: < 0.1 units";
    qDebug() << "Initial position:" << m_translation;
    qDebug() << "Initial rotation:" << m_rotation;
    qDebug() << "Initial scale:" << m_scale;
}

// Finish current task
void OpenGL3DViewport::finishAlignmentTask() {
    if (m_taskActive) {
        qint64 elapsedTime = m_taskStartTime.elapsed();
        emit alignmentCompleted(m_alignmentAccuracy, static_cast<int>(elapsedTime));
        m_taskActive = false;
        emit taskStateChanged();
        qDebug() << "Task manually finished - Accuracy:" << m_alignmentAccuracy
                 << "Time:" << elapsedTime << "ms";
    }
}

// Cycle through interaction modes for research
void OpenGL3DViewport::nextInteractionMode() {
    // This will be used to switch between:
    // 1. Traditional Mouse (current implementation)
    // 2. SpaceMouse (6DOF) - to be implemented
    // 3. Multi-touch trackpad - to be implemented

    qDebug() << "Cycling to next interaction mode (placeholder for future implementation)";
}

// Mouse interaction methods implementation
void OpenGL3DViewport::handleMousePress(QMouseEvent* event) {
    m_mousePressed = true;
    m_lastMousePos = event->pos();
    m_activeButton = event->button();
    emit mousePressedChanged();
    qDebug() << "Mouse pressed:" << event->button() << "at" << event->pos();
    event->accept();
}

void OpenGL3DViewport::handleMouseMove(QMouseEvent* event) {
    if (!m_mousePressed) {
        return;
    }

    QPoint currentPos = event->pos();
    QPoint delta = currentPos - m_lastMousePos;

    if (delta.isNull()) {
        return;
    }

    // Apply transformations based on which mouse button is pressed
    switch (m_activeButton) {
        case Qt::LeftButton:
            // Left button: Rotate object
            applyRotationDelta(delta);
            break;

        case Qt::RightButton:
            // Right button: Translate object in XY plane
            applyTranslationDelta(delta);
            break;

        case Qt::MiddleButton:
            // Middle button: Scale object
            applyScaleDelta(delta);
            break;

        default:
            break;
    }

    m_lastMousePos = currentPos;
    event->accept();
}

void OpenGL3DViewport::handleMouseRelease(QMouseEvent* event) {
    m_mousePressed = false;
    m_activeButton = Qt::NoButton;
    emit mousePressedChanged();
    qDebug() << "Mouse released:" << event->button();
    event->accept();
}

void OpenGL3DViewport::handleWheelEvent(QWheelEvent* event) {
    // Mouse wheel: Scale object or translate in Z
    QPoint angleDelta = event->angleDelta();

    if (!angleDelta.isNull()) {
        float delta = angleDelta.y() / 120.0f;  // Standard wheel step

        // Check if Ctrl is pressed for Z translation, otherwise scale
        if (event->modifiers() & Qt::ControlModifier) {
            // Ctrl + wheel: Translate in Z direction
            QVector3D newTranslation = m_translation;
            newTranslation.setZ(newTranslation.z() + delta * 0.1f);
            setTranslation(newTranslation);
        } else {
            // Normal wheel: Scale object
            float scaleFactor = 1.0f + (delta * 0.1f);
            float newScale = qBound(0.1f, m_scale * scaleFactor, 5.0f);
            setScale(newScale);
        }

        qDebug() << "Wheel event:" << delta << "New scale/translation:" << m_scale;
    }

    event->accept();
}

// Helper methods for applying transformations
void OpenGL3DViewport::applyRotationDelta(const QPoint& delta) {
    // Convert mouse movement to rotation
    float deltaX = delta.x() * m_rotationSensitivity;
    float deltaY = delta.y() * m_rotationSensitivity;

    // Apply rotation around Y axis (horizontal mouse movement)
    // and around X axis (vertical mouse movement)
    QVector3D newRotation = m_rotation;
    newRotation.setY(newRotation.y() + deltaX);
    newRotation.setX(newRotation.x() - deltaY);  // Inverted for natural feel

    // Clamp X rotation to avoid gimbal lock
    newRotation.setX(qBound(-89.0f, newRotation.x(), 89.0f));

    setRotation(newRotation);
}

void OpenGL3DViewport::applyTranslationDelta(const QPoint& delta) {
    // Convert mouse movement to translation in screen space
    float deltaX = delta.x() * m_translationSensitivity;
    float deltaY = -delta.y() * m_translationSensitivity;  // Inverted Y for natural feel

    QVector3D newTranslation = m_translation;
    newTranslation.setX(newTranslation.x() + deltaX);
    newTranslation.setY(newTranslation.y() + deltaY);

    // Limit translation range
    newTranslation.setX(qBound(-5.0f, newTranslation.x(), 5.0f));
    newTranslation.setY(qBound(-5.0f, newTranslation.y(), 5.0f));

    setTranslation(newTranslation);
}

void OpenGL3DViewport::applyScaleDelta(const QPoint& delta) {
    // Use vertical mouse movement for scaling
    float deltaY = -delta.y() * m_scaleSensitivity * 0.01f;
    float scaleFactor = 1.0f + deltaY;
    float newScale = qBound(0.1f, m_scale * scaleFactor, 5.0f);

    setScale(newScale);
}

// Override mouse event handlers in QQuickItem
void OpenGL3DViewport::mousePressEvent(QMouseEvent* event) {
    handleMousePress(event);
}

void OpenGL3DViewport::mouseMoveEvent(QMouseEvent* event) {
    handleMouseMove(event);
}

void OpenGL3DViewport::mouseReleaseEvent(QMouseEvent* event) {
    handleMouseRelease(event);
}

void OpenGL3DViewport::wheelEvent(QWheelEvent* event) {
    handleWheelEvent(event);
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

// Enhanced keyboard event handler with better feedback
void OpenGL3DViewport::keyPressEvent(QKeyEvent* event) {
    const float step = 0.1f;       // Translation step
    const float rotStep = 5.0f;    // Rotation step in degrees
    const float scaleStep = 0.1f;  // Scale step

    bool handled = true;
    QString action;  // For logging

    switch (event->key()) {
        // Translation controls
        case Qt::Key_W:
        case Qt::Key_Up:
            setTranslation(m_translation + QVector3D(0, step, 0));
            action = "Move Up";
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            setTranslation(m_translation + QVector3D(0, -step, 0));
            action = "Move Down";
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            setTranslation(m_translation + QVector3D(-step, 0, 0));
            action = "Move Left";
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            setTranslation(m_translation + QVector3D(step, 0, 0));
            action = "Move Right";
            break;
        case Qt::Key_Q:
            setTranslation(m_translation + QVector3D(0, 0, step));
            action = "Move Forward";
            break;
        case Qt::Key_E:
            setTranslation(m_translation + QVector3D(0, 0, -step));
            action = "Move Back";
            break;

        // Rotation controls (with Shift modifier)
        case Qt::Key_I:
            if (event->modifiers() & Qt::ShiftModifier) {
                setRotation(m_rotation + QVector3D(rotStep, 0, 0));
                action = "Rotate X+";
            } else {
                handled = false;
            }
            break;
        case Qt::Key_K:
            if (event->modifiers() & Qt::ShiftModifier) {
                setRotation(m_rotation + QVector3D(-rotStep, 0, 0));
                action = "Rotate X-";
            } else {
                handled = false;
            }
            break;
        case Qt::Key_J:
            if (event->modifiers() & Qt::ShiftModifier) {
                setRotation(m_rotation + QVector3D(0, -rotStep, 0));
                action = "Rotate Y-";
            } else {
                handled = false;
            }
            break;
        case Qt::Key_L:
            if (event->modifiers() & Qt::ShiftModifier) {
                setRotation(m_rotation + QVector3D(0, rotStep, 0));
                action = "Rotate Y+";
            } else {
                handled = false;
            }
            break;
        case Qt::Key_U:
            if (event->modifiers() & Qt::ShiftModifier) {
                setRotation(m_rotation + QVector3D(0, 0, rotStep));
                action = "Rotate Z+";
            } else {
                handled = false;
            }
            break;
        case Qt::Key_O:
            if (event->modifiers() & Qt::ShiftModifier) {
                setRotation(m_rotation + QVector3D(0, 0, -rotStep));
                action = "Rotate Z-";
            } else {
                handled = false;
            }
            break;

        // Scale controls
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            setScale(qBound(0.1f, m_scale + scaleStep, 5.0f));
            action = "Scale Up";
            break;
        case Qt::Key_Minus:
            setScale(qBound(0.1f, m_scale - scaleStep, 5.0f));
            action = "Scale Down";
            break;

        // Reset
        case Qt::Key_R:
            if (event->modifiers() & Qt::ControlModifier) {
                resetTransform();
                action = "Reset Transform";
            } else {
                handled = false;
            }
            break;

        // Shape selection
        case Qt::Key_1:
            setCurrentShape(1);
            action = "Select Cube";
            break;
        case Qt::Key_2:
            setCurrentShape(2);
            action = "Select Sphere";
            break;
        case Qt::Key_3:
            setCurrentShape(3);
            action = "Select Torus";
            break;
        case Qt::Key_4:
            setCurrentShape(4);
            action = "Select Tetrahedron";
            break;

        default:
            handled = false;
            break;
    }

    if (handled) {
        event->accept();
        qDebug() << "Keyboard action:" << action;
    } else {
        QQuickFramebufferObject::keyPressEvent(event);
    }
}

void OpenGL3DViewport::focusInEvent(QFocusEvent* event) {
    qDebug() << "3D Viewport gained focus - keyboard shortcuts active";
    QQuickFramebufferObject::focusInEvent(event);
}

// Add sensitivity adjustment methods
void OpenGL3DViewport::setRotationSensitivity(float sensitivity) {
    m_rotationSensitivity = qBound(0.1f, sensitivity, 2.0f);
}

void OpenGL3DViewport::setTranslationSensitivity(float sensitivity) {
    float newSensitivity = qBound(0.001f, sensitivity, 0.1f);
    if (qAbs(m_translationSensitivity - newSensitivity) > 0.0001f) {
        m_translationSensitivity = newSensitivity;
        emit sensitivityChanged();
        qDebug() << "Translation sensitivity changed to:" << m_translationSensitivity;
    }
}

void OpenGL3DViewport::setScaleSensitivity(float sensitivity) {
    m_scaleSensitivity = qBound(0.01f, sensitivity, 1.0f);
}