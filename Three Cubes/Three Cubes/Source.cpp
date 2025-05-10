#include "common.h"
#include <vector>

GLchar windowTitle[] = "Three Cubes with Cylindrical Camera";
GLuint cameraPositionLoc;
vec3   cameraPosition = vec3(9.0f, 0.0f, 0.0f),
cameraTarget = vec3(0.0f, 0.0f, 0.0f),
cameraUpVector = vec3(0.0f, 0.0f, 1.0f);
GLuint lightPositionLoc;
vec3   lightPosition;
GLuint inverseTransposeMatrixLoc;
mat3   inverseTransposeMatrix;
GLuint useTextureLoc;
GLuint textureSamplerLoc;
bool useTexture = true;

float r = 9.0f;
float theta = 0.0f;
float z_cam = 0.0f;
bool  lightingEnabled = true;
float lightAngle = 0.0f;
float lightSpeed = 1.0f;
GLuint useLightingLoc;

// G�mb adatok
std::vector<GLfloat> sphereVertices;
std::vector<GLuint> sphereIndices;
int sphereIndexCount = 0;
float lightSphereRadius = 0.5f;  // A f�nyforr�st reprezent�l� g�mb sugara

GLfloat verticesData[] = {
    // positions         // colors           // normals
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

// G�mb gener�l� f�ggv�ny
void generateSphere(std::vector<GLfloat>& vertices, int rings, int sectors, float radius) {
    float const R = 1.0f / static_cast<float>(rings - 1);
    float const S = 1.0f / static_cast<float>(sectors - 1);

    vertices.clear();

    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < sectors; ++s) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            // Vertex poz�ci�
            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            // Vertex sz�n - napszer� s�rga sz�n
            vertices.push_back(1.0f);
            vertices.push_back(0.8f);
            vertices.push_back(0.0f);

            // Norm�lvektor - egys�g g�mb eset�n megegyezik a poz�ci�val
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Text�ra koordin�t�k hozz�ad�sa
            vertices.push_back((float)s * S);  // U koordin�ta (0-1)
            vertices.push_back((float)r * R);  // V koordin�ta (0-1)
        }
    }
}

// Indexek gener�l�sa a g�mb h�romsz�gekre bont�s�hoz
void generateSphereIndices(std::vector<GLuint>& indices, int rings, int sectors) {
    indices.clear();
    for (int r = 0; r < rings - 1; ++r) {
        for (int s = 0; s < sectors - 1; ++s) {
            indices.push_back(r * sectors + s);
            indices.push_back(r * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + (s + 1));

            indices.push_back(r * sectors + s);
            indices.push_back((r + 1) * sectors + (s + 1));
            indices.push_back((r + 1) * sectors + s);
        }
    }
}

