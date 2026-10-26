#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"

static const int TEXT_SIZE_COUNT = 3;

class World;

class TextRenderer
{
public:
    TextRenderer()
    {
        for (int i = 0; i < TEXT_SIZE_COUNT; ++i)
        {
            images[i] = nullptr;
        }
    };

    virtual ~TextRenderer()
    {
        for (int i = 0; i < TEXT_SIZE_COUNT; ++i)
        {
            if (images[i])
            {
                delete images[i];
                images[i] = nullptr;
            }
        }
    };

    void loadImages();

    TextRenderer(const TextRenderer &) = delete;
    TextRenderer &operator=(const TextRenderer &) = delete;

    void drawString(GEB::Window &canvas, const char *text, int x, int y, GEB::TextSize size) const;

private:
    void initialize();

    void drawCharacter(GEB::Window &canvas,
                       GEB::TextSize size,
                       unsigned int x,
                       unsigned int y,
                       unsigned int row,
                       unsigned int col) const;

    bool checkImagesValid() const;

    GEB::Image *images[TEXT_SIZE_COUNT];
};