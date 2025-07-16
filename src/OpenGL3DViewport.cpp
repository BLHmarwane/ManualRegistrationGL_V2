#include "OpenGL3DViewport.hpp"

#include <QDebug>
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include <QRandomGenerator>
#include <QtMath>

#include "SpaceMouseManager.hpp"

// ===================================================================
// SHADER SOURCES
// ===================================================================

// Enhanced vertex shader with lighting support for dual-model rendering
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

// Enhanced fragment shader with lighting and alpha transparency
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

// ===================================================================
// OPENGL3DRENDERER IMPLEMENTATION
// ===================================================================

OpenGL3DRenderer::OpenGL3DRenderer()
    : m_program(nullptr),
      m_vertexBuffer(nullptr),
      m_indexBuffer(nullptr),
      m_normalBuffer(nullptr),
      m_sphereVertexBuffer(nullptr),
      m_sphereIndexBuffer(nullptr),
      m_sphereNormalBuffer(nullptr),
      m_currentShape(4),                // Default: Tetrahedron
      m_translation(0.0f, 0.0f, 0.0f),  // Origin position
      m_scale(1.0f),                    // Unity scale
      m_initialized(false),             // Not initialized yet
      m_showReferenceModel(true),       // Show reference model
      m_showMovableModel(true),         // Show movable model
      m_showVertexLabels(true)          // Show vertex markers
{
    // Set initial rotation for better 3D viewing angle
    m_rotation = QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f);

    qDebug() << "OpenGL3DRenderer created - Dual model research renderer initialized";
}

OpenGL3DRenderer::~OpenGL3DRenderer() {
    // Clean up OpenGL resources
    delete m_program;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_normalBuffer;
    delete m_sphereVertexBuffer;
    delete m_sphereIndexBuffer;
    delete m_sphereNormalBuffer;

    qDebug() << "OpenGL3DRenderer destroyed - All resources cleaned up";
}

void OpenGL3DRenderer::render() {
    // Initialize OpenGL context if needed
    if (!m_initialized) {
        initializeGL();
        m_initialized = true;
    }

    // Clear framebuffer with dark background for research contrast
    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);  // Dark blue-gray background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing and blending for transparency
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup camera matrices (shared by both models)
    setupCameraMatrices();

    // Render dual models for research alignment task
    if (m_showReferenceModel) {
        renderReferenceModel();  // Semi-transparent reference model
    }

    if (m_showMovableModel) {
        renderMovableModel();  // User-controlled colored model
    }

    // Render vertex markers on top for alignment feedback
    if (m_showVertexLabels) {
        renderVertexLabels();
    }

    // Restore OpenGL state
    glDisable(GL_BLEND);
}

QOpenGLFramebufferObject* OpenGL3DRenderer::createFramebufferObject(const QSize& size) {
    m_viewportSize = size;

    // Create framebuffer with combined depth/stencil
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);  // Enable 4x MSAA for smoother edges

    return new QOpenGLFramebufferObject(size, format);
}

void OpenGL3DRenderer::synchronize(QQuickFramebufferObject* item) {
    // Sync data from QML viewport to OpenGL renderer
    OpenGL3DViewport* viewport = qobject_cast<OpenGL3DViewport*>(item);
    if (!viewport) {
        return;
    }

    // Check if shape changed and regenerate geometry if needed
    int newShape = viewport->currentShape();
    if (m_currentShape != newShape) {
        m_currentShape = newShape;
        generateGeometry();
        qDebug() << "Shape changed to:" << newShape;
    }

    // Sync transform data
    m_translation = viewport->translation();

    // Convert Euler angles to quaternion for smooth rotation
    QVector3D rot = viewport->rotation();
    m_rotation = QQuaternion::fromEulerAngles(rot.x(), rot.y(), rot.z());

    m_scale = viewport->scale();

    // Sync research display settings
    m_showReferenceModel = viewport->showReferenceModel();
    m_showMovableModel = viewport->showMovableModel();
    m_showVertexLabels = viewport->showVertexLabels();
}

void OpenGL3DRenderer::initializeGL() {
    // Initialize OpenGL functions
    initializeOpenGLFunctions();

    qDebug() << "Initializing OpenGL for dual-model research renderer...";
    qDebug() << "OpenGL version:" << (char*)glGetString(GL_VERSION);
    qDebug() << "GLSL version:" << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    // Setup shaders
    if (!setupShaders()) {
        qDebug() << "ERROR: Failed to setup shaders!";
        return;
    }

    // Generate initial geometry
    generateGeometry();

    // Generate sphere markers for vertex labeling
    generateSphereMarkerGeometry();

    qDebug() << "Research OpenGL 3D Renderer initialized successfully";
}

bool OpenGL3DRenderer::setupShaders() {
    // Create shader program
    m_program = new QOpenGLShaderProgram();

    // Compile vertex shader
    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qDebug() << "ERROR: Failed to compile vertex shader:" << m_program->log();
        return false;
    }

    // Compile fragment shader
    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qDebug() << "ERROR: Failed to compile fragment shader:" << m_program->log();
        return false;
    }

    // Link shader program
    if (!m_program->link()) {
        qDebug() << "ERROR: Failed to link shader program:" << m_program->log();
        return false;
    }

    qDebug() << "Shaders compiled and linked successfully";
    return true;
}

