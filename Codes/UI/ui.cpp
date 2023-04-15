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

#include "../ChunkLoader/chunkLoader.h"

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
Menu UI::devChunkLoaderMenu;

void UI::initMenus()
{
    float marginLeft = 50;
    float marginLeft2 = 250;
    float marginTop = 50;
    float lineHeight = 40;

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
        backToGameButton.init(marginLeft, marginTop, 150, 30, uiColor, "back to game", uiTextColor);
        backToGameButton.onLeftMouseDown = [](Button* self)
        {
            mouseLock = true;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

            pauseMenu.hide();
        };
        pauseMenu.add(std::make_unique<Button>(backToGameButton));

        Button settingsButton;
        settingsButton.init(marginLeft, marginTop+lineHeight, 150, 30, uiColor, "settings", uiTextColor);
        settingsButton.onLeftMouseDown = [](Button* self)
        {
            pauseMenu.hide();
            settingsMenu.show();
        };
        pauseMenu.add(std::make_unique<Button>(settingsButton));

        Button saveAndQuitButton;
        saveAndQuitButton.init(marginLeft, marginTop+lineHeight*2, 150, 30, uiColor, "save and quit", uiTextColor);
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
        backButton.init(marginLeft, marginTop, 150, 30, uiColor, "back", uiTextColor);
        backButton.onLeftMouseDown = [](Button* self){
            settingsMenu.hide();
            pauseMenu.show();
        };
        settingsMenu.add(std::make_unique<Button>(backButton));

        Textbox toggleWireframeModeTextbox;
        toggleWireframeModeTextbox.init(marginLeft+7, marginTop+lineHeight+15, "wireframe mode", uiColor, false, true);
        settingsMenu.add(std::make_unique<Textbox>(toggleWireframeModeTextbox));

        ToggleButton toggleWireframeModeButton;
        toggleWireframeModeButton.init(marginLeft2, marginTop+lineHeight, 50, 30, uiColor, {"off", "on"}, uiTextColor);
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
        backToGameButton.init(marginLeft, marginTop, 150, 30, uiColor, "back to game", uiTextColor);
        backToGameButton.onLeftMouseDown = [](Button* self)
        {
            mouseLock = true;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

            devMenu.hide();
        };
        devMenu.add(std::make_unique<Button>(backToGameButton));

        Button chunkLoaderMenuButton;
        chunkLoaderMenuButton.init(marginLeft, marginTop+lineHeight, 150, 30, uiColor, "chunk loader", uiTextColor);
        chunkLoaderMenuButton.onLeftMouseDown = [](Button* self)
        {
            devMenu.hide();
            devChunkLoaderMenu.show();
        };
        devMenu.add(std::make_unique<Button>(chunkLoaderMenuButton));
    }

    {
        devChunkLoaderMenu.init(DEV);
        devChunkLoaderMenu.onUpdate = [](Menu* self)
        {
            if (Input::justPressed("ESC") && self->getShown())
            {
                devChunkLoaderMenu.hide();
                devMenu.show();
            }
            else if (Input::justPressed("P") && self->getShown())
            {
                devChunkLoaderMenu.hide();
            }
        };

        Button backButton;
        backButton.init(marginLeft, marginTop, 150, 30, uiColor, "back", uiTextColor);
        backButton.onLeftMouseDown = [](Button* self){
            devChunkLoaderMenu.hide();
            devMenu.show();
        };
        devChunkLoaderMenu.add(std::make_unique<Button>(backButton));

        Textbox terrainHeightNoiseText;
        terrainHeightNoiseText.init(marginLeft+7, marginTop+lineHeight+15, "terrain height noise", uiColor, false, true);
        devChunkLoaderMenu.add(std::make_unique<Textbox>(terrainHeightNoiseText));

        Textbox terrainHeightNoise_OctaveText;
        terrainHeightNoise_OctaveText.init(marginLeft+7, marginTop+lineHeight*2+15, "octave", uiColor, false, true);
        devChunkLoaderMenu.add(std::make_unique<Textbox>(terrainHeightNoise_OctaveText));

        Slider terrainHeightNoise_OctaveSlider;
        terrainHeightNoise_OctaveSlider.init(marginLeft2, marginTop+lineHeight*2+17, 100, 0, 4, uiColor, 0);
        terrainHeightNoise_OctaveSlider.onValueUpdate = [](Slider* self)
        {
            ChunkLoader::uiValueUpdate_terrainHeightNoise_Octave.setValue(self->getCurrentValue());
        };
        devChunkLoaderMenu.add(std::make_unique<Slider>(terrainHeightNoise_OctaveSlider));

        Textbox terrainHeightNoise_FrequencyText;
        terrainHeightNoise_FrequencyText.init(marginLeft+7, marginTop+lineHeight*3+15, "frequency", uiColor, false, true);
        devChunkLoaderMenu.add(std::make_unique<Textbox>(terrainHeightNoise_FrequencyText));

        Slider terrainHeightNoise_FrequencySlider;
        terrainHeightNoise_FrequencySlider.init(marginLeft2, marginTop+lineHeight*3+17, 300, 0, 1, uiColor, 5);
        terrainHeightNoise_FrequencySlider.onValueUpdate = [](Slider* self)
        {
            ChunkLoader::uiValueUpdate_terrainHeightNoise_Frequency.setValue(self->getCurrentValue());
        };
        devChunkLoaderMenu.add(std::make_unique<Slider>(terrainHeightNoise_FrequencySlider));

        Textbox maxTerrainHeightText;
        maxTerrainHeightText.init(marginLeft+7, marginTop+lineHeight*4+15, "max terrain height", uiColor, false, true);
        devChunkLoaderMenu.add(std::make_unique<Textbox>(maxTerrainHeightText));

        Slider maxTerrainHeightSlider;
        maxTerrainHeightSlider.init(marginLeft2, marginTop+lineHeight*4+17, 300, 16, 100, uiColor, 0);
        maxTerrainHeightSlider.onValueUpdate = [](Slider* self)
        {
            ChunkLoader::uiValueUpdate_maxTerrainHeight.setValue(self->getCurrentValue());
        };
        devChunkLoaderMenu.add(std::make_unique<Slider>(maxTerrainHeightSlider));
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
    devChunkLoaderMenu.update();
}

void UI::drawMenus()
{
    pauseMenu.draw();
    settingsMenu.draw();
    devMenu.draw();
    devChunkLoaderMenu.draw();
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