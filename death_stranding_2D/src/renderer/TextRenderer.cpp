#include "renderer/TextRenderer.h"
#include "core/GameBase.h"
#include "world/World.h"

const char *TEXT_IMAGE_PATHS[TEXT_SIZE_COUNT] = {
    "assets/font/font_9x18.png",
    "assets/font/font_12x24.png",
    "assets/font/font_18x36.png"};

const int TEXT_WIDTH[TEXT_SIZE_COUNT] = {9, 12, 18};
const int TEXT_HEIGHT[TEXT_SIZE_COUNT] = {18, 24, 36};
const int CHAR_COUNT_ROW = 16;

void TextRenderer::loadImages()
{
    for (int i = 0; i < TEXT_SIZE_COUNT; ++i)
    {
        GEB::Image *image = new GEB::Image();
        if (!image->load(TEXT_IMAGE_PATHS[i]))
        {
            delete image;

            images[i] = nullptr;
            continue;
        }

        images[i] = image;
    }
}

bool TextRenderer::checkImagesValid() const
{
    for (int i = 0; i < TEXT_SIZE_COUNT; ++i)
    {
        if (!images[i])
            return false;
    }
    return true;
}

void TextRenderer::drawCharacter(GEB::Window &canvas,
                                 GEB::TextSize size,
                                 unsigned int x,
                                 unsigned int y,
                                 unsigned int row,
                                 unsigned int col) const
{
    const int imageIndex = static_cast<int>(size);
    const int fontWidth = TEXT_WIDTH[imageIndex];
    const int fontHeight = TEXT_HEIGHT[imageIndex];
    const int charXInImage = col * TEXT_WIDTH[imageIndex];
    const int charYInImage = row * TEXT_HEIGHT[imageIndex];

    const int validWidth = canvas.getWidth();
    const int validHeight = canvas.getHeight();

    const GEB::Image *fontImage = images[imageIndex];

    if (!fontImage)
        return;

    for (int i = 0; i < fontWidth; i++)
    {
        for (int j = 0; j < fontHeight; j++)
        {
            const int targetX = x + i;
            const int targetY = y + j;
            if (targetX >= 0 && targetX < validWidth && targetY >= 0 && targetY < validHeight)
            {
                if (fontImage->alphaAt(charXInImage + i, charYInImage + j))
                    canvas.draw(targetX, targetY, fontImage->at(charXInImage + i, charYInImage + j));
            }
        }
    }
}

void TextRenderer::drawString(GEB::Window &canvas,
                              const char *text,
                              int x,
                              int y,
                              GEB::TextSize size) const
{
    if (!checkImagesValid())
        return;

    if (!text || !*text)
        return;

    const int ratio = static_cast<int>(size) + 1;

    char c = *text;
    while (c != '\0')
    {
        // Determine the position of the character in the font image
        int row = 0, col = 0;
        // 0 - 9: (1, 0) ~ (1, 9)
        if (c >= '0' && c <= '9')
        {
            row = 1;
            col = c - '0';
        }
        // A - Z: (2, 1) ~ (3, 10)
        else if (c >= 'A' && c <= 'Z')
        {
            row = 2 + (c - 'A' + 1) / CHAR_COUNT_ROW;
            col = (c - 'A' + 1) % CHAR_COUNT_ROW;
        }
        // a - z: (4, 1) ~ (5, 10)
        else if (c >= 'a' && c <= 'z')
        {
            row = 4 + (c - 'a' + 1) / CHAR_COUNT_ROW;
            col = (c - 'a' + 1) % CHAR_COUNT_ROW;
        }
        else if (c >= ' ' && c <= '~')
        {
            const int charIndex = c - ' ';
            row = charIndex / CHAR_COUNT_ROW;
            col = charIndex % CHAR_COUNT_ROW;
        }
        else
        {
            c = *++text;
            continue;
        }

        if (c != ' ')
            drawCharacter(canvas, size, x, y, row, col);

        x += (TEXT_WIDTH[static_cast<int>(size)]);
        // Move to the next character
        c = *++text;
    }
}
