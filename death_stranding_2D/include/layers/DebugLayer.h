#pragma once

#include "layers/Layer.h"
#include "stages/Stage.h"
#include "structure/Array.h"
#include "core/GameBase.h"
#include "entities/Entity.h"

class World;

class DebugLayer : public Layer
{
public:
    DebugLayer();
    virtual ~DebugLayer() = default;
};