// Nap text�ra bet�lt�se
void loadSunTexture() {
    // Text�ra inicializ�l�sa
    glGenTextures(1, &texture[SunTexture]);
    glBindTexture(GL_TEXTURE_2D, texture[SunTexture]);

    // Text�ra param�terek be�ll�t�sa
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bet�ltj�k a text�r�t SOIL2 seg�ts�g�vel
    int width, height;
    unsigned char* image = SOIL_load_image("sun_texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);

    if (image) {
        // Ha siker�lt bet�lteni a k�pet
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        cout << "Nap text�ra sikeresen bet�ltve." << endl;
    }
    else {
        // Ha nem siker�lt bet�lteni a k�pet, gener�lunk egy egyszer� narancs-s�rga text�r�t
        cout << "Nem siker�lt bet�lteni a nap text�r�t! Gener�lunk egy helyettes�t�t." << endl;

        // 64x64 m�ret� narancs-s�rga text�ra
        const int texWidth = 64;
        const int texHeight = 64;
        unsigned char* data = new unsigned char[texWidth * texHeight * 3];

        for (int y = 0; y < texHeight; y++) {
            for (int x = 0; x < texWidth; x++) {
                int index = (y * texWidth + x) * 3;

                // Alap narancs-s�rga sz�n
                data[index] = 255;           // R (v�r�s)
                data[index + 1] = 165;       // G (z�ld)
                data[index + 2] = 0;         // B (k�k)

                // N�mi random vari�ci�t adunk a sz�nhez
                float noise = (rand() % 30) / 255.0f;
                data[index] = (unsigned char)std::min(255.0f, static_cast<float>(data[index]) * (1.0f + noise));
                data[index + 1] = (unsigned char)std::min(255.0f, static_cast<float>(data[index + 1]) * (1.0f + noise));
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        delete[] data;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void initShaderProgram() {
    ShaderInfo shader_info[ProgramCount][3] = { {
        { GL_VERTEX_SHADER, "./vertexShader.glsl" },
        { GL_FRAGMENT_SHADER, "./fragmentShader.glsl" },
        { GL_NONE, nullptr }
    } };

    for (int programItem = 0; programItem < ProgramCount; programItem++) {
        program[programItem] = LoadShaders(shader_info[programItem]);
        locationMatModel = glGetUniformLocation(program[programItem], "matModel");
        locationMatView = glGetUniformLocation(program[programItem], "matView");
        locationMatProjection = glGetUniformLocation(program[programItem], "matProjection");
    }

    // Kock�k vertexeinek be�ll�t�sa
    glBindVertexArray(VAO[VAOVerticesData]);
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // G�mb adatok gener�l�sa
    generateSphere(sphereVertices, 30, 30, lightSphereRadius);
    generateSphereIndices(sphereIndices, 30, 30);
    sphereIndexCount = sphereIndices.size();

    // G�mb vertexeinek be�ll�t�sa
    glBindVertexArray(VAO[VAOSphereData]);
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOSphereData]);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat), sphereVertices.data(), GL_STATIC_DRAW);

    // EBO (Element Buffer Object) be�ll�t�sa
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BO[EBOSphereData]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLuint), sphereIndices.data(), GL_STATIC_DRAW);

    // Attrib�tumok be�ll�t�sa a g�mbh�z(11 float / vertex: 3 position + 3 color + 3 normal + 2 texcoord)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // VAO kikapcsol�sa
    glBindVertexArray(0);

    // Pr�b�ljuk meg bet�lteni a nap text�r�t
    loadSunTexture();

    // Shader v�ltoz�k el�r�s�nek be�ll�t�sa
    glUseProgram(program[QuadScreenProgram]);
    projectionType = Perspective;
    matModel = mat4(1.0);
    matView = lookAt(cameraPosition, cameraTarget, cameraUpVector);
    glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
    glUniformMatrix4fv(locationMatView, 1, GL_FALSE, value_ptr(matView));
    glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));

    inverseTransposeMatrixLoc = glGetUniformLocation(program[QuadScreenProgram], "inverseTransposeMatrix");
    lightPositionLoc = glGetUniformLocation(program[QuadScreenProgram], "lightPosition");
    cameraPositionLoc = glGetUniformLocation(program[QuadScreenProgram], "cameraPosition");
    useLightingLoc = glGetUniformLocation(program[QuadScreenProgram], "useLighting");
    glUniform1i(useLightingLoc, 1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    useTextureLoc = glGetUniformLocation(program[QuadScreenProgram], "useTexture");
    textureSamplerLoc = glGetUniformLocation(program[QuadScreenProgram], "textureSampler");
    glUniform1i(useTextureLoc, 0);  // Alap�rtelmezetten kikapcsolva
    glUniform1i(textureSamplerLoc, 0);  // 0-s text�ra egys�g
}

void computeModelMatrix() {
    matModel = mat4(1.0f);
    glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
}

