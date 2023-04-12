#include "globals.h"

GLFWwindow* glfwWindow = NULL;

bool gamePaused = false;
bool mouseLock = true;

bool thirdPersonView = false;
bool flying = true;
bool wireframeMode = false;
bool ignoreCollision = false;

Player player;
Camera mainCamera;

BlockRaycast currentBlockRaycast;

int currentWindowWidth = INIT_WINDOW_WIDTH;
int currentWindowHeight = INIT_WINDOW_HEIGHT;

void pauseGame()
{
    gamePaused = true;
    mouseLock = false;
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
    glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
}

void resumeGame()
{
    gamePaused = false;
    mouseLock = true;
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}