#pragma once

class KeyState
{
private:
    bool wasDown_ = false;

public:
    bool justPressed(bool isDown)
    {
        const bool triggered = isDown && !wasDown_;
        wasDown_ = isDown;
        return triggered;
    }
};