void OpenGL3DRenderer::setupCameraMatrices() {
    // Setup projection matrix with perspective view
    m_projectionMatrix.setToIdentity();
    float aspect = float(m_viewportSize.width()) / float(m_viewportSize.height());
    m_projectionMatrix.perspective(45.0f, aspect, 0.1f, 100.0f);

    // Setup view matrix with fixed camera position for research consistency
    m_viewMatrix.setToIdentity();
    QVector3D cameraPos(4.0f, 3.0f, 6.0f);  // Camera position
    QVector3D target(0.0f, 0.0f, 0.0f);     // Look at origin
    QVector3D up(0.0f, 1.0f, 0.0f);         // Up vector
    m_viewMatrix.lookAt(cameraPos, target, up);
}

void OpenGL3DRenderer::renderReferenceModel() {
    // Render the semi-transparent reference model at fixed position
    if (!m_program || !m_vertexBuffer || !m_indexBuffer || m_vertices.isEmpty()) {
        return;
    }

    m_program->bind();

    // REFERENCE MODEL: Fixed transformation with good 3D viewing angle
    QMatrix4x4 referenceMatrix;
    referenceMatrix.setToIdentity();
    referenceMatrix.rotate(QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f));

    // Calculate matrices
    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * referenceMatrix;
    QMatrix3x3 normalMatrix = referenceMatrix.normalMatrix();

    // Set shader uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", referenceMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", QVector3D(4.0f, 3.0f, 6.0f));

    // REFERENCE MODEL COLOR: Semi-transparent light blue-gray
    QVector3D referenceColor(0.7f, 0.7f, 0.8f);
    m_program->setUniformValue("color", referenceColor);
    m_program->setUniformValue("alpha", 0.4f);  // Semi-transparent

    // Bind and render geometry
    bindAndRenderGeometry();

    m_program->release();
}

void OpenGL3DRenderer::renderMovableModel() {
    // Render the user-controlled colored model with visibility offset
    if (!m_program || !m_vertexBuffer || !m_indexBuffer || m_vertices.isEmpty()) {
        return;
    }

    m_program->bind();

    // MOVABLE MODEL: Apply user transformations with visibility offset
    QMatrix4x4 movableMatrix;
    movableMatrix.setToIdentity();

    // Apply small offset to ensure movable model is always visible
    QVector3D visibilityOffset(0.0f, 0.3f, 0.3f);
    movableMatrix.translate(m_translation + visibilityOffset);
    movableMatrix.rotate(m_rotation);
    movableMatrix.scale(m_scale);

    // Calculate matrices
    QMatrix4x4 mvpMatrix = m_projectionMatrix * m_viewMatrix * movableMatrix;
    QMatrix3x3 normalMatrix = movableMatrix.normalMatrix();

    // Set shader uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", movableMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", QVector3D(4.0f, 3.0f, 6.0f));

    // MOVABLE MODEL COLOR: Distinct bright color based on shape type
    QVector3D movableColor = getShapeColor(m_currentShape);
    m_program->setUniformValue("color", movableColor);

    // First pass: Semi-transparent fill
    m_program->setUniformValue("alpha", 0.4f);
    bindAndRenderGeometry();

    // Second pass: Solid wireframe edges for better visibility
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(4.0f);  // Thick edges for research visibility
    m_program->setUniformValue("alpha", 1.0f);
    bindAndRenderGeometry();

    // Restore fill mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_program->release();
}

void OpenGL3DRenderer::bindAndRenderGeometry() {
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

    // Render indexed geometry
    m_indexBuffer->bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup vertex attributes
    glDisableVertexAttribArray(0);
    if (m_normalBuffer && !m_normals.isEmpty()) {
        glDisableVertexAttribArray(1);
        m_normalBuffer->release();
    }

    m_indexBuffer->release();
    m_vertexBuffer->release();
}

QVector3D OpenGL3DRenderer::getShapeColor(int shapeType) const {
    // Return distinct bright colors for each shape type
    switch (shapeType) {
        case 1:
            return QVector3D(1.0f, 0.3f, 0.3f);  // Bright red cube
        case 2:
            return QVector3D(0.3f, 0.6f, 1.0f);  // Bright blue sphere
        case 3:
            return QVector3D(0.3f, 1.0f, 0.3f);  // Bright green torus
        case 4:
            return QVector3D(1.0f, 0.3f, 1.0f);  // Bright magenta tetrahedron
        default:
            return QVector3D(0.8f, 0.8f, 0.8f);  // Gray fallback
    }
}

