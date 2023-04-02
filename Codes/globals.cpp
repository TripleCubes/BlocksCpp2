#include "globals.h"

GLFWwindow* glfwWindow = NULL;

bool thirdPersonView = false;
bool flying = true;
bool wireframeMode = false;
bool ignoreCollision = false;

Player player;
Camera mainCamera;

BlockRaycast currentBlockRaycast;

int currentWindowWidth = INIT_WINDOW_WIDTH;
int currentWindowHeight = INIT_WINDOW_HEIGHT;