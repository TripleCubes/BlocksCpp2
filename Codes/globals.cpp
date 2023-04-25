#include "globals.h"

GLFWwindow* glfwWindow = NULL;

Color uiTextColor = Color(0.23, 0.27, 0.13, 1.0);
Color uiColor = Color(0.84, 0.92, 0.61, 1.0);

MenuGroup openingMenuGroup = MenuGroup::NONE;

bool mouseLock = true;

bool thirdPersonView = false;
bool flying = true;
bool wireframeMode = false;
bool ignoreCollision = false;

float fov = 70;

Player player;
Camera mainCamera;

BlockRaycast currentBlockRaycast;

int currentWindowWidth = INIT_WINDOW_WIDTH;
int currentWindowHeight = INIT_WINDOW_HEIGHT;