void OpenGL3DRenderer::renderVertexLabels() {
    // Render sphere markers at vertices for alignment feedback
    if (!m_showVertexLabels) {
        return;
    }

    // Get base vertex positions for current shape
    QVector<QVector3D> baseVertices = getShapeVertices(m_currentShape);

    // Render reference model vertex markers (large bright white spheres)
    if (m_showReferenceModel) {
        QMatrix4x4 referenceMatrix;
        referenceMatrix.rotate(QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f));

        for (int i = 0; i < baseVertices.size(); ++i) {
            QVector3D refPos = (referenceMatrix * QVector4D(baseVertices[i], 1.0f)).toVector3D();
            QVector3D whiteColor(1.0f, 1.0f, 1.0f);         // Bright white for reference
            renderVertexMarker(refPos, whiteColor, 0.15f);  // Large markers (1', 2', 3', 4')
        }
    }

    // Render movable model vertex markers (colored spheres)
    if (m_showMovableModel) {
        QMatrix4x4 movableMatrix;

        // Apply same transformations as movable model
        QVector3D visibilityOffset(0.0f, 0.3f, 0.3f);
        movableMatrix.translate(m_translation + visibilityOffset);
        movableMatrix.rotate(m_rotation);
        movableMatrix.scale(m_scale);

        QVector3D markerColor = getShapeColor(m_currentShape);

        for (int i = 0; i < baseVertices.size(); ++i) {
            QVector3D movPos = (movableMatrix * QVector4D(baseVertices[i], 1.0f)).toVector3D();
            renderVertexMarker(movPos, markerColor, 0.12f);  // Medium markers (1, 2, 3, 4)
        }
    }
}

QVector<QVector3D> OpenGL3DRenderer::getShapeVertices(int shapeType) const {
    // Return vertex positions for different shape types
    switch (shapeType) {
        case 1:  // Cube - 8 corners
            return {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, -1.0f, -1.0f),
                    QVector3D(1.0f, 1.0f, -1.0f),   QVector3D(-1.0f, 1.0f, -1.0f),
                    QVector3D(-1.0f, -1.0f, 1.0f),  QVector3D(1.0f, -1.0f, 1.0f),
                    QVector3D(1.0f, 1.0f, 1.0f),    QVector3D(-1.0f, 1.0f, 1.0f)};

        case 4:  // Tetrahedron - 4 corners
        default:
            return {
                QVector3D(0.0f, 1.2f, 0.0f),    // apex
                QVector3D(-1.0f, -0.4f, 1.0f),  // base front-left
                QVector3D(1.0f, -0.4f, 1.0f),   // base front-right
                QVector3D(0.0f, -0.4f, -1.4f)   // base back
            };
    }
}

void OpenGL3DRenderer::renderVertexMarker(const QVector3D& position, const QVector3D& color,
                                          float scale) {
    // Render a single sphere marker at specified position
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

    // Set shader uniforms
    m_program->setUniformValue("mvpMatrix", mvpMatrix);
    m_program->setUniformValue("modelMatrix", markerMatrix);
    m_program->setUniformValue("normalMatrix", normalMatrix);
    m_program->setUniformValue("lightPos", QVector3D(5.0f, 5.0f, 5.0f));
    m_program->setUniformValue("viewPos", QVector3D(4.0f, 3.0f, 6.0f));
    m_program->setUniformValue("color", color);
    m_program->setUniformValue("alpha", 1.0f);  // Solid markers

    // Bind sphere geometry
    m_sphereVertexBuffer->bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    m_sphereNormalBuffer->bind();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Render sphere marker
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

// ===================================================================
// GEOMETRY GENERATION METHODS
// ===================================================================

void OpenGL3DRenderer::generateGeometry() {
    // Clear existing geometry data
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();

    // Generate geometry based on current shape type
    switch (m_currentShape) {
        case 1:
            generateCubeGeometry();
            break;
        case 2:
            generateSphereGeometry();
            break;
        case 3:
            generateTorusGeometry();
            break;
        case 4:
            generateTetrahedronGeometry();
            break;
        default:
            generateTetrahedronGeometry();
            break;
    }

    // Update OpenGL buffers with new geometry
    updateBuffers();

    qDebug() << "Generated geometry for shape" << m_currentShape
             << "- Vertices:" << m_vertices.size() / 3 << "Triangles:" << m_indices.size() / 3;
}

void OpenGL3DRenderer::generateCubeGeometry() {
    // Generate cube with unique vertices per face for proper lighting
    m_vertices = {// Front face (z = 1.0)
                  -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
                  // Back face (z = -1.0)
                  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
                  // Top face (y = 1.0)
                  -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
                  // Bottom face (y = -1.0)
                  -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
                  // Right face (x = 1.0)
                  1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
                  // Left face (x = -1.0)
                  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f};

    // Face normals for proper lighting
    m_normals = {// Front face normals (z+)
                 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                 // Back face normals (z-)
                 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                 // Top face normals (y+)
                 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                 // Bottom face normals (y-)
                 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
                 // Right face normals (x+)
                 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 // Left face normals (x-)
                 -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f};

    // Triangle indices for each face (2 triangles per face)
    m_indices = {
        0,  1,  2,  2,  3,  0,   // Front face
        4,  5,  6,  6,  7,  4,   // Back face
        8,  9,  10, 10, 11, 8,   // Top face
        12, 13, 14, 14, 15, 12,  // Bottom face
        16, 17, 18, 18, 19, 16,  // Right face
        20, 21, 22, 22, 23, 20   // Left face
    };
}

void OpenGL3DRenderer::generateSphereGeometry() {
    // Generate sphere using latitude/longitude method
    const int stacks = 12;  // Horizontal divisions
    const int slices = 16;  // Vertical divisions
    const float radius = 1.0f;

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

            // For sphere, normal equals normalized position
            m_normals.append(x / radius);
            m_normals.append(y / radius);
            m_normals.append(z / radius);
        }
    }

    // Generate triangle indices
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;

            // Two triangles per quad
            m_indices.append(first);
            m_indices.append(second);
            m_indices.append(first + 1);

            m_indices.append(second);
            m_indices.append(second + 1);
            m_indices.append(first + 1);
        }
    }
}

