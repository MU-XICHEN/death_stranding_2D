#include <string>
#include "renderer/SliceRenderer.h"
#include "core/GameBase.h"
#include "world/World.h"

void SliceRenderer::loadSlices(
    World &world,
    const char *dirPath,
    const char *fileName,
    const unsigned int sliceCount)
{
    auto &canvas = world.canvas();
    for (unsigned int i = 0; i < sliceCount; ++i)
    {
        GEB::Image *image = new GEB::Image();
        std::string filePathStr = std::string(dirPath) + "/" + fileName + "_" + std::to_string(i) + ".png";

        if (!image->load(filePathStr))
        {
            delete image;
            continue;
        }

        if (image->width != canvas.getWidth() || image->height != canvas.getHeight())
        {
            delete image;
            continue;
        }

        if (image->channels != 3)
        {
            // The canvas needs RGB pixels for a direct copy.
            delete image;
            continue;
        }

        slices_.pushBack(image);
    }
}

void SliceRenderer::playSlices(World &world)
{
    if (slices_.size() == 0)
        return;

    // Change the image after its duration, then loop back at the end.
    elapsedTime_ += world.gameInfo().dt();
    if (elapsedTime_ >= sliceDuration_)
    {
        currentSliceIndex_ = (currentSliceIndex_ + 1) % slices_.size();
        elapsedTime_ = 0.0f;
    }

    auto &canvas = world.canvas();
    auto *buffer = canvas.getBackBuffer();

    auto *currentSlice = slices_[currentSliceIndex_];

    if (!currentSlice)
        return;

    if (canvas.getWidth() != currentSlice->width || canvas.getHeight() != currentSlice->height)
    {
        // for now, make sure the canvas size matches the slice size
        return;
    }

    const int bytes = currentSlice->width * currentSlice->height * 3;

    std::memcpy(buffer, currentSlice->data, bytes);
}
