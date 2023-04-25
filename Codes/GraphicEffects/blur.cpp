#include "blur.h"
#include <glad/glad.h>
#include "../globals.h"

Shader GraphicEffects::Blur::shader;
Mesh GraphicEffects::Blur::mesh;
FrameBuffer GraphicEffects::Blur::reducedSizeFrameBufferMultisampled;
FrameBuffer GraphicEffects::Blur::reducedSizeFrameBuffer;

void GraphicEffects::Blur::init()
{
    shader.init("./Shaders/screen");

    std::vector<float> drawVerticies = {
        -1,  1,
         1,  1,
        -1, -1,
        
         1,  1,
         1, -1,
        -1, -1,
    };
    mesh.set2d(drawVerticies);

    reducedSizeFrameBufferMultisampled.init(true, true, currentWindowWidth/2, currentWindowHeight/2);
    reducedSizeFrameBuffer.init(false, true, currentWindowWidth/2, currentWindowHeight/2);
}

void GraphicEffects::Blur::createBlurTexture(Texture texture)
{
    createBlurTexture(texture.getTexture());
}

void GraphicEffects::Blur::createBlurTexture(unsigned int textureId)
{
    reducedSizeFrameBufferMultisampled.bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glViewport(0, 0, currentWindowWidth/2, currentWindowHeight/2);

    shader.useProgram();
    shader.setTextureUniform("viewTexture", textureId, 0);
    mesh.draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, reducedSizeFrameBufferMultisampled.getFrameBufferObject());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reducedSizeFrameBuffer.getFrameBufferObject());
    glBlitFramebuffer(0, 0, currentWindowWidth/2, currentWindowHeight/2, 
                        0, 0, currentWindowWidth/2, currentWindowHeight/2, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);
}

void GraphicEffects::Blur::draw()
{    
    shader.useProgram();
    shader.setTextureUniform("viewTexture", reducedSizeFrameBuffer.getTexture(), 0);
    mesh.draw();
}

void GraphicEffects::Blur::release()
{
    shader.release();
    mesh.release();
    reducedSizeFrameBuffer.release();
}