void OpenGL3DRenderer::generateTorusGeometry() {
    // Generate torus geometry
    const int majorSegments = 16;  // Around major radius
    const int minorSegments = 12;  // Around minor radius
    const float majorRadius = 1.0f;
    const float minorRadius = 0.4f;

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

    // Generate triangle indices
    for (int i = 0; i < majorSegments; ++i) {
        for (int j = 0; j < minorSegments; ++j) {
            int first = i * (minorSegments + 1) + j;
            int second = first + minorSegments + 1;

            // Two triangles per quad
            m_indices.append(first);
            m_indices.append(second);
            m_indices.append(first + 1);

            m_indices.append(second);
            m_indices.append(second + 1);
            m_indices.append(first + 1);
        }
    }
}

void OpenGL3DRenderer::generateTetrahedronGeometry() {
    // Regular tetrahedron vertices
    m_vertices = {
        0.0f,  1.2f,  0.0f,  // apex
        -1.0f, -0.4f, 1.0f,  // base front-left
        1.0f,  -0.4f, 1.0f,  // base front-right
        0.0f,  -0.4f, -1.4f  // base back
    };

    // Calculate face normals for tetrahedron
    m_normals = {0.0f,  1.0f,  0.0f,  // apex normal
                 -0.5f, -0.5f, 0.5f,  // base vertex normals (approximated)
                 0.5f,  -0.5f, 0.5f, 0.0f, -0.5f, -0.7f};

    // Triangle indices (4 triangular faces)
    m_indices = {
        0, 1, 2,  // front face
        0, 2, 3,  // right face
        0, 3, 1,  // left face
        1, 3, 2   // base face
    };
}

void OpenGL3DRenderer::generateSphereMarkerGeometry() {
    // Generate low-resolution sphere for vertex markers
    const int stacks = 8;  // Fewer divisions for performance
    const int slices = 12;
    const float radius = 1.0f;

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

            // Normal equals position for unit sphere
            m_sphereNormals.append(x);
            m_sphereNormals.append(y);
            m_sphereNormals.append(z);
        }
    }

    // Generate triangle indices
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

    // Create sphere marker OpenGL buffers
    createSphereBuffers();
}

void OpenGL3DRenderer::createSphereBuffers() {
    // Create vertex buffer for sphere markers
    if (!m_sphereVertexBuffer) {
        m_sphereVertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_sphereVertexBuffer->create();
    }
    m_sphereVertexBuffer->bind();
    m_sphereVertexBuffer->allocate(m_sphereVertices.constData(),
                                   m_sphereVertices.size() * sizeof(float));
    m_sphereVertexBuffer->release();

    // Create normal buffer for sphere markers
    if (!m_sphereNormalBuffer) {
        m_sphereNormalBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_sphereNormalBuffer->create();
    }
    m_sphereNormalBuffer->bind();
    m_sphereNormalBuffer->allocate(m_sphereNormals.constData(),
                                   m_sphereNormals.size() * sizeof(float));
    m_sphereNormalBuffer->release();

    // Create index buffer for sphere markers
    if (!m_sphereIndexBuffer) {
        m_sphereIndexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_sphereIndexBuffer->create();
    }
    m_sphereIndexBuffer->bind();
    m_sphereIndexBuffer->allocate(m_sphereIndices.constData(),
                                  m_sphereIndices.size() * sizeof(unsigned int));
    m_sphereIndexBuffer->release();
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

// Legacy method for compatibility
void OpenGL3DRenderer::renderShape() {
    // This method is kept for compatibility but dual model rendering
    // is now handled by renderReferenceModel() and renderMovableModel()
    renderMovableModel();
}

// ===================================================================
// OPENGL3DVIEWPORT IMPLEMENTATION
// ===================================================================

OpenGL3DViewport::OpenGL3DViewport(QQuickItem* parent)
    : QQuickFramebufferObject(parent),
      m_currentShape(4),                // Default: Tetrahedron
      m_translation(0.0f, 0.0f, 0.0f),  // Start at origin
      m_rotation(15.0f, 25.0f, 0.0f),   // Initial 3D viewing angle
      m_scale(1.0f),                    // Unity scale
      m_mousePressed(false),            // No mouse input initially
      m_activeButton(Qt::NoButton),     // No active button
      m_rotationSensitivity(0.5f),      // Default rotation sensitivity
      m_translationSensitivity(0.01f),  // Default translation sensitivity
      m_scaleSensitivity(0.1f),         // Default scale sensitivity
      m_showReferenceModel(true),       // Show reference model
      m_showMovableModel(true),         // Show movable model
      m_showVertexLabels(true),         // Show vertex markers
      m_alignmentAccuracy(0.0f),        // Initial alignment accuracy
      m_taskActive(false),              // NEW
      m_interactionMode("Mouse"),       // SpaceMouse integration
      m_spaceMouseEnabled(false),
      m_spaceMouseManager(nullptr),
      m_spaceMouseTranslationSensitivity(2.0f),
      m_spaceMouseRotationSensitivity(10.0f),
      m_spaceMouseTranslationInput(0, 0, 0),
      m_spaceMouseRotationInput(0, 0, 0)  // No task active initially
                                          // Initialize SpaceMouse support

{
    // Configure QML framebuffer object
    setMirrorVertically(true);  // Flip Y-axis for proper QML integration

    // Enable mouse and touch input
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MiddleButton);
    setAcceptHoverEvents(true);
    setFlag(QQuickItem::ItemAcceptsInputMethod, true);
    setFlag(QQuickItem::ItemIsFocusScope, true);
    setFocus(true);

    // Setup animation timer for continuous rendering
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, &OpenGL3DViewport::updateAnimation);
    m_animationTimer->start(33);  // ~30 FPS

    // Connect transform changes to alignment calculation for research
    connect(this, &OpenGL3DViewport::transformChanged, this,
            &OpenGL3DViewport::calculateAlignmentAccuracy);
    initializeSpaceMouse();
    qDebug() << "OpenGL3DViewport created - Ready for dual model research";
}

