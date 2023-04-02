#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <thread>

#include "globals.h"
#include "vectormath.h"
#include "shader.h"
#include "mesh.h"
#include "frameBuffer.h"
#include "texture.h"
#include "time.h"
#include "chunkLoader.h"
#include "camera.h"
#include "ui.h"
#include "graphics.h"

#include "threadControls.h"

float cameraRotationX = 0;
float cameraRotationY = 0;

float lastMousePosX = INIT_WINDOW_WIDTH / 2;
float lastMousePosY = INIT_WINDOW_HEIGHT / 2;
bool mouseInWindow = false;

bool mouseLock = true;

void onWindowResize(GLFWwindow *window, int width, int height)
{
    if (width != 0 && height != 0)
    {
        currentWindowWidth = width;
        currentWindowHeight = height;

        glm::mat4 projectionMat = glm::perspective(glm::radians(70.0f), (float)currentWindowWidth / (float)currentWindowHeight, 0.1f, 300.0f);
        Graphics::getViewShader().useProgram();
        Graphics::getViewShader().setUniform("projectionMat", projectionMat);
        
        glViewport(0, 0, width, height);
        Graphics::getViewFrameBuffer().resize();
    }
}

void onMouseMove(GLFWwindow* window, double mousex, double mousey)
{
    if (!mouseLock) 
    {
        mouseInWindow = false;
        return;
    }

    float offsetx = mousex - lastMousePosX;
    float offsety = mousey - lastMousePosY;
    lastMousePosX = mousex;
    lastMousePosY = mousey;

    if (mouseInWindow)
    {
        cameraRotationX -= offsety * 0.17;
        if (cameraRotationX > 89.99)
        {
            cameraRotationX = 89.99f;
        }
        else if (cameraRotationX < -89.99)
        {
            cameraRotationX = -89.99f;
        }
        cameraRotationY -= offsetx * 0.17;

        Vec3 lookDir = Vec3(0, 0, -1);
        lookDir = lookDir.rotateX(cameraRotationX);
        lookDir = lookDir.rotateY(cameraRotationY);
        mainCamera.lookDir = lookDir;

        Vec3 playerFrontDir = Vec3(0, 0, -1);
        playerFrontDir = playerFrontDir.rotateY(cameraRotationY);
        player.frontDir = playerFrontDir;
    }
    else
    {
        mouseInWindow = true;
    }
}

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        mouseLock = !mouseLock;
        if (mouseLock)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        }
    }

    else if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        thirdPersonView = !thirdPersonView;
    }

    else if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        flying = !flying;
    }

    else if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        wireframeMode = !wireframeMode;
    }

    else if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        ignoreCollision = !ignoreCollision;
    }
}

void onMouseClick(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        if (currentBlockRaycast.found)
        {
            ChunkLoader::placeBlock(Block(TEST, IntPos(currentBlockRaycast.blockPlacingPos)));
        }
    }

    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (currentBlockRaycast.found)
        {
            ChunkLoader::breakBlock(currentBlockRaycast.selectedBlockPos);
        }
    }
}

void initOpenGL()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, "Blocks", NULL, NULL);
    if (glfwWindow == NULL)
    {
        printf("failed to create window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("failed to initialize GLAD\n");
        return;
    }

    glViewport(0, 0, INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(glfwWindow, onWindowResize);
    glfwSetCursorPosCallback(glfwWindow, onMouseMove);
    glfwSetKeyCallback(glfwWindow, onKeyPress);
    glfwSetMouseButtonCallback(glfwWindow, onMouseClick);
}

int main()
{
    using namespace std::chrono;
    using namespace std::this_thread;

    srand(time(0));

    initOpenGL();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    Graphics::init();

    ChunkLoader::init();
    UI::init();

    player.init(Vec3(0, 0, 10), Vec3(0, 0, -1));
    mainCamera.init(player);

    ThreadControls::init();

    while (!glfwWindowShouldClose(glfwWindow))
    {
        float frameStartTime = glfwGetTime();

        ThreadControls::lockMainThread();
        ChunkLoader::update();

        if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        {
            player.move(player.frontDir);
        }
        if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        {
            player.move(player.frontDir.rotateY(90));
        }
        if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        {
            player.move(player.frontDir * -1);
        }
        if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        {
            player.move(player.frontDir.rotateY(-90));
        }
        if (flying || ignoreCollision)
        {
            if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                player.move(Vec3(0, 1, 0));
            }
            if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            {
                player.move(Vec3(0, -1, 0));
            }
        }
        else
        {
            if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
            {
                player.jump();
            }
        }

        player.update();

        currentBlockRaycast = BlockRaycast(Vec3(mainCamera.pos.x, mainCamera.pos.y + 1.5, mainCamera.pos.z), mainCamera.lookDir, 5);
        mainCamera.update(player);

        Graphics::update();
        Graphics::draw();

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();

        ThreadControls::markMainThreadCycleFinished();

        float frameTime = glfwGetTime() - frameStartTime;
        sleep_for(milliseconds((int)round((1/(float)FPS_CAP - frameTime)* 1000)));

        deltaTime = glfwGetTime() - previousTime;
        previousTime = glfwGetTime();
    }

    ThreadControls::release();
    player.release();
    ChunkLoader::release();
    UI::release();
    Graphics::release();
    glfwTerminate();

    return 0;
}