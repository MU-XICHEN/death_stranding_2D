#include <iostream> // todo: remove

#include "world/World.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "layers/MainLayer.h"

World::World()
    : canvas_(),
      gameInfo_(),
      layerList_()
{
    // initialize canvas
    canvas_.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Tiles");

    // Main Layer which handle main game process: Intro - Menu - Gameplay - Tally - Then, again!
    initLayers();

    gameLoop();
}

World::~World()
{
    for (int i = 0; i < layerList_.size(); ++i)
    {
        if (layerList_[i] == nullptr)
            continue;
        delete layerList_[i];
        layerList_[i] = nullptr;
    }
}

void World::initLayers()
{
    // Game main layer
    layerList_.pushBack(new MainLayer());
}

void World::refreshTimer()
{
    if (canvas_.consumeMoveResizeFinished())
    {
        // Discard time spent in the native move/resize event.
        timer_.reset();
        gameInfo_.updateDt(0.0f);
        return;
    }

    float dt = timer_.dt();
    gameInfo_.updateDt(dt);

    if (dt > 0.0f)
        std::cout << "FPS: " << 1.0f / dt << std::endl;
}

void World::frameStart()
{
    refreshTimer();

    // Check whether window is alive.
    if (canvas_.keyPressed(VK_ESCAPE) || canvas_.checkCloseRequested())
        running_ = false;

    // Clear the window for the next frame rendering
    canvas_.clear();
}

void World::frameDraw()
{
    // render layers from bottom to top
    // which means layers pushed in last will be the last to render
    if (layerList_.size() == 0)
        return;

    for (int i = 0; i < layerList_.size(); ++i)
    {
        auto *layer = layerList_[i];

        auto &stage = layer->currentStage();
        // update states of nodes in current Layer
        stage.updateEntities(*this, *layer);
        // render this layer
        stage.renderEntities(*this, *layer);
    }

    // Events like mouse/keyboard are saved in present() by pumpLoop().
    canvas_.present();
}

bool World::frameFinished()
{
    bool result = !running_;
    // Manage layers/stages here.
    if (layerList_.size() == 0)
        return result;

    // Handle Stages
    for (int i = layerList_.size() - 1; i >= 0; --i)
    {
        auto *layer = layerList_[i];

        layer->checkGoToNextStage();
    }

    // todo: handle Layers

    return result;
}

void World::gameLoop()
{

    timer_.reset();

    while (running_)
    {
        frameStart();

        frameDraw();

        bool endLoop = frameFinished();
        if (endLoop)
            break; // Exit the loop if running is false
    }
}
