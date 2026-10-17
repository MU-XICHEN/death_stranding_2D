#pragma once

#include "Entity.h"
#include "Layer.h"
#include "Hero.h"
#include "Intro.h"
#include "Menu.h"
#include "Viewport.h"
#include "Tally.h"

class IntroStage : public Stage
{
public:
    IntroStage()
    {
        Intro *intro = new Intro(0, 0, 50, 50);
        pushEntity(intro);
    };
    ~IntroStage() = default;
};

class MenuStage : public Stage
{
public:
    MenuStage()
    {
        Menu *menu = new Menu(0, 0, 50, 50);
        pushEntity(menu);
    };
    ~MenuStage() = default;
};

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

class TallyStage : public Stage
{
public:
    TallyStage()
    {
        Tally *tally = new Tally(0, 0, 50, 50);
        pushEntity(tally);
    };
    ~TallyStage() = default;
};

const StageCreator mainLayerStageCreator[] = {
    []() -> Stage *
    { return new IntroStage(); },
    []() -> Stage *
    { return new MenuStage(); },
    []() -> Stage *
    { return new PlayStage(); },
    []() -> Stage *
    { return new TallyStage(); }};

class MainLayer : public Layer
{
public:
    MainLayer() : Layer(mainLayerStageCreator) {};
};
