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
Menu UI::menu_dev_noises_terrainShape;
Menu UI::menu_dev_noises_terrainHeight;

void UI::initMenus()
{
    float margin = 50;
    float margin2 = 250;
    float lineHeight = 40;

    float buttonw = 150;
    float buttonh= 30;

    float textMarginTop = 2;
    float textMarginLeft = 7;

    float sliderMarginTop = 18;

    float curveGraphWH = 250;

    {
        menu_pause.init(MenuGroup::PAUSE);
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
        menu_pause_settings.init(MenuGroup::PAUSE);
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
        text_toggleWireframeMode.init(margin+textMarginLeft, margin+lineHeight*2+textMarginTop, "wireframe mode", uiColor);
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

        Textbox text_loadDistance;
        text_loadDistance.init(margin+textMarginLeft, margin+lineHeight*3+textMarginTop, "load distance", uiColor);
        menu_pause_settings.add(std::make_unique<Textbox>(text_loadDistance));

        Slider slider_loadDistance;
        slider_loadDistance.init(margin2, margin+lineHeight*3+sliderMarginTop, 100, 1, 10, uiColor, 0);
        slider_loadDistance.onLeftMouseUp = [](Slider* self)
        {
            ChunkLoader::setLoadDistance((int)round(self->getValue()));
        };
        menu_pause_settings.add(std::make_unique<Slider>(slider_loadDistance));

        Textbox text_fov;
        text_fov.init(margin+textMarginLeft, margin+lineHeight*4+textMarginTop, "FOV", uiColor);
        menu_pause_settings.add(std::make_unique<Textbox>(text_fov));

        Slider slider_fov;
        slider_fov.init(margin2, margin+lineHeight*4+sliderMarginTop, 200, 45, 120, uiColor, 0);
        slider_fov.onLeftMouseHold = [](Slider* self)
        {
            fov = self->getValue();
        };
        menu_pause_settings.add(std::make_unique<Slider>(slider_fov));
    }
    
    {
        menu_dev.init(MenuGroup::DEV);
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
        menu_dev_noises.init(MenuGroup::DEV);
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

        Button button_terrainShape;
        button_terrainShape.init(margin, margin+lineHeight*2, buttonw, buttonh, uiColor, "terrain shape", uiTextColor);
        button_terrainShape.onLeftMouseDown = [](Button* self)
        {
            menu_dev_noises.hide();
            menu_dev_noises_terrainShape.show();
        };
        menu_dev_noises.add(std::make_unique<Button>(button_terrainShape));

        Button button_terrainHeight;
        button_terrainHeight.init(margin, margin+lineHeight*3, buttonw, buttonh, uiColor, "terrain height", uiTextColor);
        button_terrainHeight.onLeftMouseDown = [](Button* self)
        {
            menu_dev_noises.hide();
            menu_dev_noises_terrainHeight.show();
        };
        menu_dev_noises.add(std::make_unique<Button>(button_terrainHeight));
    }

    {
        menu_dev_noises_terrainShape.init(MenuGroup::DEV);
        menu_dev_noises_terrainShape.onUpdate = [](Menu* self)
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
        text_title.init(margin, margin, "DEV > NOISES > TERRAIN SHAPE", uiColor);
        menu_dev_noises_terrainShape.add(std::make_unique<Textbox>(text_title));

        Button button_back;
        button_back.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back", uiTextColor);
        button_back.onLeftMouseDown = [](Button* self){
            menu_dev_noises_terrainShape.hide();
            menu_dev_noises.show();
        };
        menu_dev_noises_terrainShape.add(std::make_unique<Button>(button_back));

        CurveGraph curveGraph;
        curveGraph.init(currentWindowWidth-margin-curveGraphWH, currentWindowHeight-margin-curveGraphWH,
                        curveGraphWH, curveGraphWH, uiColor, uiTextColor);
        curveGraph.onLeftMouseUp = [](CurveGraph* self)
        {
            Terrain::terrain_curveMap.setPoints(self->getPoints());

            printf("------------\n");
            std::vector<Vec2> points = self->getPoints();
            for (int i = 0; i < points.size(); i++)
            {
                printf("Vec2(%f, %f),\n", points[i].x, points[i].y);
            }

            ChunkLoader::requestUnloadAllChunks();
        };
        menu_dev_noises_terrainShape.add(std::make_unique<CurveGraph>(curveGraph));

        Textbox text_caves;
        text_caves.init(currentWindowWidth-margin-curveGraphWH, currentWindowHeight-margin-curveGraphWH-28,
                                "caves +");
        menu_dev_noises_terrainShape.add(std::make_unique<Textbox>(text_caves));

        Textbox text_terrainHeight;
        text_terrainHeight.init(currentWindowWidth-margin-120, currentWindowHeight-margin,
                                "terrain height +");
        menu_dev_noises_terrainShape.add(std::make_unique<Textbox>(text_terrainHeight));
    }

    {
        menu_dev_noises_terrainHeight.init(MenuGroup::DEV);
        menu_dev_noises_terrainHeight.onUpdate = [](Menu* self)
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
        text_title.init(margin, margin, "DEV > NOISES > TERRAIN HEIGHT", uiColor);
        menu_dev_noises_terrainHeight.add(std::make_unique<Textbox>(text_title));

        Button button_back;
        button_back.init(margin, margin+lineHeight, buttonw, buttonh, uiColor, "back", uiTextColor);
        button_back.onLeftMouseDown = [](Button* self){
            menu_dev_noises_terrainHeight.hide();
            menu_dev_noises.show();
        };
        menu_dev_noises_terrainHeight.add(std::make_unique<Button>(button_back));

        Textbox text_maxHeight;
        text_maxHeight.init(margin+textMarginLeft, margin+lineHeight*2+textMarginTop, "max height", uiColor);
        menu_dev_noises_terrainHeight.add(std::make_unique<Textbox>(text_maxHeight));

        Slider slider_maxHeight;
        slider_maxHeight.init(margin2, margin+lineHeight*2+sliderMarginTop, 150, 40, 150, uiColor, 0);
        slider_maxHeight.onLeftMouseUp = [](Slider* self)
        {
            Terrain::terrainHeight_max = (int)round(self->getValue());
            ChunkLoader::requestUnloadAllChunks();
        };
        menu_dev_noises_terrainHeight.add(std::make_unique<Slider>(slider_maxHeight));

        CurveGraph curveGraph_bigMountains;
        curveGraph_bigMountains.init(currentWindowWidth-margin-(curveGraphWH-70), currentWindowHeight-margin-(curveGraphWH-70),
                        curveGraphWH-70, curveGraphWH-70, uiColor, uiTextColor);
        curveGraph_bigMountains.onLeftMouseUp = [](CurveGraph* self)
        {
            Terrain::bigMountains_curveMap.setPoints(self->getPoints());

            printf("------------\n");
            std::vector<Vec2> points = self->getPoints();
            for (int i = 0; i < points.size(); i++)
            {
                printf("Vec2(%f, %f),\n", points[i].x, points[i].y);
            }

            ChunkLoader::requestUnloadAllChunks();
        };
        menu_dev_noises_terrainHeight.add(std::make_unique<CurveGraph>(curveGraph_bigMountains));

        Textbox text_bigMountains;
        text_bigMountains.init(currentWindowWidth-margin-(curveGraphWH-70), currentWindowHeight-margin-(curveGraphWH-70)-25,
                                "big mountains");
        menu_dev_noises_terrainHeight.add(std::make_unique<Textbox>(text_bigMountains));

        CurveGraph curveGraph_smallMountains;
        curveGraph_smallMountains.init(currentWindowWidth-margin-(curveGraphWH-70), currentWindowHeight-margin-(curveGraphWH-70)*2-50,
                        curveGraphWH-70, curveGraphWH-70, uiColor, uiTextColor);
        curveGraph_smallMountains.onLeftMouseUp = [](CurveGraph* self)
        {
            Terrain::smallMountains_curveMap.setPoints(self->getPoints());

            printf("------------\n");
            std::vector<Vec2> points = self->getPoints();
            for (int i = 0; i < points.size(); i++)
            {
                printf("Vec2(%f, %f),\n", points[i].x, points[i].y);
            }

            ChunkLoader::requestUnloadAllChunks();
        };
        menu_dev_noises_terrainHeight.add(std::make_unique<CurveGraph>(curveGraph_smallMountains));

        Textbox text_smallMountains;
        text_smallMountains.init(currentWindowWidth-margin-(curveGraphWH-70), currentWindowHeight-margin-(curveGraphWH-70)*2-50-25,
                                "small mountains");
        menu_dev_noises_terrainHeight.add(std::make_unique<Textbox>(text_smallMountains));
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
    crosshairShader.setUniform("currentWindowSize", Vec2((float)currentWindowWidth, (float)currentWindowHeight));



    blockSelectionShader.init("./Shaders/UI/blockSelection");

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
    blockSelectionMesh.set3d(blockSelectionVerticies, blockSelectionIndicies, true);



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
    rectShader.setUniform("windowSize", Vec2((float)currentWindowWidth, (float)currentWindowHeight));



    initMenus();
}

void UI::update()
{
    if (Input::justPressed("ESC") && openingMenuGroup == MenuGroup::NONE)
    {
        menu_pause.show();
        mouseLock = false;
        glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
        glfwSetCursorPos(glfwWindow, currentWindowWidth/2, currentWindowHeight/2);
    }

    if (Input::justPressed("P") && openingMenuGroup == MenuGroup::NONE)
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
    menu_dev_noises_terrainShape.update();
    menu_dev_noises_terrainHeight.update();
}

void UI::drawMenus()
{
    menu_pause.draw();
    menu_pause_settings.draw();
    menu_dev.draw();
    menu_dev_noises.draw();
    menu_dev_noises_terrainShape.draw();
    menu_dev_noises_terrainHeight.draw();
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
    blockSelectionShader.setUniform("projectionMat", Graphics::getViewProjectionMat());
    blockSelectionShader.setUniform("viewMat", Graphics::getViewViewMat());
    blockSelectionShader.setUniform("modelMat", blockSelectionModelMat);
    blockSelectionMesh.draw();
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
    rectShader.setUniform("rectPos", Vec2(x1, y1));
    rectShader.setUniform("rectSize", Vec2(x2 - x1, y2 - y1));
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