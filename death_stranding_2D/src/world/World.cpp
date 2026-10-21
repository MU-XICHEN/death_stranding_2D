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

    // initialize game info
    gameInfo_.initialize();

    // Main Layer which handle main game process: Intro - Menu - Gameplay - Tally - Then, again!
    initLayers();

    gameLoop();
}

World::~World()
{
    if (layerList_.empty())
        return;

    auto *headPointer = layerList_.front();
    while (headPointer)
    {
        // delete list nodes
        if (headPointer->data)
            delete headPointer->data;

        headPointer = headPointer->next;
    }
}

void World::initLayers()
{
    // Game main layer
    Layer *mainLayer = new MainLayer();

    layerList_.push_back(mainLayer);
}

void World::refreshTimer()
{
    float dt = timer_.dt();
    gameInfo_.updateDt(dt);

    float FPS = 1 / dt;
    std::cout << "FPS: " << FPS << std::endl;
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
    if (layerList_.empty())
        return;

    for (auto *layerNode = layerList_.front(); layerNode != nullptr; layerNode = layerNode->next)
    {

        auto *layer = layerNode->data;
        if (!layer)
        {
            GamesEngineeringBase::Debugger::showAlert();
            return;
        }

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
    if (layerList_.empty())
        return result;

    // Handle Stages
    for (auto *layerNode = layerList_.back(); layerNode != nullptr; layerNode = layerNode->prev)
    {

        auto *layer = layerNode->data;

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
