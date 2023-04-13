#include "graphics.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../globals.h"

class ChunkLoader
{
    public:
        static void draw();
};

class UI
{
    public:
        static void drawMenus();
        static void drawSelectedBlock();
        static void drawCrosshair();
};

glm::mat4 Graphics::viewViewMat;
glm::mat4 Graphics::viewProjectionMat;

Shader Graphics::viewShader;
FrameBuffer Graphics::viewFrameBuffer;
Texture Graphics::testTexture;

Shader Graphics::screenShader;
Mesh Graphics::screenMesh;

void Graphics::init()
{
    viewViewMat = glm::mat4(1.0f);
    viewProjectionMat = glm::perspective(glm::radians(70.0f), 
                                        (float)INIT_WINDOW_WIDTH / (float)INIT_WINDOW_HEIGHT, 
                                        0.1f, 300.0f);
                                        
    viewFrameBuffer.init(false, 1);
    viewShader.init("./Shaders/viewVertex.glsl", "./Shaders/viewFragment.glsl");
    viewShader.useProgram();
    viewShader.setUniform("projectionMat", viewProjectionMat);
    testTexture.load("./Textures/test.png", NEAREST);

    screenShader.init("./Shaders/screenVertex.glsl", "./Shaders/screenFragment.glsl");
    std::vector<float> screenVerticies = {
        -1,  1,
         1,  1,
        -1, -1,
        
         1,  1,
         1, -1,
        -1, -1
    };
    screenMesh.set2d(screenVerticies);
}

void Graphics::update()
{
    if (!thirdPersonView)
    {
        viewViewMat = glm::lookAt(toGlmVec3(mainCamera.pos + Vec3(0, 1.5, 0)),
                                toGlmVec3(mainCamera.pos + Vec3(0, 1.5, 0) + mainCamera.lookDir),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else
    {
        viewViewMat = glm::lookAt(toGlmVec3(mainCamera.pos + Vec3(0, 1.5, 0) - (mainCamera.lookDir * 10)),
                                toGlmVec3(mainCamera.pos + Vec3(0, 1.5, 0)),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

void Graphics::draw()
{
    viewFrameBuffer.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    if (wireframeMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glLineWidth(1);

    viewShader.useProgram();
    viewShader.setUniform("viewMat", viewViewMat);
    ChunkLoader::draw();
    player.draw();
    if (!thirdPersonView && currentBlockRaycast.found)
    {
        UI::drawSelectedBlock();
    }



    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    screenShader.useProgram();
    screenShader.setTextureUniform("viewTexture", viewFrameBuffer.getTexture(), 0);
    screenMesh.draw();

    if (!thirdPersonView)
    {
        UI::drawCrosshair();
    }
    UI::drawMenus();
}

glm::mat4 Graphics::getViewViewMat()
{
    return viewViewMat;
}

glm::mat4 Graphics::getViewProjectionMat()
{
    return viewProjectionMat;
}

Shader Graphics::getViewShader()
{
    return viewShader;
}

FrameBuffer Graphics::getViewFrameBuffer()
{
    return viewFrameBuffer;
}

void Graphics::release()
{
    viewShader.release();
    viewFrameBuffer.release();

    screenShader.release();
    screenMesh.release();
}