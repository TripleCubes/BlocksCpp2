#include "text.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../globals.h"

FT_Library Text::library;
FT_Face Text::face;
std::vector<Text::Character> Text::characters;
int Text::firstCharacterCode = 32;
Shader Text::textShader;
Mesh Text::textMesh;

void Text::init()
{
    textShader.init("./Shaders/Text/text");
    std::vector<float> textVerticies = {
        0, 1,
        1, 1,
        0, 0,
        
        1, 1,
        1, 0,
        0, 0
    };
    textMesh.set2d(textVerticies);



    if (FT_Init_FreeType(&library))
    {
        printf("Couldnt init freetype\n");
    }
    
    if (FT_New_Face(library, "./Fonts/Munro/munro.ttf", 0, &face))
    {
        printf("couldnt load munro.ttf\n");
    }

    FT_Set_Pixel_Sizes(face, 0, 20);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = firstCharacterCode; c <= 126; c++)
    {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                        0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Character character;
        character.textureId = textureId;
        character.w = face->glyph->bitmap.width;
        character.h = face->glyph->bitmap.rows;
        character.bearingX = face->glyph->bitmap_left;
        character.bearingY = face->glyph->bitmap_top;
        character.advance = face->glyph->advance.x/64;
        character.lineHeight = 20;
        characters.push_back(character);
    }
}

Text::Character Text::drawCharacter(float x, float y, char characterCode, Color color)
{
    Character character = characters[characterCode - firstCharacterCode];
    y = currentWindowHeight - y - 20;
    x += character.bearingX;
    y -= character.h - character.bearingY;
    textShader.useProgram();
    textShader.setTextureUniform("textTexture", character.textureId, 0);
    textShader.setUniform("textPos", x, y);
    textShader.setUniform("textSize", character.w, character.h);
    textShader.setUniform("textColor", color);
    textShader.setUniform("windowSize", (float)currentWindowWidth, (float)currentWindowHeight);
    textMesh.draw();

    return character;
}

Rect Text::getTextBoxSize(std::string text)
{
    Rect result;

    for (int i = 0; i < text.size(); i++)
    {
        result.w += characters[text[i] - firstCharacterCode].advance;
    }
    result.h = 20;

    return result;
}

Rect Text::drawTextBox(float x, float y, std::string text, Color color)
{
    int cursorX = x;
    int cursorY = y;
    for (int i = 0; i < text.size(); i++)
    {
        Character drawnCharacter = drawCharacter(cursorX, cursorY, text[i], color);
        cursorX += drawnCharacter.advance;
    }

    return Rect();
}

Rect Text::drawTextBoxCentered(float x, float y, std::string text, Color color)
{
    Rect textBoxSize = getTextBoxSize(text);
    
    return drawTextBox(x - textBoxSize.w/2, y - textBoxSize.h/2 - 2, text, color);
}

Rect Text::drawTextBoxCenteredVertically(float x, float y, std::string text, Color color)
{
    Rect textBoxSize = getTextBoxSize(text);
    
    return drawTextBox(x, y - textBoxSize.h/2 - 3, text, color);
}

void Text::release()
{
    for (int i = 0; i < characters.size(); i++)
    {
        glDeleteTextures(1, &characters[i].textureId);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);
}