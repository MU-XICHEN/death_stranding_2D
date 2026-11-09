#pragma once

class KeyState
{
private:
    bool wasDown_ = false;

public:
    bool justPressed(bool isDown)
    {
        // Trigger only when the key changes from released to pressed.
        const bool triggered = isDown && !wasDown_;
        wasDown_ = isDown;
        return triggered;
    }
};
