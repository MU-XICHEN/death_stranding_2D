#include <iostream> // todo: remove

#include "World.h"
#include "GameBase.h"
#include "Hero.h"
#include "Viewport.h"
#include "MainLayer.h"

GameInfo::GameInfo(Map map, Hero hero, Viewport viewport)
    : map_(map), hero_(hero), viewport_(viewport) {}

World::World()
    : canvas_(),
      gameInfo_{
          Map{
              static_cast<int>(MAP_WIDTH),
              static_cast<int>(MAP_HEIGHT),
              MAP_BORDER_SIZE,
              GEB::Color{255, 255, 255}},
          Hero{MAP_BORDER_SIZE, MAP_BORDER_SIZE, 20, 20},
          Viewport{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT},
      },
      layerList_(),
      timer_()
{
    // initialize canvas
    canvas_.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Tiles");

    // todo: push debug layer, showing information like FPS

    // Main Layer which handle main game process: Intro - Menu - Gameplay - Tally - Then, again!
    pushMainLayer();

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
        auto *temp = headPointer->next;

        headPointer->next = nullptr;
        headPointer->prev = nullptr;

        if (headPointer->data)
            delete headPointer->data;

        delete headPointer;

        headPointer = temp;
    }
}

void World::pushMainLayer()
{
    // Game main layer
    Layer *mainLayer = new MainLayer();

    layerList_.push_back(mainLayer);
}

void World::renderLayers()
{
    // Clear the window for the next frame rendering
    canvas_.clear();

    // render layers from bottom to top
    // which means layers pushed in last will be the first to render
    if (layerList_.empty())
        return;

    for (auto *layerNode = layerList_.back(); layerNode != nullptr; layerNode = layerNode->prev)
    {

        auto *layer = layerNode->data;
        if (!layer || !(layer->currentStage()))
            return;

        // update states of nodes in current Layer
        layer->currentStage()->updateEntities(*this);
        // render this layer
        layer->currentStage()->renderEntities(*this);
    }

    // Events like mouse/keyboard are saved in present() by pumpLoop().
    canvas_.present();
}

void World::refreshTimer()
{
    float dt = timer_.dt();
    gameInfo_.updateDt(dt);

    float FPS = 1 / dt;
    std::cout << "FPS: " << FPS << std::endl;
}

void World::gameLoop()
{

    timer_.reset();

    while (running_)
    {
        refreshTimer();

        // Check whether window is alive.
        if (canvas_.keyPressed(VK_ESCAPE) || canvas_.checkCloseRequested())
            running_ = false;

        // render layers
        renderLayers();

        // Exit here rather than at the top of loop because the events come from last frame, which should be consumed and presented in current frame.
        if (!running_)
            break; // Exit the loop if running is false
    }
}
