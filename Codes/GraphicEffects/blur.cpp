#include "blur.h"
#include <glad/glad.h>
#include "../globals.h"

Shader GraphicEffects::Blur::shader;
Mesh GraphicEffects::Blur::mesh;

FrameBuffer GraphicEffects::Blur::horizontalBlurFrameBuffer;
FrameBuffer GraphicEffects::Blur::blurFrameBuffer;

void GraphicEffects::Blur::init()
{
    shader.init("./Shaders/GraphicEffects/blur");

    std::vector<float> drawVerticies = {
        -1,  1,
         1,  1,
        -1, -1,
        
         1,  1,
         1, -1,
        -1, -1,
    };
    mesh.set2d(drawVerticies);

    horizontalBlurFrameBuffer.init();
    blurFrameBuffer.init();
}

void GraphicEffects::Blur::createBlurTexture1Time(unsigned int textureId)
{
    horizontalBlurFrameBuffer.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader.useProgram();
    shader.setUniform("horizontal", 1);
    shader.setTextureUniform("texture", textureId, 0);
    mesh.draw();

    blurFrameBuffer.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader.setUniform("horizontal", 0);
    shader.setTextureUniform("texture", horizontalBlurFrameBuffer.getTexture(), 0);
    mesh.draw();
}

void GraphicEffects::Blur::createBlurTexture(Texture texture, int blurSize, int blurTimes)
{
    createBlurTexture(texture.getTexture(), blurSize, blurTimes);
}

void GraphicEffects::Blur::createBlurTexture(unsigned int textureId, int blurSize, int blurTimes)
{
    shader.useProgram();
    shader.setUniform("blurSize", blurSize);
    createBlurTexture1Time(textureId);
    for (int i = 0; i < blurTimes - 1; i++)
    {
        createBlurTexture1Time(blurFrameBuffer.getTexture());
    }
}

void GraphicEffects::Blur::draw()
{    
    shader.useProgram();
    shader.setTextureUniform("viewTexture", blurFrameBuffer.getTexture(), 0);
    mesh.draw();
}

void GraphicEffects::Blur::release()
{
    shader.release();
    mesh.release();
    horizontalBlurFrameBuffer.release();
    blurFrameBuffer.release();
}
