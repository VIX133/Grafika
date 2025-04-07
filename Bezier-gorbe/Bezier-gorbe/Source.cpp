#include "common.h"

vector<vec2> calculateBezierCurve(const vector<vec2>& controlPoints, int segments);
GLchar              windowTitle[] = "Bezier-gorbe";
GLfloat             aspectRatio;
/* -1 jelentése, hogy nem vonszolunk semmit. */
GLint               dragged = -1;
/* Vektor a szakasz végpontjainak tárolásához. */
static vector<vec2> verticesData; // Üres vektor induláskor
static vector<vec2> bezierCurve;
/* Vektor a Bézier-görbe pontjainak tárolásához. */

void initShaderProgram() {
    ShaderInfo shader_info[ProgramCount][3] = { {
        { GL_VERTEX_SHADER,     "./vertexShader.glsl" },
        { GL_FRAGMENT_SHADER,   "./fragmentShader.glsl" },
        { GL_NONE, nullptr } } };

    for (int programItem = 0; programItem < ProgramCount; programItem++) {
        program[programItem] = LoadShaders(shader_info[programItem]);
        locationMatModel = glGetUniformLocation(program[programItem], "matModel");
        locationMatView = glGetUniformLocation(program[programItem], "matView");
        locationMatProjection = glGetUniformLocation(program[programItem], "matProjection");
        locationDrawColor = glGetUniformLocation(program[programItem], "drawColor");
    }
    /** Csatoljuk a vertex array objektumunkat a paraméterhez. */
    /** glBindVertexArray binds the vertex array object to the parameter. */
    glBindVertexArray(VAO[VAOVerticesData]);
    /** A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
    /** We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
    /** Másoljuk az adatokat a bufferbe! */
    /** Copy the data to the buffer! */
    bezierCurve = calculateBezierCurve(verticesData, 50); // Inicializáljuk a görbét
    glBufferData(GL_ARRAY_BUFFER, bezierCurve.size() * sizeof(vec2), bezierCurve.data(), GL_STATIC_DRAW);
    /** Engedélyezzük az attribútumot. */
    /** Enable the attribute. */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);
    /** Az aktuális program bekapcsolása. */
    /** Switch on the current sample program. */
    glUseProgram(program[QuadScreenProgram]);
    /** Mátrixok kezdőértékének beállítása. */
    /** Set the matrices to the initial values. */
    matModel = mat4(1.0);
    matView = lookAt(
        vec3(0.0f, 0.0f, 9.0f),     // camera position
        vec3(0.0f, 0.0f, 0.0f),     // look at point
        vec3(0.0f, 1.0f, 0.0f));    // up vector
    glUniformMatrix4fv(locationMatModel, 1, GL_FALSE, value_ptr(matModel));
    glUniformMatrix4fv(locationMatView, 1, GL_FALSE, value_ptr(matView));
    glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));
}

/* Bézier-görbe kiszámítása 4 kontrollpont alapján. */
/* Calculate Bézier curve based on 4 control points. */
vector<vec2> calculateBezierCurve(const vector<vec2>& controlPoints, int segments) {
    vector<vec2> curvePoints;
    int n = controlPoints.size() - 1; // Fokszám: n = kontrollpontok száma - 1
    if (n < 0) return curvePoints; // Ha nincs kontrollpont, üres vektort adunk vissza

    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        vec2 point(0.0f, 0.0f);
        for (int k = 0; k <= n; k++) {
            // Binomiális együttható: C(n, k) = n! / (k! * (n-k)!)
            float binomial = 1.0f;
            for (int j = 0; j < k; j++) binomial *= (float)(n - j) / (j + 1);
            // Bernstein-polinom: B(k,n,t) = C(n,k) * t^k * (1-t)^(n-k)
            float bernstein = binomial * pow(t, k) * pow(1.0f - t, n - k);
            point += bernstein * controlPoints[k];
        }
        curvePoints.push_back(point);
    }
    return curvePoints;
}

GLfloat distanceSquare(vec2 p1, vec2 p2) {
    vec2 delta = p1 - p2;
    return dot(delta, delta);
}

