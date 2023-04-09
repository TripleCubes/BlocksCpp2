#include "ui.h"
#include "../globals.h"
#include "../Entities/camera.h"
#include "../Graphics/graphics.h"

Mesh UI::crosshairMesh;
Shader UI::crosshairShader;
Mesh UI::blockSelectionMesh;
Shader UI::blockSelectionShader;
Mesh UI::rectMesh;
Shader UI::rectShader;

Button UI::testButton(50, 50, 100, 30, Color(0.84, 0.92, 0.61, 1.0));

void UI::init()
{
    std::vector<float> crosshairVerticies = {
        -(float)CROSSHAIR_WIDTH  / 2,  (float)CROSSHAIR_HEIGHT / 2, // A 0
         (float)CROSSHAIR_WIDTH  / 2,  (float)CROSSHAIR_HEIGHT / 2, // B 1
         (float)CROSSHAIR_WIDTH  / 2, -(float)CROSSHAIR_HEIGHT / 2, // C 2
        -(float)CROSSHAIR_WIDTH  / 2, -(float)CROSSHAIR_HEIGHT / 2, // D 3
        -(float)CROSSHAIR_HEIGHT / 2,  (float)CROSSHAIR_WIDTH  / 2, // E 4
         (float)CROSSHAIR_HEIGHT / 2,  (float)CROSSHAIR_WIDTH  / 2, // F 5
         (float)CROSSHAIR_HEIGHT / 2, -(float)CROSSHAIR_WIDTH  / 2, // G 6
        -(float)CROSSHAIR_HEIGHT / 2, -(float)CROSSHAIR_WIDTH  / 2, // H 7
    };
    std::vector<unsigned int> crosshairIndicies = {
        0, 2, 3, 
        0, 1, 2, 
        4, 5, 6, 
        4, 6, 7
    };

    crosshairMesh.set2d(crosshairVerticies, crosshairIndicies);
    crosshairShader.init("./Shaders/UI/crosshair");
    crosshairShader.useProgram();
    crosshairShader.setUniform("currentWindowSize", (float)currentWindowWidth, (float)currentWindowHeight);



    blockSelectionShader.init("./Shaders/UI/blockSelection");
    blockSelectionShader.useProgram();
    blockSelectionShader.setUniform("projectionMat", Graphics::getViewProjectionMat());

    std::vector<float> blockSelectionVerticies = {
        -0.501,  0.501, -0.501, // A 0
         0.501,  0.501, -0.501, // B 1
         0.501,  0.501,  0.501, // C 2
        -0.501,  0.501,  0.501, // D 3
        -0.501, -0.501, -0.501, // E 4
         0.501, -0.501, -0.501, // F 5
         0.501, -0.501,  0.501, // G 6
        -0.501, -0.501,  0.501, // H 7
    };
    std::vector<unsigned int> blockSelectionIndicies = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        0, 4,
        1, 5,
        2, 6,
        3, 7,
        4, 5,
        5, 6,
        6, 7,
        7, 4
    };
    blockSelectionMesh.set(blockSelectionVerticies, blockSelectionIndicies);



    std::vector<float> rectVerticies = {
        0, 1,
        1, 1,
        0, 0,
        
        1, 1,
        1, 0,
        0, 0
    };
    rectMesh.set2d(rectVerticies);
    rectShader.init("./Shaders/UI/rect");
    rectShader.useProgram();
    rectShader.setUniform("windowSize", (float)currentWindowWidth, (float)currentWindowHeight);
}

void UI::update()
{
    testButton.update();
    if (testButton.leftMouseHold())
    {
        printf("left clicked\n");
    }
}

void UI::drawCrosshair()
{
    crosshairShader.useProgram();
    crosshairShader.setUniform("viewTexture", Graphics::getViewFrameBuffer().getTexture(), 0);
    crosshairMesh.draw();
}

void UI::drawSelectedBlock()
{
    glm::mat4 blockSelectionModelMat = glm::mat4(1.0f);
    blockSelectionModelMat = glm::translate(blockSelectionModelMat, glm::vec3(currentBlockRaycast.selectedBlockPos.x + 0.5, 
                                                                                currentBlockRaycast.selectedBlockPos.y + 0.5, 
                                                                                currentBlockRaycast.selectedBlockPos.z + 0.5));
                                
    blockSelectionShader.useProgram();
    blockSelectionShader.setUniform("viewMat", Graphics::getViewViewMat());
    blockSelectionShader.setUniform("modelMat", blockSelectionModelMat);
    blockSelectionMesh.draw(true);
}

void UI::hidePauseMenu()
{
    testButton.hide();
}

void UI::showPauseMenu()
{
    testButton.show();
}

void UI::drawPauseMenu()
{
    testButton.draw();
}

void UI::drawRectPos(float x1, float y1, float x2, float y2, Color color)
{
    if (x1 > x2)
    {
        float x3 = x1;
        x1 = x2;
        x2 = x3;
    }
    if (y1 > y2)
    {
        float y3 = y1;
        y1 = y2;
        y2 = y3;
    }

    float y3 = y1;
    y1 = currentWindowHeight - y2;
    y2 = currentWindowHeight - y3;

    rectShader.useProgram();
    rectShader.setUniform("rectPos", x1, y1);
    rectShader.setUniform("rectSize", x2 - x1, y2 - y1);
    rectShader.setUniform("rectColor", color);
    rectMesh.draw();
}

void UI::drawRectWH(float x, float y, float w, float h, Color color)
{
    drawRectPos(x, y, x + w, y + h, color);
}

void UI::release()
{
    crosshairMesh.release();
    crosshairShader.release();

    blockSelectionMesh.release();
    blockSelectionShader.release();

    rectMesh.release();
    rectShader.release();
}