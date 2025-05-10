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

// Gömb adatok
std::vector<GLfloat> sphereVertices;
std::vector<GLuint> sphereIndices;
int sphereIndexCount = 0;
float lightSphereRadius = 0.5f;  // A fényforrást reprezentáló gömb sugara

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

// Gömb generáló függvény
void generateSphere(std::vector<GLfloat>& vertices, int rings, int sectors, float radius) {
    float const R = 1.0f / static_cast<float>(rings - 1);
    float const S = 1.0f / static_cast<float>(sectors - 1);

    vertices.clear();

    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < sectors; ++s) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            // Vertex pozíció
            vertices.push_back(x * radius);
            vertices.push_back(y * radius);
            vertices.push_back(z * radius);

            // Vertex szín - napszerû sárga szín
            vertices.push_back(1.0f);
            vertices.push_back(0.8f);
            vertices.push_back(0.0f);

            // Normálvektor - egység gömb esetén megegyezik a pozícióval
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Textúra koordináták hozzáadása
            vertices.push_back((float)s * S);  // U koordináta (0-1)
            vertices.push_back((float)r * R);  // V koordináta (0-1)
        }
    }
}

// Indexek generálása a gömb háromszögekre bontásához
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

// Nap textúra betöltése
void loadSunTexture() {
    // Textúra inicializálása
    glGenTextures(1, &texture[SunTexture]);
    glBindTexture(GL_TEXTURE_2D, texture[SunTexture]);

    // Textúra paraméterek beállítása
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Betöltjük a textúrát SOIL2 segítségével
    int width, height;
    unsigned char* image = SOIL_load_image("sun_texture.jpg", &width, &height, 0, SOIL_LOAD_RGB);

    if (image) {
        // Ha sikerült betölteni a képet
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        cout << "Nap textúra sikeresen betöltve." << endl;
    }
    else {
        // Ha nem sikerült betölteni a képet, generálunk egy egyszerû narancs-sárga textúrát
        cout << "Nem sikerült betölteni a nap textúrát! Generálunk egy helyettesítõt." << endl;

        // 64x64 méretû narancs-sárga textúra
        const int texWidth = 64;
        const int texHeight = 64;
        unsigned char* data = new unsigned char[texWidth * texHeight * 3];

        for (int y = 0; y < texHeight; y++) {
            for (int x = 0; x < texWidth; x++) {
                int index = (y * texWidth + x) * 3;

                // Alap narancs-sárga szín
                data[index] = 255;           // R (vörös)
                data[index + 1] = 165;       // G (zöld)
                data[index + 2] = 0;         // B (kék)

                // Némi random variációt adunk a színhez
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

    // Kockák vertexeinek beállítása
    glBindVertexArray(VAO[VAOVerticesData]);
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Gömb adatok generálása
    generateSphere(sphereVertices, 30, 30, lightSphereRadius);
    generateSphereIndices(sphereIndices, 30, 30);
    sphereIndexCount = sphereIndices.size();

    // Gömb vertexeinek beállítása
    glBindVertexArray(VAO[VAOSphereData]);
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOSphereData]);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat), sphereVertices.data(), GL_STATIC_DRAW);

    // EBO (Element Buffer Object) beállítása
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BO[EBOSphereData]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLuint), sphereIndices.data(), GL_STATIC_DRAW);

    // Attribútumok beállítása a gömbhöz(11 float / vertex: 3 position + 3 color + 3 normal + 2 texcoord)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // VAO kikapcsolása
    glBindVertexArray(0);

    // Próbáljuk meg betölteni a nap textúrát
    loadSunTexture();

    // Shader változók elérésének beállítása
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
    glUniform1i(useTextureLoc, 0);  // Alapértelmezetten kikapcsolva
    glUniform1i(textureSamplerLoc, 0);  // 0-s textúra egység
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

    // Kamera mozgatása
    if (keyboard[GLFW_KEY_LEFT]) theta += rotationSpeed * deltaTime;
    if (keyboard[GLFW_KEY_RIGHT]) theta -= rotationSpeed * deltaTime;
    if (keyboard[GLFW_KEY_UP]) z_cam += moveSpeed * deltaTime;
    if (keyboard[GLFW_KEY_DOWN]) z_cam -= moveSpeed * deltaTime;

    // Fénysebesség állítás
    if (keyboard[GLFW_KEY_PAGE_UP]) lightSpeed += 0.5f * deltaTime;
    if (keyboard[GLFW_KEY_PAGE_DOWN] && lightSpeed > 0.1f) lightSpeed -= 0.5f * deltaTime;

    // Kamera pozíció frissítése
    cameraPosition = vec3(r * cos(theta), r * sin(theta), z_cam);
    computeCameraMatrix();

    // Fény pozíció frissítése
    lightAngle += lightSpeed * deltaTime;
    lightPosition = vec3(5 * cos(lightAngle), 5 * sin(lightAngle), 0);
    glUniform3fv(lightPositionLoc, 1, value_ptr(lightPosition));
    glUniform3fv(cameraPositionLoc, 1, value_ptr(cameraPosition));
    glUniform1i(useLightingLoc, lightingEnabled);

    // Kockák rajzolása (textúra nélkül)
    glUniform1i(useTextureLoc, 0); // Textúra kikapcsolása a kockákhoz
    glBindVertexArray(VAO[VAOVerticesData]);
    vec3 cubePositions[] = { vec3(0, 0, 0), vec3(0, 0, 2), vec3(0, 0, -2) };
    for (int i = 0; i < 3; i++) {
        matModel = translate(mat4(1.0f), cubePositions[i]);
        glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
        inverseTransposeMatrix = mat3(transpose(inverse(matModel)));
        glUniformMatrix3fv(inverseTransposeMatrixLoc, 1, GL_FALSE, value_ptr(inverseTransposeMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Fényforrás (Nap) rajzolása gömbként (textúrával)
    glBindVertexArray(VAO[VAOSphereData]);
    matModel = translate(mat4(1.0f), lightPosition);
    glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
    inverseTransposeMatrix = mat3(transpose(inverse(matModel)));
    glUniformMatrix3fv(inverseTransposeMatrixLoc, 1, GL_FALSE, value_ptr(inverseTransposeMatrix));

    // Textúra bekapcsolása a naphoz
    if (texture[SunTexture] != 0 && useTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[SunTexture]);
        glUniform1i(textureSamplerLoc, 0);
        glUniform1i(useTextureLoc, 1); // Textúra bekapcsolása
    }
    else {
        glUniform1i(useTextureLoc, 0); // Textúra kikapcsolása, ha nincs textúra vagy useTexture hamis
    }

    // Mindig engedélyezzük a fényt a nap gömbön, hogy világítson
    glUniform1i(useLightingLoc, 0); // Kikapcsoljuk a fényszámítást a naphoz

    // Gömb kirajzolása indexekkel
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

    // Visszaállítjuk az eredeti fényezési állapotot
    glUniform1i(useLightingLoc, lightingEnabled);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Visszaállítjuk az alap VAO-t a következõ frame-re
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
    cout << "T\t\ttoggle sun texture" << endl;  // Új információ
    cout << "==============================================" << endl;

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanUpScene(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}