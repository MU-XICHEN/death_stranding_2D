#include <string>
#include "layers/DebugLayer.h"
#include "world/World.h"

class FPSBox : public Entity
{
public:
    ~FPSBox() = default;

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
    }

private:
    int count_ = 0;
    float elapsedTime_ = 0.0;
    std::string fpsText_ = "FPS 0";
};

DebugLayer::DebugLayer()
{
    pushEntity(new FPSBox());
}
