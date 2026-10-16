#include "World.h"
#include "GameBase.h"
#include "Hero.h"
#include "Viewport.h"

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

    // construct layers
    pushMainLayer();

    gameLoop();
}

void World::pushMainLayer()
{
    Layer *mainLayer = new Layer();

    Hero *heroPointer = new Hero(MAP_BORDER_SIZE, MAP_BORDER_SIZE, 20, 20);
    heroPointer->setColor(255, 0, 0);
    mainLayer->pushEntity(heroPointer);

    mainLayer->pushEntity(new Viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    layerList_.push_back(mainLayer);
}

void World::renderLayers()
{
    // Clear the window for the next frame rendering
    canvas_.clear();

    // render layers from bottom to top
    if (layerList_.empty())
        return;

    for (auto *layerNode = layerList_.front(); layerNode != nullptr; layerNode = layerNode->next)
    {

        auto *layer = layerNode->data;
        if (!layer)
            return;

        // update states of nodes in current Layer
        layer->updateEntities(*this);
        // render this layer
        layer->renderEntities(*this);
    }

    // Events like mouse/keyboard are saved in present() by pumpLoop().
    canvas_.present();
}

void World::gameLoop()
{

    timer_.reset();

    while (running_)
    {

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