GLint getActivePoint(vector<vec2> p, GLfloat sensitivity, vec2 mousePosition) {
    GLfloat sensitivitySquare = sensitivity * sensitivity;
    for (GLint i = 0; i < p.size(); i++)
        if (distanceSquare(p[i], mousePosition) < sensitivitySquare)
            return i;
    return -1;
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Bézier-görbe kirajzolása (pl. kék szín)
    glUniform4f(locationDrawColor, 0.0f, 0.0f, 1.0f, 1.0f); 
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
    glBufferData(GL_ARRAY_BUFFER, bezierCurve.size() * sizeof(vec2), bezierCurve.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, bezierCurve.size());

    // Kontrollpoligon kirajzolása (pl. sárga szín)
    glUniform4f(locationDrawColor, 1.0f, 1.0f, 0.0f, 1.0f); 
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
    glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(vec2), verticesData.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_LINE_STRIP, 0, verticesData.size());

    // Kontrollpontok kirajzolása (pl. zöld szín)
    glUniform4f(locationDrawColor, 0.0f, 1.0f, 0.0f, 1.0f);
    glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
    glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(vec2), verticesData.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_POINTS, 0, verticesData.size());
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = glm::max(width, 1);
    windowHeight = glm::max(height, 1);

    aspectRatio = (float)windowWidth / (float)windowHeight;
    glViewport(0, 0, windowWidth, windowHeight);
    if (projectionType == Orthographic)
        if (windowWidth < windowHeight)
            matProjection = ortho(-worldSize, worldSize, -worldSize / aspectRatio, worldSize / aspectRatio, -100.0, 100.0);
        else
            matProjection = ortho(-worldSize * aspectRatio, worldSize * aspectRatio, -worldSize, worldSize, -100.0, 100.0);
    else
        matProjection = perspective(
            radians(45.0f),
            aspectRatio,
            0.1f,
            100.0f
        );
    glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (action == GLFW_PRESS)
        keyboard[key] = GL_TRUE;
    else if (action == GLFW_RELEASE)
        keyboard[key] = GL_FALSE;
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        projectionType = Orthographic;
        framebufferSizeCallback(window, windowWidth, windowHeight);
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        projectionType = Perspective;
        framebufferSizeCallback(window, windowWidth, windowHeight);
    }
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    if (dragged >= 0) {
        dvec2 mousePosition;
        cout << "cursorPosCallback\t\t\t" << xPos << "\t" << yPos << endl;
        mousePosition.x = xPos * 2.0f / (GLdouble)windowWidth - 1.0f;
        mousePosition.y = ((GLdouble)windowHeight - yPos) * 2.0f / (GLdouble)windowHeight - 1.0f;
        if (windowWidth < windowHeight)
            mousePosition.y /= aspectRatio;
        else
            mousePosition.x *= aspectRatio;
        cout << "cursorPosCallback normalized coords\t" << mousePosition.x << "\t" << mousePosition.y << endl;
        verticesData[dragged] = mousePosition;
        /** Görbe újraszámítása és frissítése a GPU-n. */
        /** Recalculate and update curve on GPU. */
        bezierCurve = calculateBezierCurve(verticesData, 50);
        glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
        glBufferData(GL_ARRAY_BUFFER, bezierCurve.size() * sizeof(vec2), bezierCurve.data(), GL_STATIC_DRAW);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    dvec2 mousePosition;
    glfwGetCursorPos(window, &mousePosition.x, &mousePosition.y);
    mousePosition.x = mousePosition.x * 2.0f / (GLdouble)windowWidth - 1.0f;
    mousePosition.y = ((GLdouble)windowHeight - mousePosition.y) * 2.0f / (GLdouble)windowHeight - 1.0f;
    if (windowWidth < windowHeight)
        mousePosition.y /= aspectRatio;
    else
        mousePosition.x *= aspectRatio;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // Ellenőrizzük, hogy van-e közeli kontrollpont (drag-and-drop)
        dragged = getActivePoint(verticesData, 0.1f, mousePosition);
        if (dragged == -1) { // Ha nincs közeli pont, új kontrollpont hozzáadása
            verticesData.push_back(vec2(mousePosition.x, mousePosition.y));
            // Görbe újraszámítása
            bezierCurve = calculateBezierCurve(verticesData, 50);
            // GPU buffer frissítése
            glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
            glBufferData(GL_ARRAY_BUFFER, bezierCurve.size() * sizeof(vec2), bezierCurve.data(), GL_STATIC_DRAW);
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        dragged = -1;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        GLint index = getActivePoint(verticesData, 0.1f, mousePosition);
        if (index != -1 && verticesData.size() > 2) { // Legalább 2 pont maradjon
            verticesData.erase(verticesData.begin() + index);
            bezierCurve = calculateBezierCurve(verticesData, 50);
            glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);
            glBufferData(GL_ARRAY_BUFFER, bezierCurve.size() * sizeof(vec2), bezierCurve.data(), GL_STATIC_DRAW);
        }
    }
}

int main(void) {
    init(3, 3, GLFW_OPENGL_COMPAT_PROFILE);
    initShaderProgram();
    framebufferSizeCallback(window, windowWidth, windowHeight);
    setlocale(LC_ALL, "");
    cout << "Drag-and-Drop using mouse" << endl << endl;
    cout << "Keyboard control" << endl;
    cout << "ESC\t\texit" << endl;
    cout << "O\t\tinduces orthographic projection" << endl;
    cout << "P\t\tinduces perspective projection" << endl << endl;
    cout << "Bézier Curve with 4 control points" << endl;
    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    cleanUpScene(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}