void computeCameraMatrix() {
    matView = lookAt(cameraPosition, cameraTarget, cameraUpVector);
    glUniformMatrix4fv(locationMatView, 1, GL_FALSE, value_ptr(matView));
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static GLdouble lastFrame = 0.0f;
    GLdouble deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;

    if (deltaTime > 0.1f) deltaTime = 0.1f;

    float rotationSpeed = 1.0f;
    float moveSpeed = 5.0f;

    // Kamera mozgat�sa
    if (keyboard[GLFW_KEY_LEFT]) theta += rotationSpeed * deltaTime;
    if (keyboard[GLFW_KEY_RIGHT]) theta -= rotationSpeed * deltaTime;
    if (keyboard[GLFW_KEY_UP]) z_cam += moveSpeed * deltaTime;
    if (keyboard[GLFW_KEY_DOWN]) z_cam -= moveSpeed * deltaTime;

    // F�nysebess�g �ll�t�s
    if (keyboard[GLFW_KEY_PAGE_UP]) lightSpeed += 0.5f * deltaTime;
    if (keyboard[GLFW_KEY_PAGE_DOWN] && lightSpeed > 0.1f) lightSpeed -= 0.5f * deltaTime;

    // Kamera poz�ci� friss�t�se
    cameraPosition = vec3(r * cos(theta), r * sin(theta), z_cam);
    computeCameraMatrix();

    // F�ny poz�ci� friss�t�se
    lightAngle += lightSpeed * deltaTime;
    lightPosition = vec3(5 * cos(lightAngle), 5 * sin(lightAngle), 0);
    glUniform3fv(lightPositionLoc, 1, value_ptr(lightPosition));
    glUniform3fv(cameraPositionLoc, 1, value_ptr(cameraPosition));
    glUniform1i(useLightingLoc, lightingEnabled);

    // Kock�k rajzol�sa (text�ra n�lk�l)
    glUniform1i(useTextureLoc, 0); // Text�ra kikapcsol�sa a kock�khoz
    glBindVertexArray(VAO[VAOVerticesData]);
    vec3 cubePositions[] = { vec3(0, 0, 0), vec3(0, 0, 2), vec3(0, 0, -2) };
    for (int i = 0; i < 3; i++) {
        matModel = translate(mat4(1.0f), cubePositions[i]);
        glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
        inverseTransposeMatrix = mat3(transpose(inverse(matModel)));
        glUniformMatrix3fv(inverseTransposeMatrixLoc, 1, GL_FALSE, value_ptr(inverseTransposeMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // F�nyforr�s (Nap) rajzol�sa g�mbk�nt (text�r�val)
    glBindVertexArray(VAO[VAOSphereData]);
    matModel = translate(mat4(1.0f), lightPosition);
    glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
    inverseTransposeMatrix = mat3(transpose(inverse(matModel)));
    glUniformMatrix3fv(inverseTransposeMatrixLoc, 1, GL_FALSE, value_ptr(inverseTransposeMatrix));

    // Text�ra bekapcsol�sa a naphoz
    if (texture[SunTexture] != 0 && useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[SunTexture]);
        glUniform1i(textureSamplerLoc, 0);
        glUniform1i(useTextureLoc, 1); // Text�ra bekapcsol�sa
    }
    else {
        glUniform1i(useTextureLoc, 0); // Text�ra kikapcsol�sa, ha nincs text�ra vagy useTexture hamis
    }

    // Mindig enged�lyezz�k a f�nyt a nap g�mb�n, hogy vil�g�tson
    glUniform1i(useLightingLoc, 0); // Kikapcsoljuk a f�nysz�m�t�st a naphoz

    // G�mb kirajzol�sa indexekkel
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

    // Vissza�ll�tjuk az eredeti f�nyez�si �llapotot
    glUniform1i(useLightingLoc, lightingEnabled);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Vissza�ll�tjuk az alap VAO-t a k�vetkez� frame-re
    glBindVertexArray(0);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = glm::max(width, 1);
    windowHeight = glm::max(height, 1);

    GLfloat aspectRatio = (GLfloat)windowWidth / (GLfloat)windowHeight;
    glViewport(0, 0, windowWidth, windowHeight);

    if (projectionType == Orthographic)
        if (windowWidth < windowHeight)
            matProjection = ortho(-worldSize, worldSize, -worldSize / aspectRatio, worldSize / aspectRatio, -100.0, 100.0);
        else
            matProjection = ortho(-worldSize * aspectRatio, worldSize * aspectRatio, -worldSize, worldSize, -100.0, 100.0);
    else
        matProjection = perspective(radians(55.0f), aspectRatio, 0.1f, 100.0f);

    glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    modifiers = mods;
    if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (action == GLFW_PRESS)
        keyboard[key] = GL_TRUE;
    else if (action == GLFW_RELEASE)
        keyboard[key] = GL_FALSE;
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        lightingEnabled = !lightingEnabled;
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        useTexture = !useTexture;
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {}

int main(void) {
    init(3, 3, GLFW_OPENGL_COMPAT_PROFILE);
    initShaderProgram();
    framebufferSizeCallback(window, windowWidth, windowHeight);
    setlocale(LC_ALL, "hu_HU");

    cout << "=== Three Cubes with Cylindrical Camera ===" << endl;
    cout << "Controls:" << endl;
    cout << "ESC\t\texit" << endl;
    cout << "Left/Right\trotate camera around z-axis" << endl;
    cout << "Up/Down\tmove camera along z-axis" << endl;
    cout << "PgUp/PgDown\tchange light rotation speed" << endl;
    cout << "L\t\ttoggle lighting" << endl;
    cout << "T\t\ttoggle sun texture" << endl;  // �j inform�ci�
    cout << "==============================================" << endl;

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanUpScene(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}