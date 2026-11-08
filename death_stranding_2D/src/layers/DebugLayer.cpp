#include <string>
#include "layers/DebugLayer.h"
#include "world/World.h"

class GameInfoBox : public Entity
{
public:
    ~GameInfoBox() = default;

    void update(World &world, Layer &layer) override
    {
        const float dtCurrentFrame = world.gameInfo().dt();
        if (dtCurrentFrame <= 0.0f)
            return;

        elapsedTime_ += dtCurrentFrame;
        ++count_;

        // update FPS text once per second
        if (elapsedTime_ >= 1.0)
        {
            int fps = static_cast<int>(count_ / elapsedTime_);
            std::string fpsStr = std::to_string(fps);
            fpsText_ = "FPS " + fpsStr;

            // reset
            elapsedTime_ = 0.0;
            count_ = 0;
        }
    }

    void render(World &world, Layer &layer) override
    {
        const auto &txtRdr = world.textRenderer();
        txtRdr.drawString(world, fpsText_.c_str(), 20, 20, GEB::TextSizeType::TEXT_SIZE_MEDIUM);
        txtRdr.drawString(world, "W/S: Up/Down", 20, 52, GEB::TextSizeType::TEXT_SIZE_MEDIUM);
        txtRdr.drawString(world, "A/D: Left/Right", 20, 84, GEB::TextSizeType::TEXT_SIZE_MEDIUM);
        txtRdr.drawString(world, "J: Skill", 20, 116, GEB::TextSizeType::TEXT_SIZE_MEDIUM);
        txtRdr.drawString(world, "K: Magic Skill", 20, 148, GEB::TextSizeType::TEXT_SIZE_MEDIUM);
        txtRdr.drawString(world, "L: Speed Up", 20, 180, GEB::TextSizeType::TEXT_SIZE_MEDIUM);
    }

private:
    int count_ = 0;
    float elapsedTime_ = 0.0;
    std::string fpsText_ = "FPS 0";
};

DebugLayer::DebugLayer()
{
    pushEntity(new GameInfoBox());
}
