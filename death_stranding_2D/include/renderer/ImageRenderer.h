#pragma once

#include "world/World.h"
#include "utils/Utils.h"
#include <algorithm>

// Draw a whole image at screen coordinates, keeping its original size and transparency.
inline void drawImage(World &world, AssetId asset, int left, int top)
{
    const auto *image = world.assetManager().getImage(asset);
    if (!image)
        return;

    auto &canvas = world.canvas();
    auto *buffer = canvas.getBackBuffer();
    int right = std::min(left + static_cast<int>(image->width), static_cast<int>(canvas.getWidth()));
    int bottom = std::min(top + static_cast<int>(image->height), static_cast<int>(canvas.getHeight()));
    for (int y = std::max(0, top); y < bottom; ++y)
        for (int x = std::max(0, left); x < right; ++x)
            GEB::Utils::blendPixel(buffer + (y * canvas.getWidth() + x) * 3,
                image->at(x - left, y - top), image->alphaAt(x - left, y - top));
}