QQuickFramebufferObject::Renderer* OpenGL3DViewport::createRenderer() const {
    return new OpenGL3DRenderer();
}

// ===================================================================
// PROPERTY SETTERS
// ===================================================================

void OpenGL3DViewport::setCurrentShape(int shape) {
    if (m_currentShape != shape) {
        m_currentShape = shape;
        emit currentShapeChanged();
        update();  // Trigger re-render
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

void OpenGL3DViewport::setRotationSensitivity(float sensitivity) {
    float newSensitivity = qBound(0.1f, sensitivity, 2.0f);
    if (qAbs(m_rotationSensitivity - newSensitivity) > 0.01f) {
        m_rotationSensitivity = newSensitivity;
        emit sensitivityChanged();
        qDebug() << "Rotation sensitivity changed to:" << m_rotationSensitivity;
    }
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
    float newSensitivity = qBound(0.01f, sensitivity, 1.0f);
    if (qAbs(m_scaleSensitivity - newSensitivity) > 0.01f) {
        m_scaleSensitivity = newSensitivity;
        emit sensitivityChanged();
        qDebug() << "Scale sensitivity changed to:" << m_scaleSensitivity;
    }
}

// ===================================================================
// RESEARCH-SPECIFIC METHODS
// ===================================================================

void OpenGL3DViewport::setShowReferenceModel(bool show) {
    if (m_showReferenceModel != show) {
        m_showReferenceModel = show;
        emit displayChanged();
        update();
        qDebug() << "Reference model visibility:" << show;
    }
}

void OpenGL3DViewport::setShowMovableModel(bool show) {
    if (m_showMovableModel != show) {
        m_showMovableModel = show;
        emit displayChanged();
        update();
        qDebug() << "Movable model visibility:" << show;
    }
}

void OpenGL3DViewport::setShowVertexLabels(bool show) {
    if (m_showVertexLabels != show) {
        m_showVertexLabels = show;
        emit displayChanged();
        update();
        qDebug() << "Vertex labels visibility:" << show;
    }
}

void OpenGL3DViewport::calculateAlignmentAccuracy() {
    // Calculate alignment accuracy for research metrics
    if (!m_taskActive) {
        return;
    }

    // Calculate RMS distance between corresponding vertices
    float totalDistance = 0.0f;
    int vertexCount = 0;

    // Get base vertices for current shape
    QVector<QVector3D> baseVertices = getBaseVertices();

    // Transform vertices using current matrices
    QMatrix4x4 referenceMatrix;
    referenceMatrix.rotate(QQuaternion::fromEulerAngles(15.0f, 25.0f, 0.0f));

    QMatrix4x4 movableMatrix;
    movableMatrix.translate(m_translation);
    movableMatrix.rotate(
        QQuaternion::fromEulerAngles(m_rotation.x(), m_rotation.y(), m_rotation.z()));
    movableMatrix.scale(m_scale);

    // Calculate distances between corresponding vertices
    for (const QVector3D& vertex : baseVertices) {
        QVector3D refPos = (referenceMatrix * QVector4D(vertex, 1.0f)).toVector3D();
        QVector3D movPos = (movableMatrix * QVector4D(vertex, 1.0f)).toVector3D();

        float distance = (refPos - movPos).length();
        totalDistance += distance * distance;
        vertexCount++;
    }

    // Calculate RMS accuracy
    float newAccuracy = vertexCount > 0 ? sqrt(totalDistance / vertexCount) : 100.0f;

    // Update accuracy if changed significantly
    if (qAbs(m_alignmentAccuracy - newAccuracy) > 0.001f) {
        m_alignmentAccuracy = newAccuracy;
        emit alignmentChanged();

        // Check if alignment task is completed (research threshold)
        if (m_alignmentAccuracy < 0.1f && m_taskActive) {
            qint64 elapsedTime = m_taskStartTime.elapsed();
            emit alignmentCompleted(m_alignmentAccuracy, static_cast<int>(elapsedTime));
            m_taskActive = false;
            emit taskStateChanged();
            qDebug() << "Task completed! Accuracy:" << m_alignmentAccuracy << "Time:" << elapsedTime
                     << "ms";
        }
    }
}

QVector<QVector3D> OpenGL3DViewport::getBaseVertices() const {
    // Return base vertex positions for current shape
    switch (m_currentShape) {
        case 1:  // Cube
            return {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, -1.0f, -1.0f),
                    QVector3D(1.0f, 1.0f, -1.0f),   QVector3D(-1.0f, 1.0f, -1.0f),
                    QVector3D(-1.0f, -1.0f, 1.0f),  QVector3D(1.0f, -1.0f, 1.0f),
                    QVector3D(1.0f, 1.0f, 1.0f),    QVector3D(-1.0f, 1.0f, 1.0f)};

        case 4:  // Tetrahedron
        default:
            return {
                QVector3D(0.0f, 1.2f, 0.0f),    // apex
                QVector3D(-1.0f, -0.4f, 1.0f),  // base front-left
                QVector3D(1.0f, -0.4f, 1.0f),   // base front-right
                QVector3D(0.0f, -0.4f, -1.4f)   // base back
            };
    }
}

void OpenGL3DViewport::startAlignmentTask() {
    // Start a new research alignment task
    m_taskStartTime.start();
    m_taskActive = true;
    emit taskStateChanged();

    // Randomize initial position for research consistency
    QRandomGenerator* rng = QRandomGenerator::global();

    // Generate random transform values
    float randX = (rng->bounded(400) - 200) / 100.0f;  // -2.0 to 2.0
    float randY = (rng->bounded(400) - 200) / 100.0f;
    float randZ = (rng->bounded(400) - 200) / 100.0f;

    float randRotX = rng->bounded(360);
    float randRotY = rng->bounded(360);
    float randRotZ = rng->bounded(360);

    float randScale = 0.5f + (rng->bounded(200)) / 200.0f;  // 0.5 to 1.5

    // Apply random initial transform
    setTranslation(QVector3D(randX, randY, randZ));
    setRotation(QVector3D(randRotX, randRotY, randRotZ));
    setScale(randScale);

    qDebug() << "Alignment task started - Target accuracy: < 0.1 units";
    qDebug() << "Initial position:" << m_translation;
    qDebug() << "Initial rotation:" << m_rotation;
    qDebug() << "Initial scale:" << m_scale;
}

void OpenGL3DViewport::finishAlignmentTask() {
    // Manually finish current task
    if (m_taskActive) {
        qint64 elapsedTime = m_taskStartTime.elapsed();
        emit alignmentCompleted(m_alignmentAccuracy, static_cast<int>(elapsedTime));
        m_taskActive = false;
        emit taskStateChanged();
        qDebug() << "Task manually finished - Accuracy:" << m_alignmentAccuracy
                 << "Time:" << elapsedTime << "ms";
    }
}

void OpenGL3DViewport::nextInteractionMode() {
    // Cycle through interaction modes for research
    // TODO: Implement cycling between Mouse, SpaceMouse, Multi-touch
    QStringList availableModes = {"Mouse"};

    if (spaceMouseConnected()) {
        availableModes.append("SpaceMouse");
    }

    int currentIndex = availableModes.indexOf(m_interactionMode);
    int nextIndex = (currentIndex + 1) % availableModes.size();

    QString nextMode = availableModes[nextIndex];
    qDebug() << "Cycling interaction mode from" << m_interactionMode << "to" << nextMode;

    setInteractionMode(nextMode);
    qDebug() << "Cycling to next interaction mode (placeholder for future implementation)";
}

void OpenGL3DViewport::resetTransform() {
    // Reset to initial viewing position
    setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    setRotation(QVector3D(15.0f, 25.0f, 0.0f));
    setScale(1.0f);
    qDebug() << "Transform reset to initial position";
}

// ===================================================================
// MOUSE EVENT HANDLING
// ===================================================================

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

void OpenGL3DViewport::handleMousePress(QMouseEvent* event) {
    m_mousePressed = true;
    m_lastMousePos = event->pos();
    m_activeButton = event->button();
    emit mousePressedChanged();
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

    // Apply transformations based on active mouse button
    switch (m_activeButton) {
        case Qt::LeftButton:
            applyRotationDelta(delta);  // Rotate object
            break;

        case Qt::RightButton:
            applyTranslationDelta(delta);  // Translate object in XY
            break;

        case Qt::MiddleButton:
            applyScaleDelta(delta);  // Scale object
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
    event->accept();
}

void OpenGL3DViewport::handleWheelEvent(QWheelEvent* event) {
    QPoint angleDelta = event->angleDelta();

    if (!angleDelta.isNull()) {
        float delta = angleDelta.y() / 120.0f;  // Standard wheel step

        // Check for modifier keys
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
    }

    event->accept();
}

// ===================================================================
// MOUSE TRANSFORMATION HELPERS
// ===================================================================

void OpenGL3DViewport::applyRotationDelta(const QPoint& delta) {
    // Convert mouse movement to rotation
    float deltaX = delta.x() * m_rotationSensitivity;
    float deltaY = delta.y() * m_rotationSensitivity;

    // Apply rotation around Y axis (horizontal mouse) and X axis (vertical mouse)
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

    // Limit translation range for research consistency
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

// ===================================================================
// KEYBOARD EVENT HANDLING
// ===================================================================

void OpenGL3DViewport::keyPressEvent(QKeyEvent* event) {
    // Keyboard shortcuts for precise control
    const float step = 0.1f;       // Translation step
    const float rotStep = 5.0f;    // Rotation step
    const float scaleStep = 0.1f;  // Scale step

    bool handled = true;
    QString action;

    switch (event->key()) {
        // Translation controls (WASD + QE)
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

        // Rotation controls (Shift + IJKL/UO)
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

        // Reset transform
        case Qt::Key_R:
            if (event->modifiers() & Qt::ControlModifier) {
                resetTransform();
                action = "Reset Transform";
            } else {
                handled = false;
            }
            break;

        // Shape selection (1-4)
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

        // Space mouse switch with keyboard shortcut
        case Qt::Key_M:
            if (event->modifiers() & Qt::ControlModifier) {
                if (m_interactionMode == "Mouse") {
                    setInteractionMode("SpaceMouse");
                } else {
                    setInteractionMode("Mouse");
                }
                action = "Toggle Interaction Mode";
            } else {
                handled = false;
            }
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

// ===================================================================
// SPACEMOUSE INTEGRATION METHODS
// ===================================================================

void OpenGL3DViewport::initializeSpaceMouse() {
    qDebug() << "Initializing SpaceMouse integration...";

    // Create SpaceMouse manager
    m_spaceMouseManager = new SpaceMouseManager(this);

    // Connect SpaceMouse signals to viewport handlers
    connect(m_spaceMouseManager, &SpaceMouseManager::translationInput, this,
            &OpenGL3DViewport::handleSpaceMouseTranslation);
    connect(m_spaceMouseManager, &SpaceMouseManager::rotationInput, this,
            &OpenGL3DViewport::handleSpaceMouseRotation);
    connect(m_spaceMouseManager, &SpaceMouseManager::leftButtonPressed, this,
            &OpenGL3DViewport::handleSpaceMouseLeftButton);
    connect(m_spaceMouseManager, &SpaceMouseManager::rightButtonPressed, this,
            &OpenGL3DViewport::handleSpaceMouseRightButton);
    connect(m_spaceMouseManager, &SpaceMouseManager::connectionChanged, this,
            &OpenGL3DViewport::onSpaceMouseConnectionChanged);

    qDebug() << "SpaceMouse integration initialized";
}

void OpenGL3DViewport::setInteractionMode(const QString& mode) {
    qDebug() << "setInteractionMode called with:" << mode;
    qDebug() << "Current mode:" << m_interactionMode;
    qDebug() << "SpaceMouse connected:" << spaceMouseConnected();

    if (m_interactionMode == mode) {
        qDebug() << "Mode already set, returning";
        return;
    }

    QString oldMode = m_interactionMode;
    m_interactionMode = mode;

    qDebug() << "Interaction mode changed from" << oldMode << "to" << mode;

    // Handle mode switching
    if (mode == "SpaceMouse") {
        if (spaceMouseConnected()) {
            setSpaceMouseEnabled(true);
            qDebug() << "SpaceMouse mode activated - 6DOF control enabled";
        } else {
            qWarning() << "SpaceMouse mode requested but no device connected";
            m_interactionMode = "Mouse";
            emit interactionModeChanged();
            return;
        }
    } else if (mode == "Mouse") {
        setSpaceMouseEnabled(false);
        qDebug() << "Mouse mode activated - traditional 2D control";
    }

    emit interactionModeChanged();
}

bool OpenGL3DViewport::spaceMouseConnected() const {
    return m_spaceMouseManager && m_spaceMouseManager->isConnected();
}

void OpenGL3DViewport::setSpaceMouseEnabled(bool enabled) {
    if (m_spaceMouseEnabled == enabled) {
        return;
    }

    m_spaceMouseEnabled = enabled;

    if (m_spaceMouseManager) {
        m_spaceMouseManager->setEnabled(enabled);
    }

    qDebug() << "SpaceMouse" << (enabled ? "enabled" : "disabled");
    emit spaceMouseEnabledChanged();
}

void OpenGL3DViewport::handleSpaceMouseTranslation(const QVector3D& translation) {
    qDebug() << "SpaceMouse translation input:" << translation;
    qDebug() << "SpaceMouse enabled:" << m_spaceMouseEnabled;
    qDebug() << "Interaction mode:" << m_interactionMode;

    if (!m_spaceMouseEnabled || m_interactionMode != "SpaceMouse") {
        qDebug() << "SpaceMouse input ignored - not enabled or wrong mode";
        return;
    }
    // Debug: Show the raw input values
    qDebug() << "Raw SpaceMouse translation input:" << translation;

    m_spaceMouseTranslationInput = translation;
    emit spaceMouseInputChanged();

    QVector3D scaledTranslation = translation * (m_spaceMouseTranslationSensitivity * 100000.0f);
    // Debug: Show scaled values
    qDebug() << "Scaled translation:" << scaledTranslation;

    QVector3D newTranslation = m_translation + scaledTranslation;

    newTranslation.setX(qBound(-10.0f, newTranslation.x(), 10.0f));
    newTranslation.setY(qBound(-10.0f, newTranslation.y(), 10.0f));
    newTranslation.setZ(qBound(-10.0f, newTranslation.z(), 10.0f));
    qDebug() << "Applying SpaceMouse translation:" << scaledTranslation;
    setTranslation(newTranslation);
}

void OpenGL3DViewport::handleSpaceMouseRotation(const QVector3D& rotation) {
    if (!m_spaceMouseEnabled || m_interactionMode != "SpaceMouse") {
        return;
    }
    // Debug: Show the raw input values
    qDebug() << "Raw SpaceMouse rotation input:" << rotation;

    m_spaceMouseRotationInput = rotation;
    emit spaceMouseInputChanged();

    QVector3D scaledRotation = rotation * (m_spaceMouseRotationSensitivity * 100000.0f);
    // Debug: Show scaled values
    qDebug() << "Scaled rotation:" << scaledRotation;

    QVector3D newRotation = m_rotation + scaledRotation;

    // Normalize angles
    while (newRotation.x() > 180.0f)
        newRotation.setX(newRotation.x() - 360.0f);
    while (newRotation.x() < -180.0f)
        newRotation.setX(newRotation.x() + 360.0f);
    while (newRotation.y() > 180.0f)
        newRotation.setY(newRotation.y() - 360.0f);
    while (newRotation.y() < -180.0f)
        newRotation.setY(newRotation.y() + 360.0f);
    while (newRotation.z() > 180.0f)
        newRotation.setZ(newRotation.z() - 360.0f);
    while (newRotation.z() < -180.0f)
        newRotation.setZ(newRotation.z() + 360.0f);

    setRotation(newRotation);
}

void OpenGL3DViewport::handleSpaceMouseLeftButton() {
    if (!m_spaceMouseEnabled) {
        return;
    }

    qDebug() << "SpaceMouse left button - resetting transform";
    resetTransform();
}

void OpenGL3DViewport::handleSpaceMouseRightButton() {
    if (!m_spaceMouseEnabled) {
        return;
    }

    qDebug() << "SpaceMouse right button - cycling interaction mode";
    nextInteractionMode();
}

void OpenGL3DViewport::onSpaceMouseConnectionChanged(bool connected) {
    qDebug() << "SpaceMouse connection changed:" << connected;

    if (!connected && m_interactionMode == "SpaceMouse") {
        qWarning() << "SpaceMouse disconnected - switching to Mouse mode";
        setInteractionMode("Mouse");
    }

    emit spaceMouseConnectionChanged();
}

void OpenGL3DViewport::setSpaceMouseTranslationSensitivity(float sensitivity) {
    float newSensitivity = qBound(0.1f, sensitivity, 10.0f);
    if (qAbs(m_spaceMouseTranslationSensitivity - newSensitivity) > 0.01f) {
        m_spaceMouseTranslationSensitivity = newSensitivity;

        if (m_spaceMouseManager) {
            m_spaceMouseManager->setTranslationSensitivity(newSensitivity);  // *50.0f
        }

        emit spaceMouseSensitivityChanged();
        qDebug() << "SpaceMouse translation sensitivity:" << m_spaceMouseTranslationSensitivity;
    }
}

void OpenGL3DViewport::setSpaceMouseRotationSensitivity(float sensitivity) {
    float newSensitivity = qBound(0.1f, sensitivity, 10.0f);
    if (qAbs(m_spaceMouseRotationSensitivity - newSensitivity) > 0.01f) {
        m_spaceMouseRotationSensitivity = newSensitivity;

        if (m_spaceMouseManager) {
            m_spaceMouseManager->setRotationSensitivity(newSensitivity);
        }

        emit spaceMouseSensitivityChanged();
        qDebug() << "SpaceMouse rotation sensitivity:" << m_spaceMouseRotationSensitivity;
    }
}

// ===================================================================
// ANIMATION AND UPDATE
// ===================================================================

void OpenGL3DViewport::updateAnimation() {
    // Trigger continuous re-rendering for smooth animation
    update();
}