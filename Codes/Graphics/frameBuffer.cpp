#include "frameBuffer.h"
#include <stdio.h>
#include <glad/glad.h>
#include "../globals.h"

void FrameBuffer::init(bool multisample, bool linearFilter, int width, int height)
{
    if (width == 0)
    {
        width = currentWindowWidth;
    }
    if (height == 0)
    {
        height = currentWindowHeight;
    }

    isMultisample = multisample;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &texture);
    
    if (!multisample)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linearFilter? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearFilter? GL_LINEAR : GL_NEAREST);  
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);  
    }

    if (!multisample)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);  
    }
    else
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0); 
    }



    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO); 
    if (!multisample)
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
    }
    else
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);   
    }
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("cant init frame buffer\n");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

unsigned int FrameBuffer::getFrameBufferObject()
{
    return FBO;
}

unsigned int FrameBuffer::getTexture()
{
    return texture;
}

void FrameBuffer::resize(int width, int height)
{
    if (width == 0)
    {
        width = currentWindowWidth;
    }
    if (height == 0)
    {
        height = currentWindowHeight;
    }
    
    if (!isMultisample)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, RBO); 
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); 
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    {
        
    }
}

void FrameBuffer::release()
{
    glDeleteTextures(1, &texture);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteFramebuffers(1, &FBO);  
}