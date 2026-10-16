#include "World.h"
#include "Constants.h"
#include "Hero.h"
#include "Viewport.h"

// int checkHeroInWindow(const Entity &hero, const Entity &window)
// {
//     int inHorizontal = window.x() <= hero.x() && hero.x() <= (window.x() + window.width());
//     int inVertical = window.y() <= hero.y() && hero.y() <= (window.y() + window.height());
//     return inHorizontal && inVertical;
// }

GameInfo::GameInfo(Map map, Hero hero, Viewport viewport)
    : map_(map), hero_(hero), viewport_(viewport) {}

World::World()
    : canvas_(),
      gameInfo_{
          Map{
              static_cast<int>(MAP_WIDTH),
              static_cast<int>(MAP_HEIGHT),
              MAP_BORDER_SIZE,
              Color{255, 255, 255}},
          Hero{MAP_BORDER_SIZE, MAP_BORDER_SIZE, 20, 20},
          Viewport{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}}
{
    // initialize canvas
    canvas_.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Tiles");

    // construct layers
    pushMainLayer();

    gameLoop();
}

void World::pushMainLayer()
{
    Layer mainLayer;

    // The postion of viewport relies on hero, so push hero first.
    Hero *heroPointer = new Hero(MAP_BORDER_SIZE, MAP_BORDER_SIZE, 20, 20);
    heroPointer->setColor(255, 0, 0);
    mainLayer.pushEntity(heroPointer);

    mainLayer.pushEntity(new Viewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));

    layerList_.push_back(mainLayer);
}

void World::renderLayers()
{
    // render layers from bottom to top
    if (layerList_.empty())
        return;

    // update states
    for (auto *layerNode = layerList_.back(); layerNode != nullptr; layerNode = layerNode->prev)
        layerNode->data.updateEntities(*this);

    // draw
}

void World::render()
{
    // Clear the window for the next frame rendering
    canvas_.clear();

    // render layers
    renderLayers();

    // Events like mouse/keyboard are saved in present() by pumpLoop().
    canvas_.present();
}

void World::gameLoop()
{

    while (running_)
    {
        // --------------- Event Handling Stage ---------------
        // Handle events from last frame, then change states for current frame.

        // --------------- Draw Stage ---------------
        render();

        // Exit here rather than at the top of loop because the events come from last frame, which should be consumed and presented
        if (!running_)
            break; // Exit the loop if running is false
    }
}
