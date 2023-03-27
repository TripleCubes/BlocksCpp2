#ifndef GLOBALS_H
#define GLOBALS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "player.h"
#include "frameBuffer.h"
#include "raycast.h"
#include "camera.h"

const int INIT_WINDOW_WIDTH = 800;
const int INIT_WINDOW_HEIGHT = 500;
const int FPS_CAP = 60;

extern bool thirdPersonView;

extern Player player;
extern Camera mainCamera;

extern BlockRaycast currentBlockRaycast;

extern int currentWindowWidth;
extern int currentWindowHeight;

#endif