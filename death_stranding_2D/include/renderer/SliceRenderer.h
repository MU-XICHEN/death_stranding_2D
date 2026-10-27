#pragma once

#include "GamesEngineeringBase.h"
#include "core/GameBase.h"
#include "structure/Array.h"

class World;

class SliceRenderer
{
public:
    SliceRenderer(
        float sliceDuration = 0.1f)
        : sliceDuration_(sliceDuration) {}
    virtual ~SliceRenderer()
    {
        for (int i = 0; i < slices_.size(); ++i)
        {
            if (slices_[i])
                delete slices_[i];
            slices_[i] = nullptr;
        }
    };

    void loadSlices(
        World &world,
        const char *dirPath,
        const char *fileName,
        const unsigned int sliceCount);

    void playSlices(World &world);

private:
    float sliceDuration_ = 0.1f; // each slice duration in seconds
    GEB::Structures::Array<GEB::Image *> slices_;

    int currentSliceIndex_ = 0;
    float elapsedTime_ = 0.0f;
};