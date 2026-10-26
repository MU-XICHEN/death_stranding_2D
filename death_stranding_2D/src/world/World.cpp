#include <iostream> // todo: remove

#include "world/World.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "layers/MainLayer.h"
#include "layers/DebugLayer.h"

World::World()
    : canvas_(),
      gameInfo_(),
      layerList_()
{
    // initialize canvas
    canvas_.create(static_cast<unsigned int>(WINDOW_WIDTH),
                   static_cast<unsigned int>(WINDOW_HEIGHT), "Tiles");

    // initialize text renderer
    textRenderer_.loadImages();

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
    layerList_.pushBack(new DebugLayer());
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

        // update states of nodes in current Layer
        layer->update(*this);
        // render this layer
        layer->render(*this);
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
