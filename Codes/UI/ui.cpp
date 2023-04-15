#include "ui.h"

#include "../globals.h"
#include "../input.h"

#include "../Graphics/graphics.h"
#include "../Graphics/text.h"

#include "button.h"
#include "toggleButton.h"
#include "slider.h"
#include "textbox.h"
#include "curveGraph.h"

#include <memory>

Mesh UI::crosshairMesh;
Shader UI::crosshairShader;
Mesh UI::blockSelectionMesh;
Shader UI::blockSelectionShader;
Mesh UI::rectMesh;
Shader UI::rectShader;

Menu UI::pauseMenu;
Menu UI::settingsMenu;
Menu UI::devMenu;

void UI::initMenus()
{
    {
        pauseMenu.init(PAUSE);
        pauseMenu.onUpdate = [](Menu* self)
        {
            if (Input::justPressed("ESC") && pauseMenu.getShown() && !pauseMenu.getShownFirstFrame())
            {
                mouseLock = true;
                glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

                pauseMenu.hide();
            }
        };
        Button backToGameButton;
        Button settingsButton;
        Button saveAndQuitButton;
        backToGameButton.init(50, 50, 150, 30, uiColor, "back to game", uiTextColor);
        backToGameButton.onLeftMouseDown = [](Button* self)
        {
            mouseLock = true;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

            pauseMenu.hide();
        };
        settingsButton.init(50, 90, 150, 30, uiColor, "settings", uiTextColor);
        settingsButton.onLeftMouseDown = [](Button* self)
        {
            pauseMenu.hide();
            settingsMenu.show();
        };
        saveAndQuitButton.init(50, 130, 150, 30, uiColor, "save and quit", uiTextColor);
        pauseMenu.add(std::make_unique<Button>(backToGameButton));
        pauseMenu.add(std::make_unique<Button>(settingsButton));
        pauseMenu.add(std::make_unique<Button>(saveAndQuitButton));
    }

    {
        settingsMenu.init(PAUSE);
        settingsMenu.onUpdate = [](Menu* self)
        {
            if (Input::justPressed("ESC") && settingsMenu.getShown())
            {
                settingsMenu.hide();
                pauseMenu.show();
            }
        };
        Button backButton;
        Textbox toggleWireframeModeTextbox;
        ToggleButton toggleWireframeModeButton;
        backButton.init(50, 50, 150, 30, uiColor, "back", uiTextColor);
        backButton.onLeftMouseDown = [](Button* self){
            settingsMenu.hide();
            pauseMenu.show();
        };
        toggleWireframeModeTextbox.init(57, 105, "wireframe mode", uiColor, false, true);
        toggleWireframeModeButton.init(250, 90, 50, 30, uiColor, {"off", "on"}, uiTextColor);
        toggleWireframeModeButton.onLeftMouseDown = [](ToggleButton* self){
            if (self->getText() == "on")
            {
                wireframeMode = true;
            }
            else
            {
                wireframeMode = false;
            }
        };
        toggleWireframeModeButton.onShow = [](ToggleButton* self){
            self->setText(wireframeMode ? "on" : "off");
        };
        settingsMenu.add(std::make_unique<Button>(backButton));
        settingsMenu.add(std::make_unique<Textbox>(toggleWireframeModeTextbox));
        settingsMenu.add(std::make_unique<ToggleButton>(toggleWireframeModeButton));
    }
    
    {
        devMenu.init(DEV);
        devMenu.onUpdate = [](Menu* self)
        {
            if ((Input::justPressed("ESC") || Input::justPressed("P")) && devMenu.getShown() && !devMenu.getShownFirstFrame())
            {
                mouseLock = true;
                glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

                devMenu.hide();
            }
        };
        Button backToGameButton;
        CurveGraph testGraph;
        Slider testSlider;
        backToGameButton.init(50, 50, 150, 30, uiColor, "back to game", uiTextColor);
        backToGameButton.onLeftMouseDown = [](Button* self)
        {
            mouseLock = true;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

            devMenu.hide();
        };
        testGraph.init(currentWindowWidth - 350, currentWindowHeight - 350, 300, 300, uiColor, uiTextColor);
        testSlider.init(currentWindowWidth - 300, 65, 200, 0, 100, uiColor, 0);
        devMenu.add(std::make_unique<Button>(backToGameButton));
        devMenu.add(std::make_unique<CurveGraph>(testGraph));
        devMenu.add(std::make_unique<Slider>(testSlider));
    }
}

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



    initMenus();
}

void UI::update()
{
    if (Input::justPressed("ESC") && openingMenuGroup == NONE)
    {
        pauseMenu.show();
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    if (Input::justPressed("P") && openingMenuGroup == NONE)
    {
        devMenu.show();
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    pauseMenu.update();
    settingsMenu.update();
    devMenu.update();
}

void UI::drawMenus()
{
    pauseMenu.draw();
    settingsMenu.draw();
    devMenu.draw();
}

void UI::drawCrosshair()
{
    crosshairShader.useProgram();
    crosshairShader.setTextureUniform("viewTexture", Graphics::getViewFrameBuffer().getTexture(), 0);
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