enum eAnimationType {
	RotateFirst,
	TranslateFirst,
	Bouncing
};

#include "common.h"


static array<vec2, 36> vertices;
static array<vec2, 2> lineVertices;

void initVertices() {
	vertices[0] = vec2(0.0f, 0.0f);
	const float radius = 25.0f;
	for (int i = 0; i < 35; i++) {
		float angle = 2.0f * 3.14159f * i / 34.0f;
		vertices[i + 1] = vec2(radius * cos(angle), radius * sin(angle));
	}
	float lineLength = worldSize / 3.0; // Harmad oldalhosszúság
	lineVertices[0] = vec2(-lineLength / 1.0f, 0.0f); // Bal végpont
	lineVertices[1] = vec2(lineLength / 1.0f, 0.0f);  // Jobb végpont
}

GLchar			windowTitle[] = "Beadando_1";

GLuint			XoffsetLocation;
GLuint			YoffsetLocation;
GLfloat			x = 0.00f;
GLfloat			y = 0.00f;
GLfloat			increment = 0.01f;
GLboolean		xDir = GL_TRUE;
GLboolean		yDir = GL_FALSE;
mat4			translateMatrix = translate(mat4(1.0f), vec3(-0.5, 0.5, 0.0));
mat4			rotateMatrix = rotate(mat4(1.0f), (GLfloat)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
eAnimationType	animationType = Bouncing;

GLuint centerLocation;
GLuint radiusLocation;
GLfloat			lineY = 0.0f; 
GLuint lineYLocation;

GLboolean useAngle = GL_FALSE;
const float directionAngle = 25.0f * 3.14159f / 180.0f; 
GLfloat dirX = (10.0f / 600.0f) * cos(directionAngle); 
GLfloat dirY = (10.0f / 600.0f) * sin(directionAngle); 
GLboolean isMoving = GL_FALSE;

void initShaderProgram() {
	ShaderInfo shader_info[] = {
		{ GL_FRAGMENT_SHADER,	"./fragmentShader.glsl" },
		{ GL_VERTEX_SHADER,		"./vertexShader.glsl" },
		{ GL_NONE,				nullptr }
	};
	program[QuadScreenProgram] = LoadShaders(shader_info);

	glBindBuffer(GL_ARRAY_BUFFER, BO[VBOVerticesData]);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), vertices.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO[VAOVerticesData]);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, BO[VBOLineData]);
	glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(vec2), lineVertices.data(), GL_STATIC_DRAW);
	glBindVertexArray(VAO[VAOLineData]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	locationMatProjection = glGetUniformLocation(program[QuadScreenProgram], "matProjection");
	locationMatModelView = glGetUniformLocation(program[QuadScreenProgram], "matModelView");

	XoffsetLocation = glGetUniformLocation(program[QuadScreenProgram], "offsetX");
	YoffsetLocation = glGetUniformLocation(program[QuadScreenProgram], "offsetY");

	centerLocation = glGetUniformLocation(program[QuadScreenProgram], "center");
	radiusLocation = glGetUniformLocation(program[QuadScreenProgram], "radius");
	lineYLocation = glGetUniformLocation(program[QuadScreenProgram], "lineY");
	glUseProgram(program[QuadScreenProgram]);
}

