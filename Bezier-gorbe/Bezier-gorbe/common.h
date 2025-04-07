// common.h - This will be included by both source files
#ifndef COMMON_H
#define COMMON_H

#include <array>
#include <fstream>
#include <GL/glew.h>
#include <gl/GLU.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
/** Szükséges az M_PI használatához. */
/** Needed for using M_PI. */
#define _USE_MATH_DEFINES
#include <math.h>
#include <SOIL2/SOIL2.h>
#include <sstream>
#include <vector>

// Enums that were previously in Source.cpp but are needed by common.cpp
enum eVertexArrayObject {
    VAOVerticesData,
    VAOLineData,    // Új VAO a szakasznak
    VAOCount
};

enum eBufferObject {
    VBOVerticesData,
    VBOLineData,    // Új VBO a szakasznak
    BOCount
};

enum eProgram {
    QuadScreenProgram,
    ProgramCount
};

enum eTexture {
    NoTexture,      // fixes 0 sized array problem
    TextureCount
};

enum eProjection {
    Orthographic,
    Perspective
};

using namespace std;
using namespace glm;

// Forward declarations for functions defined elsewhere
extern void framebufferSizeCallback(GLFWwindow* window, int width, int height);
extern void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);
extern void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
extern GLchar windowTitle[];

// Type definitions and common globals
typedef struct {
    GLenum       type;
    const GLchar* fileName;
    GLuint       shader;
} ShaderInfo;

extern GLuint VAO[VAOCount];
extern GLuint BO[BOCount];
extern GLuint program[ProgramCount];
extern GLuint texture[TextureCount];

extern const GLdouble worldSize;
extern GLint windowWidth;
extern GLint windowHeight;

extern GLuint locationMatModel, locationMatView, locationMatProjection, locationDrawColor, locationMatModelView;
extern mat4 matModel, matView, matProjection, matModelView;
extern eProjection projectionType;

extern GLboolean keyboard[512];
extern GLint modifiers;
extern GLFWwindow* window;

// Function declarations
void cleanUpScene(int returnCode);
GLboolean checkOpenGLError();
GLvoid checkShaderLog(GLuint shader);
GLvoid checkProgramLog(GLint prog, ShaderInfo* shaders);
string ReadShader(const GLchar* filename);
GLuint LoadShaders(ShaderInfo* shaders);
void init(GLint major, GLint minor, GLint profile);

#endif // COMMON_H