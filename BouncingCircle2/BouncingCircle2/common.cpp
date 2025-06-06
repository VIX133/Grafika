﻿/** The usual stuff... */
#ifndef COMMON_CPP
#define COMMON_CPP

#include "common.h"

/** Vertex buffer object ID, vertex array object ID és program ID az adattároláshoz. */
/** Vertex buffer object ID, vertex array object ID and program ID for data storing. */
GLuint			VAO[VAOCount] = { 0 };
GLuint			BO[BOCount] = { 0 };
GLuint			program[ProgramCount] = { 0 };
GLuint			texture[TextureCount] = { 0 };
/** Az OpenGL ablak szélesség és magasság értéke. */
/** Width and height of the OpenGL window. */
const GLdouble	worldSize = 1.0f;
GLint			windowWidth = 600;
GLint			windowHeight = 600;
/** A shader változók location-jei és a CPU oldali változóik. */
/** Shader variable locations and the corresponding CPU variables. */
GLuint			locationMatModel, locationMatView, locationMatProjection, locationMatModelView;
mat4			matModel, matView, matProjection, matModelView;
eProjection		projectionType = Orthographic;
/** A normál billentyûk a [0..255] tartományban vannak, a nyilak és a speciális billentyûk pedig a [256..511] tartományban helyezkednek el. */
/** Normal keys are fom [0..255], arrow and special keys are from [256..511]. */
GLboolean		keyboard[512] = { GL_FALSE };
GLint			modifiers;		// handles CTRL, ALT, SHIFT, etc...
GLFWwindow* window = nullptr;

/** Felesleges objektumok törlése. */
/** Clenup the unnecessary objects. */
void cleanUpScene(int returnCode) {
	/** Töröljük a texture objektumokat. */
	/** Destroy the texture objects. */
	glDeleteTextures(TextureCount, texture);
	/** Töröljük a vertex array és a vertex buffer objektumokat. */
	/** Destroy the vertex array and vertex buffer objects. */
	glDeleteVertexArrays(VAOCount, VAO);
	glDeleteBuffers(BOCount, BO);
	/** Töröljük a shader programo(ka)t. */
	/** Let's delete the shader program(s). */
	for (int enumItem = 0; enumItem < ProgramCount; enumItem++)
		glDeleteProgram(enumItem);
	/** Töröljük a GLFW ablakot. Leállítjuk a GLFW-t. */
	/** Destroy the GLFW window. Stop the GLFW system. */
	glfwTerminate();
	/** Kilépés returnCode kóddal. */
	/** Stop the software and exit with returnCode code. */
	exit(returnCode);
}

GLboolean checkOpenGLError() {
	GLboolean	foundError = GL_FALSE;
	GLenum		glErr;
	/** Vizsgáljuk meg, hogy van-e aktuálisan OpenGL hiba, és amennyiben igen, írassuk ki azokat a hiba konzolra egyenként. */
	/** Check for OpenGL errors, and send them to the error console ony by one. */
	while ((glErr = glGetError()) != GL_NO_ERROR) {
		cerr << "glError: " << gluErrorString(glErr) << endl;
		foundError = GL_TRUE;
	}
	/** Ha van aktuálisan OpenGL hiba, a visszatérési érték true. */
	/** If there are OpenGL errors, the return value is true. */
	return foundError;
}

GLvoid checkShaderLog(GLuint shader) {
	GLint	compiled;
	GLint	length = 0;
	/** Vizsgáljuk meg, hogy shader lefordult-e, OK -> visszatérünk. */
	/** Check for compilation status of shader, OK -> return. */
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled) return;
	/** Vizsgáljuk meg, hogy van-e valami a Shader Info Logban, és amennyiben igen, írassuk ki azt a hiba konzolra. */
	/** Check for Shader Info Log, and send it to the error console if it is created for the last compile. */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		GLchar* log = (GLchar*)calloc((size_t)length + 1, sizeof(GLchar)); // length + 1 for zero terminated string
		/** Olvassuk és írassuk ki a logot a hiba konzolra. */
		/** Read out and and send to the error console the log. */
		glGetShaderInfoLog(shader, length, nullptr, log);
		cerr << "Shader compiling failed, info log: " << log << endl;
		free(log);
	}
	cleanUpScene(EXIT_FAILURE);
}

