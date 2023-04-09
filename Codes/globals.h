#ifndef GLOBALS_H
#define GLOBALS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Graphics/shader.h"
#include "Graphics/frameBuffer.h"
#include "Entities/player.h"
#include "Entities/camera.h"
#include "Entities/raycast.h"

const int INIT_WINDOW_WIDTH = 800;
const int INIT_WINDOW_HEIGHT = 500;
const int FPS_CAP = 60;

extern GLFWwindow* glfwWindow;

extern bool gamePaused;

extern bool thirdPersonView;
extern bool flying;
extern bool wireframeMode;
extern bool ignoreCollision;

extern Player player;
extern Camera mainCamera;

extern BlockRaycast currentBlockRaycast;

extern int currentWindowWidth;
extern int currentWindowHeight;

#endif