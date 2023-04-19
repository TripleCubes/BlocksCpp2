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
#include "../ChunkLoader/terrain.h"

#include <memory>

Mesh UI::crosshairMesh;
Shader UI::crosshairShader;
Mesh UI::blockSelectionMesh;
Shader UI::blockSelectionShader;
Mesh UI::rectMesh;
Shader UI::rectShader;

Menu UI::menu_pause;
Menu UI::menu_pause_settings;
Menu UI::menu_dev;
Menu UI::menu_dev_noises;
Menu UI::menu_dev_noises_terrain;

void UI::initMenus()
{
    float margin = 50;
    float margin2 = 250;
    float lineHeight = 40;

    float buttonw = 150;
    float buttonh= 30;

    float textMarginTop = 15;

    float curveGraphWH = 250;

    {
        menu_pause.init(PAUSE);
        menu_pause.onUpdate = [](Menu* self)
        {
            if (Input::justPressed("ESC") && menu_pause.getShown() && !menu_pause.getShownFirstFrame())
            {
                mouseLock = true;
                glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

                menu_pause.hide();
            }
        };

        Textbox text_title;
        text_title.init(margin, margin, "PAUSE", uiColor);
        menu_pause.add(std::make_unique<Textbox>(text_title));

        Button button_backToGame;
        button_backToGame.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back to game", uiTextColor);
        button_backToGame.onLeftMouseDown = [](Button* self)
        {
            mouseLock = true;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

            menu_pause.hide();
        };
        menu_pause.add(std::make_unique<Button>(button_backToGame));

        Button button_settings;
        button_settings.init(margin, margin+lineHeight*2, buttonw, buttonh, uiColor, "settings", uiTextColor);
        button_settings.onLeftMouseDown = [](Button* self)
        {
            menu_pause.hide();
            menu_pause_settings.show();
        };
        menu_pause.add(std::make_unique<Button>(button_settings));

        Button button_saveAndQuit;
        button_saveAndQuit.init(margin, margin+lineHeight*3, buttonw, buttonh, uiColor, "save and quit", uiTextColor);
        menu_pause.add(std::make_unique<Button>(button_saveAndQuit));
    }

    {
        menu_pause_settings.init(PAUSE);
        menu_pause_settings.onUpdate = [](Menu* self)
        {
            if (Input::justPressed("ESC") && menu_pause_settings.getShown())
            {
                menu_pause_settings.hide();
                menu_pause.show();
            }
        };

        Textbox text_title;
        text_title.init(margin, margin, "SETTINGS", uiColor);
        menu_pause_settings.add(std::make_unique<Textbox>(text_title));

        Button button_back;
        button_back.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back", uiTextColor);
        button_back.onLeftMouseDown = [](Button* self){
            menu_pause_settings.hide();
            menu_pause.show();
        };
        menu_pause_settings.add(std::make_unique<Button>(button_back));

        Textbox text_toggleWireframeMode;
        text_toggleWireframeMode.init(margin+7, margin+lineHeight*2+textMarginTop, "wireframe mode", uiColor, false, true);
        menu_pause_settings.add(std::make_unique<Textbox>(text_toggleWireframeMode));

        ToggleButton button_toggleWireframeMode;
        button_toggleWireframeMode.init(margin2, margin+lineHeight*2, 50, buttonh, uiColor, {"off", "on"}, uiTextColor);
        button_toggleWireframeMode.onLeftMouseDown = [](ToggleButton* self){
            if (self->getText() == "on")
            {
                wireframeMode = true;
            }
            else
            {
                wireframeMode = false;
            }
        };
        button_toggleWireframeMode.onShow = [](ToggleButton* self){
            self->setText(wireframeMode ? "on" : "off");
        };
        menu_pause_settings.add(std::make_unique<ToggleButton>(button_toggleWireframeMode));
    }
    
    {
        menu_dev.init(DEV);
        menu_dev.onUpdate = [](Menu* self)
        {
            if ((Input::justPressed("ESC") || Input::justPressed("P")) && menu_dev.getShown() && !menu_dev.getShownFirstFrame())
            {
                mouseLock = true;
                glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

                menu_dev.hide();
            }
        };

        Textbox text_title;
        text_title.init(margin, margin, "DEV", uiColor);
        menu_dev.add(std::make_unique<Textbox>(text_title));

        Button button_backToGame;
        button_backToGame.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back to game", uiTextColor);
        button_backToGame.onLeftMouseDown = [](Button* self)
        {
            mouseLock = true;
            glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

            menu_dev.hide();
        };
        menu_dev.add(std::make_unique<Button>(button_backToGame));

        Button button_noises;
        button_noises.init(margin, margin+lineHeight*2, buttonw, buttonh, uiColor, "noises", uiTextColor);
        button_noises.onLeftMouseDown = [](Button* self)
        {
            menu_dev.hide();
            menu_dev_noises.show();
        };
        menu_dev.add(std::make_unique<Button>(button_noises));
    }

    {
        menu_dev_noises.init(DEV);
        menu_dev_noises.onUpdate = [](Menu* self)
        {
            if (self->getShown() && !self->getShownFirstFrame())
            {
                if (Input::justPressed("ESC"))
                {
                    self->hide();
                    menu_dev.show();
                }
                else if (Input::justPressed("P"))
                {
                    mouseLock = true;
                    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

                    self->hide();
                }
            }
        };

        Textbox text_title;
        text_title.init(margin, margin, "DEV > NOISES", uiColor);
        menu_dev_noises.add(std::make_unique<Textbox>(text_title));

        Button button_back;
        button_back.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back", uiTextColor);
        button_back.onLeftMouseDown = [](Button* self){
            menu_dev_noises.hide();
            menu_dev.show();
        };
        menu_dev_noises.add(std::make_unique<Button>(button_back));

        Button button_terrain;
        button_terrain.init(margin, margin+lineHeight*2, buttonw, buttonh, uiColor, "terrain", uiTextColor);
        button_terrain.onLeftMouseDown = [](Button* self)
        {
            menu_dev_noises.hide();
            menu_dev_noises_terrain.show();
        };
        menu_dev_noises.add(std::make_unique<Button>(button_terrain));
    }

    {
        menu_dev_noises_terrain.init(DEV);
        menu_dev_noises_terrain.onUpdate = [](Menu* self)
        {
            if (self->getShown() && !self->getShownFirstFrame())
            {
                if (Input::justPressed("ESC"))
                {
                    self->hide();
                    menu_dev_noises.show();
                }
                else if (Input::justPressed("P"))
                {
                    mouseLock = true;
                    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
                    
                    self->hide();
                }
            }
        };

        Textbox text_title;
        text_title.init(margin, margin, "DEV > NOISES > TERRAIN", uiColor);
        menu_dev_noises_terrain.add(std::make_unique<Textbox>(text_title));

        Button button_back;
        button_back.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back", uiTextColor);
        button_back.onLeftMouseDown = [](Button* self){
            menu_dev_noises_terrain.hide();
            menu_dev_noises.show();
        };
        menu_dev_noises_terrain.add(std::make_unique<Button>(button_back));

        CurveGraph curveGraph;
        curveGraph.init(currentWindowWidth-margin-curveGraphWH, currentWindowHeight-margin-curveGraphWH,
                        curveGraphWH, curveGraphWH, uiColor, uiTextColor);
        curveGraph.onPointsUpdate = [](CurveGraph* self)
        {
            Terrain::terrain_curveMap.setPoints(self->getPoints());
            ChunkLoader::requestUnloadAllChunks();
        };
        menu_dev_noises_terrain.add(std::make_unique<CurveGraph>(curveGraph));
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
        menu_pause.show();
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    if (Input::justPressed("P") && openingMenuGroup == NONE)
    {
        menu_dev.show();
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    menu_pause.update();
    menu_pause_settings.update();
    menu_dev.update();
    menu_dev_noises.update();
    menu_dev_noises_terrain.update();
}

void UI::drawMenus()
{
    menu_pause.draw();
    menu_pause_settings.draw();
    menu_dev.draw();
    menu_dev_noises.draw();
    menu_dev_noises_terrain.draw();
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