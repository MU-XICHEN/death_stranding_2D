#pragma once

#include "entities/Entity.h"
#include "stages/Stage.h"
#include "utils/KeyState.h"

class World;
class Layer;

class MenuStage : public Stage
{
public:
    MenuStage() = default;
    ~MenuStage() = default;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

    void renderSelection(World &world, Layer &layer);

    void moveSelection(int direction);

private:
    int selectedIdx_ = 0;
    int maxIdx_ = 2;

    KeyState wKeyState_;
    KeyState sKeyState_;
    KeyState enterKeyState_;
};