GLvoid checkProgramLog(GLint prog, ShaderInfo* shaders) {
	GLint	linked;
	GLint	length = 0;
	/** Vizsgáljuk meg, hogy shaderek linkelhetoek-e, OK -> visszatérünk, NOT -> eroforrások felszabadítása. */
	/** Check for linking status of shaders, OK -> return, NOT -> release resources. */
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
	if (linked) return;
	for (ShaderInfo* entry = shaders; entry->type != GL_NONE; ++entry) {
		glDeleteShader(entry->shader);
		entry->shader = 0;
	}
	/** Vizsgáljuk meg, hogy van-e valami a Program Info Logban, és amennyiben igen, írassuk ki azt a konzolra soronként. */
	/** Check for Program Info Log, and send it to the console by lines if it is created for the last compile. */
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
	if (length > 0) {
		GLchar* log = (GLchar*)calloc((size_t)length + 1, sizeof(GLchar)); // length + 1 for zero terminated string
		/** Olvassuk és írassuk ki a logot a hiba konzolra. */
		/** Read out and and send to the error console the log. */
		glGetProgramInfoLog(prog, length, nullptr, log);
		cerr << "Shader linking failed, info log: " << log << endl;
		free(log);
	}
	cleanUpScene(EXIT_FAILURE);
}

string ReadShader(const GLchar* filename) {
	/** A file stream inicializálása olvasásra és kilépünk hiba esetén. */
	/** Let's initialize the file stream for reading and exit on error. */
	ifstream		t(filename);
	stringstream	buffer;

	if (t.fail()) cleanUpScene(EXIT_FAILURE);
	/** A shader fájl sorainak beolvasása EOF jelzésig. (EOF = End Of File) */
	/** Read in the lines of the shader file until EOF. (EOF = End Of File) */
	buffer << t.rdbuf();
	/** Visszatérés a shader fájl tartalmával egy string objektumban. */
	/** Return the content of the shader file in a string object. */
	return buffer.str();
}

