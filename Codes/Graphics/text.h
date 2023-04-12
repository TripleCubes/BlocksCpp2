#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
#include <string>

#include "shader.h"
#include "mesh.h"
#include "../Types/rect.h"
#include "../Types/color.h"

class Text
{
    private:
        struct Character
        {
            unsigned int textureId;
            float w = 0;
            float h = 0;
            float bearingX = 0;
            float bearingY = 0;
            float advance = 0;
            float lineHeight = 0;
        };

        static FT_Library library;
        static FT_Face face;

        static std::vector<Character> characters;
        static int firstCharacterCode;

        static Shader textShader;
        static Mesh textMesh;

        static Character drawCharacter(float x, float y, char characterCode, Color color = Color(1.0, 1.0, 1.0, 1.0));

    public:
        static void init();

        static Rect getTextBoxSize(std::string text);
        static Rect drawTextBox(float x, float y, std::string text, Color color = Color(1.0, 1.0, 1.0, 1.0));
        static Rect drawTextBoxCentered(float x, float y, std::string text, Color color = Color(1.0, 1.0, 1.0, 1.0));
        static Rect drawTextBoxCenteredVertically(float x, float y, std::string text, Color color = Color(1.0, 1.0, 1.0, 1.0));
        
        static void release();
};

#endif