#include "graphics.h"
#include <glad/glad.h>
#include "../globals.h"
#include "../GraphicEffects/blur.h"
#include <stdio.h>

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

bool Graphics::initialized = false;

void Graphics::init()
{
    viewViewMat = glm::mat4(1.0f);
    viewProjectionMat = glm::mat4(1.0f);
                                        
    viewFrameBuffer.init();
    viewShader.init("./Shaders/viewVertex.glsl", "./Shaders/viewFragment.glsl");
    testTexture.load("./Textures/test.png", FilterType::NEAREST);

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

    initialized = true;
}

void Graphics::update()
{
    if (!initialized)
    {
        printf("Graphics not initialized\n");
        return;
    }

    viewProjectionMat = glm::perspective(glm::radians(fov), 
                                        (float)currentWindowWidth / (float)currentWindowHeight, 
                                        0.1f, 300.0f);

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
    if (!initialized)
    {
        printf("Graphics not initialized\n");
        return;
    }

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
    viewShader.setUniform("projectionMat", viewProjectionMat);
    ChunkLoader::draw();
    player.draw();
    if (!thirdPersonView && currentBlockRaycast.found)
    {
        UI::drawSelectedBlock();
    }



    // GraphicEffects::Blur::createBlurTexture(viewFrameBuffer.getTexture(), 8, 3);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    screenShader.useProgram();
    screenShader.setTextureUniform("viewTexture", viewFrameBuffer.getTexture(), 0);
    screenMesh.draw();
    // GraphicEffects::Blur::draw();

    if (!thirdPersonView)
    {
        UI::drawCrosshair();
    }
    UI::drawMenus();
}

glm::mat4 Graphics::getViewViewMat()
{
    if (!initialized)
    {
        printf("Graphics not initialized\n");
    }
    return viewViewMat;
}

glm::mat4 Graphics::getViewProjectionMat()
{
    if (!initialized)
    {
        printf("Graphics not initialized\n");
    }
    return viewProjectionMat;
}

Shader Graphics::getViewShader()
{
    if (!initialized)
    {
        printf("Graphics not initialized\n");
    }
    return viewShader;
}

FrameBuffer Graphics::getViewFrameBuffer()
{
    if (!initialized)
    {
        printf("Graphics not initialized\n");
    }
    return viewFrameBuffer;
}

void Graphics::release()
{
    viewShader.release();
    viewFrameBuffer.release();

    screenShader.release();
    screenMesh.release();
}