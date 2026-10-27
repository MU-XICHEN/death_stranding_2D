#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"

static const int TEXT_SIZE_COUNT = 3;

class World;

class TextRenderer
{
public:
    TextRenderer() = default;

    virtual ~TextRenderer() = default;

    TextRenderer(const TextRenderer &) = delete;
    TextRenderer &operator=(const TextRenderer &) = delete;

    void drawString(World &world, const char *text, int x, int y, GEB::TextSize size) const;

private:
    void drawCharacter(World &world,
                       GEB::TextSize size,
                       unsigned int x,
                       unsigned int y,
                       unsigned int row,
                       unsigned int col) const;
};