GLuint LoadShaders(ShaderInfo* shaders) {
	if (shaders == nullptr) return 0; // 0 = NOT valid program
	/** A glCreateProgram egy üres program objektumot készít, a nem nulla azonosítóval lehet késobb hivatkozni. */
	/** glCreateProgram creates an empty program object and returns a non-zero value by which it can be referenced. */
	GLuint		program = glCreateProgram();
	ShaderInfo* entry = shaders;
	/** A glCreateShader egy üres entry->type shader objektumot készít, a nem nulla azonosítóval lehet késobb hivatkozni. */
	/** glCreateShader creates an empty shader object for entry->type and returns a non-zero value by which it can be referenced. */
	while (entry->type != GL_NONE) {
		GLuint			shader = glCreateShader(entry->type);
		string			sourceString = ReadShader(entry->fileName);
		const GLchar* source = sourceString.c_str();

		entry->shader = shader;
		/** Amikor source üres, töröljük a köztes objektumokat és kilépünk. */
		/** If source is empty string, delete the intermediate objects and exit. */
		if (source == nullptr) {
			for (entry = shaders; entry->type != GL_NONE; ++entry) {
				glDeleteShader(entry->shader);
				entry->shader = 0;
			}
			cleanUpScene(EXIT_FAILURE);
		}
		/** A shader forrás (1 db) hozzárendelése és fordítása, hiba ellenorzése. */
		/** Assign and compile shader source (1 piece), check for error. */
		/** void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length); */
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		checkShaderLog(shader);
		/** A shader programhoz csatlakoztatása. */
		/** Attach shader to program. */
		glAttachShader(program, shader);
		/** Következo bejegyzés feldolgozása. */
		/** Process next entry. */
		++entry;
	}
	/** Végül a GLSL szerkesztõ ellenõrzi, hogy a csatolt shaderek kompatibilisek-e egymással. */
	/** A shaderek linkelése a programban, hiba ellenorzése. */
	/** GLSL linker checks the shaders for compatibility. */
	/** Link shaders together in program, check for error. */
	glLinkProgram(program);
	checkProgramLog(program, shaders);
	/** Ha minden rendben ment a linkelés során, a shader objektumok törölhetõek. */
	/** If everything is OK at linking, the shader objects can be destroyed. */
	for (entry = shaders; entry->type != GL_NONE; ++entry) {
		glDeleteShader(entry->shader);
		entry->shader = 0;
	}
	/** Siker, visszatérés a program azonosítójával. */
	/** Success, return program ID. */
	return program;
}
/** Az alkalmazáshoz kapcsolódó elõkészítõ lépések. */
/** The first initialization steps of the program. */
void init(GLint major, GLint minor, GLint profile) {
	/** Próbáljuk meg inicializálni a GLFW-t! */
	/** Try to initialize GLFW! */
	if (!glfwInit()) cleanUpScene(EXIT_FAILURE);
	/** A használni kívánt OpenGL verziót beállítjuk. */
	/** The needed OpenGL version is set up. */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	/** GLFW_OPENGL_CORE_PROFILE-t használunk alapesetben. A közvetlen mód glBegin és glEnd között glVertex hívásokat használ, ekkor GLFW_OPENGL_COMPAT_PROFILE szükséges. */
	/** GLFW_OPENGL_CORE_PROFILE is default. Immediate mode is using glBegin and glEnd with glVertex in between them, then GLFW_OPENGL_COMPAT_PROFILE is needed. */
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
#ifdef __APPLE__	// To make macOS happy; should not be needed.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	/** Próbáljuk meg létrehozni az ablakunkat. */
	/** Let's try to create a window for drawing. */
	/** GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor * monitor, GLFWwindow * share) */
	if ((window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr)) == nullptr) {
		cerr << "Failed to create GLFW window." << endl;
		cleanUpScene(EXIT_FAILURE);
	}
	/** Válasszuk ki az ablakunk OpenGL kontextusát, hogy használhassuk. */
	/** Select the OpenGL context (window) for drawing. */
	glfwMakeContextCurrent(window);
	/** A képernyõ átméretezés kezelése. */
	/** Callback function for window size change. */
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	/** Billentyûzethez köthetõ események kezelése. */
	/** Callback function for keyboard events. */
	glfwSetKeyCallback(window, keyCallback);
	/** Az egér mutató helyét kezelõ függvény megadása. */
	/** Callback function for mouse position change. */
	glfwSetCursorPosCallback(window, cursorPosCallback);
	/** Az egér gombjaihoz köthetõ események kezelése. */
	/** Callback function for mouse button events. */
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	/** Incializáljuk a GLEW-t, hogy elérhetõvé váljanak az OpenGL függvények, probléma esetén kilépés EXIT_FAILURE értékkel. */
	/** Initalize GLEW, so the OpenGL functions will be available, on problem exit with EXIT_FAILURE code. */
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to init the GLEW system." << endl;
		cleanUpScene(EXIT_FAILURE);
	}
	/** 0 = v-sync kikapcsolva, 1 = v-sync bekapcsolva, n = n db képkockányi idõt várakozunk */
	/** 0 = v-sync off, 1 = v-sync on, n = n pieces frame time waiting */
	glfwSwapInterval(1);
	/** A window ablak minimum és maximum szélességének és magasságának beállítása. */
	/** The minimum and maximum width and height values of the window object. */
	glfwSetWindowSizeLimits(window, 400, 400, 3840, 2160);
	/** Elsődleges monitor és annak üzemmód lekérdezése. */
	/** Query primary monitor and its working resolution. */
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	/** Ablak középre helyezése. */
	/** Putting window in the center. */
	glfwSetWindowPos(window, (mode->width - windowWidth) / 2, (mode->height - windowHeight) / 2);
	/** Létrehozzuk a szükséges vertex buffer és vertex array objektumokat. */
	/** Create the vertex buffer and vertex array objects. */
	glGenBuffers(BOCount, BO);
	glGenVertexArrays(VAOCount, VAO);
	/** A GL_POINT_SMOOTH funkció bekapcsolása után négyzet helyett kör alakú pontokat kapunk. */
	/** Enabling GL_POINT_SMOOTH produce circle points instead of rectangles. */
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	/** Ha pontokat rajzolunk, kísérletezzünk a pont méretének növelésével! */
	/** When drawing points, we can make experiments with different point sizes! */
	glPointSize(32.0);
	/** Ha vonalakat rajzolunk, módosítsuk a vastagságát! */
	/** When drawing lines, we can make experiments with different widths! */
	glLineWidth(16.0);
	/** Állítsuk be a törlési színt az áttetszõségi értékkel együtt! [0.0, 1.0] */
	/** Set the clear color (red, green, blue, alpha), where alpha is transparency! [0.0, 1.0] */
	glClearColor(GLclampf(212.0 / 255.0), GLclampf(175.0 / 255.0), GLclampf(55.0 / 255.0), 1.0); // gold
	/** Állítsuk be a számok kiírási pontosságát 2 tizedesre. */
	/** Set feedback number precision to 2 decimal. */
	cout.precision(2);
	cout << fixed;
}
#endif