void display(GLFWwindow* window, double currentTime) {
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);
	//float pixelRadius = 50.0f / (float)windowWidth; // 50 pixel normalizálva
	const float pixelRadius = 0.08333f; // 50 pixel / 600 pixel = 0.08333
	switch (animationType) {
	case Bouncing:
		matModel = mat4(1.0);
		if (!useAngle) { 
			if (xDir) {
				x += increment;
				if ((x > 1.0f - pixelRadius) || (x < -1.0f + pixelRadius)) increment = -increment;
				glProgramUniform1f(program[QuadScreenProgram], XoffsetLocation, x);
				glProgramUniform1f(program[QuadScreenProgram], YoffsetLocation, 0.0f);
			}
			else if (yDir) {
				y += increment;
				if ((y > 1.0f - pixelRadius) || (y < -1.0f + pixelRadius)) increment = -increment;
				glProgramUniform1f(program[QuadScreenProgram], XoffsetLocation, 0.0f);
				glProgramUniform1f(program[QuadScreenProgram], YoffsetLocation, y);
			}
		}
		else if (isMoving) { // 25°-os szögű mozgás
			x += dirX;
			y += dirY;

			// Visszapattanás a képernyő széleinél
			if (x + pixelRadius > 1.0f) { // Jobb szél
				x = 1.0f - pixelRadius;
				dirX = -dirX;
			}
			else if (x - pixelRadius < -1.0f) { // Bal szél
				x = -1.0f + pixelRadius;
				dirX = -dirX;
			}
			if (y + pixelRadius > 1.0f) { // Felső szél
				y = 1.0f - pixelRadius;
				dirY = -dirY;
			}
			else if (y - pixelRadius < -1.0f) { // Alsó szél
				y = -1.0f + pixelRadius;
				dirY = -dirY;
			}

			glProgramUniform1f(program[QuadScreenProgram], XoffsetLocation, x);
			glProgramUniform1f(program[QuadScreenProgram], YoffsetLocation, y);
		}
		break;
	}
	matModelView = matView * matModel;
	glUniformMatrix4fv(locationMatModelView, 1, GL_FALSE, value_ptr(matModelView));
	glProgramUniform2f(program[QuadScreenProgram], centerLocation, x, y); // Kör középpontja
	glProgramUniform1f(program[QuadScreenProgram], radiusLocation, pixelRadius); // Kör sugara
	glProgramUniform1f(program[QuadScreenProgram], lineYLocation, lineY); // Csík y pozíciója
	glBindVertexArray(VAO[VAOVerticesData]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 36); // Kör rajzolása

	// 2. Vonal rajzolása
	glLineWidth(3.0f); // 3 pixel vastagság
	glProgramUniform1f(program[QuadScreenProgram], XoffsetLocation, 0.0f); // Vonal középre vízszintesen
	glProgramUniform1f(program[QuadScreenProgram], YoffsetLocation, lineY); // Vonal pozíciója függőlegesen
	glProgramUniform2f(program[QuadScreenProgram], centerLocation, 0.0f, lineY); // Vonal középpontja
	glProgramUniform1f(program[QuadScreenProgram], radiusLocation, 1.0f); // Nagyobb radius a vonalhoz
	glBindVertexArray(VAO[VAOLineData]);
	glDrawArrays(GL_LINES, 0, 2); // Vonal rajzolása
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	/** A minimalizálás nem fog fagyni a minimum 1 értékkel. */
	windowWidth = glm::max(width, 1);
	windowHeight = glm::max(height, 1);

	float aspectRatio = (float)windowWidth / (float)windowHeight;
	/** A kezelt képernyõ beállítása a teljes (0, 0, width, height) területre. */
	glViewport(0, 0, windowWidth, windowHeight);
	/** Orthographic projekció beállítása, worldSize lesz a szélesség és magasság közül a kisebbik. */
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
	matModel = mat4(1.0);
	matView = lookAt(
		vec3(0.0f, 0.0f, 9.0f),
		vec3(0.0f, 0.0f, 0.0f),		
		vec3(0.0f, 1.0f, 0.0f));	
	matModelView = matView * matModel;
	glUniformMatrix4fv(locationMatModelView, 1, GL_FALSE, value_ptr(matModelView));
	glUniformMatrix4fv(locationMatProjection, 1, GL_FALSE, value_ptr(matProjection));
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;
	if (key == GLFW_KEY_B && action == GLFW_PRESS) animationType = Bouncing;
	/*if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		xDir = false;
		yDir = true;
		useAngle = GL_FALSE; // Visszaállítjuk az eredeti mozgást
	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS) {
		xDir = true;
		yDir = false;
		useAngle = GL_FALSE; // Visszaállítjuk az eredeti mozgást
	}*/
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_UP) {
			lineY += 0.05f;
			if (lineY > 1.0f) lineY = 1.0f;
		}
		else if (key == GLFW_KEY_DOWN) {
			lineY -= 0.05f;
			if (lineY < -1.0f) lineY = -1.0f;
		}
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		useAngle = GL_TRUE; // Átváltunk a 25°-os szögű mozgásra
		isMoving = GL_TRUE; // Biztosítjuk, hogy a kör mozogjon
		xDir = GL_FALSE;    // Kikapcsoljuk az eredeti vízszintes mozgást
		yDir = GL_FALSE;    // Kikapcsoljuk az eredeti függőleges mozgást
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	/** Az egér mutató helyét kezelõ függvény. */
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	/** Az egér gombjaihoz köthetõ események kezelése. */
}

int main(void) {
	/** Az alkalmazáshoz kapcsolódó elõkészítõ lépések (paraméterek: major, minor OpenGL verziók, OpenGL pofile). */
	init(3, 3, GLFW_OPENGL_COMPAT_PROFILE);
	/** A rajzoláshoz használt shader programok betöltése. */
	initVertices();
	initShaderProgram();
	/** Első értékadás a uniform változóknak). */
	framebufferSizeCallback(window, windowWidth, windowHeight);
	/** Karakterkódolás a szövegekhez. */
	setlocale(LC_ALL, "");
	/** A megadott window struktúra "close flag" vizsgálata. */
	while (!glfwWindowShouldClose(window)) {
		/** A kód, amellyel rajzolni tudunk a GLFWwindow objektumunkba. */
		display(window, glfwGetTime());
		/** Double buffered mûködés. */
		glfwSwapBuffers(window);
		/** Események kezelése az ablakunkkal kapcsolatban, pl. gombnyomás. */
		glfwPollEvents();
	}
	/** Felesleges objektumok törlése. */
	cleanUpScene(EXIT_SUCCESS);
	/** Kilépés EXIT_SUCCESS kóddal. */
	return EXIT_SUCCESS;
}
