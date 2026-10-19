#pragma once

#include "stages/Stage.h"
#include "entities/Hero.h"
#include "entities/Viewport.h"

class PlayStage : public Stage
{
public:
    PlayStage()
    {
        // Hero Entity
        Hero *heroPointer = new Hero(MAP_BORDER_SIZE, MAP_BORDER_SIZE, ROLE_SIZE, ROLE_SIZE);
        heroPointer->setColor(255, 0, 0);
        pushEntity(heroPointer);

        // Viewport Entity
        pushEntity(new Viewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT));
    };
    ~PlayStage() = default;
};
