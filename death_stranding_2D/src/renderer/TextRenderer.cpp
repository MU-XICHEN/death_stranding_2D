#include "renderer/TextRenderer.h"
#include "core/GameBase.h"
#include "world/World.h"
#include "world/AssetManager.h"
#include "utils/Utils.h"

const int TEXT_WIDTH[TEXT_SIZE_COUNT] = {
    GEB::TextSizeW::TEXT_SIZE_W_SMALL,
    GEB::TextSizeW::TEXT_SIZE_W_MEDIUM,
    GEB::TextSizeW::TEXT_SIZE_W_LARGE};
const int TEXT_HEIGHT[TEXT_SIZE_COUNT] = {
    GEB::TextSizeH::TEXT_SIZE_H_SMALL,
    GEB::TextSizeH::TEXT_SIZE_H_MEDIUM,
    GEB::TextSizeH::TEXT_SIZE_H_LARGE};
const int CHAR_COUNT_ROW = 16;

AssetId translateTextSizeToAssetId(GEB::TextSizeType size)
{
    switch (size)
    {
    case GEB::TextSizeType::TEXT_SIZE_SMALL:
        return AssetId::TextSmall;
    case GEB::TextSizeType::TEXT_SIZE_MEDIUM:
        return AssetId::TextMedium;
    case GEB::TextSizeType::TEXT_SIZE_LARGE:
        return AssetId::TextLarge;
    default:
        return AssetId::None;
    }
}

void TextRenderer::drawCharacter(World &world,
                                 GEB::TextSizeType textSize,
                                 unsigned int x,
                                 unsigned int y,
                                 unsigned int row,
                                 unsigned int col,
                                 GEB::Color color) const
{
    auto &canvas = world.canvas();
    auto &assetManager = world.assetManager();
    auto id = translateTextSizeToAssetId(textSize);
    const GEB::Image *fontImage = assetManager.getImage(id);
    if (!fontImage)
        return;

    auto *buffer = canvas.getBackBuffer();
    if (!buffer)
        return;

    const int imageIndex = static_cast<int>(textSize);
    const int fontWidth = TEXT_WIDTH[imageIndex];
    const int fontHeight = TEXT_HEIGHT[imageIndex];
    const int charXInImage = col * TEXT_WIDTH[imageIndex];
    const int charYInImage = row * TEXT_HEIGHT[imageIndex];

    const int validWidth = world.canvas().getWidth();
    const int validHeight = world.canvas().getHeight();

    // Copy the letter pixels with the chosen color and transparency.
    for (int pixelY = 0; pixelY < fontHeight; pixelY++)
    {
        for (int pixelX = 0; pixelX < fontWidth; pixelX++)
        {
            const int targetX = x + pixelX;
            const int targetY = y + pixelY;
            if (targetX < 0 || targetX >= validWidth)
                continue;
            if (targetY < 0 || targetY >= validHeight)
                continue;

            int sourceX = charXInImage + pixelX;
            int sourceY = charYInImage + pixelY;
            auto *pixel = fontImage->at(sourceX, sourceY);
            auto alpha = fontImage->alphaAt(sourceX, sourceY);
            auto *destination = buffer + (targetY * validWidth + targetX) * 3;
            const unsigned char tinted[] = {
                static_cast<unsigned char>(pixel[0] * color.r / 255),
                static_cast<unsigned char>(pixel[1] * color.g / 255),
                static_cast<unsigned char>(pixel[2] * color.b / 255)};
            GEB::Utils::blendPixel(destination, tinted, alpha);
        }
    }
}

void TextRenderer::drawString(World &world,
                              const char *text,
                              int x,
                              int y,
                              GEB::TextSizeType textSize,
                              GEB::Color color) const
{

    auto id = translateTextSizeToAssetId(textSize);
    auto &assetManager = world.assetManager();
    const GEB::Image *fontImage = assetManager.getImage(id);
    if (!fontImage)
        return;

    if (!text || !*text)
        return;

    const int ratio = static_cast<int>(textSize) + 1;

    char c = *text;
    while (c != '\0')
    {
        // Find this letter in the font image.
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
            drawCharacter(world, textSize, x, y, row, col, color);

        x += (TEXT_WIDTH[static_cast<int>(textSize)]);
        // Move to the next character
        c = *++text;
    }
}
