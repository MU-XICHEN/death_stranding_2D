#pragma once

// function as viewport for the game world
// render viewport while updating the viewport
class Viewport
{
public:
    Viewport(float x, float y, int width, int height)
        : x_(x), y_(y), width_(width), height_(height) {}
    void setPosition(float x, float y)
    {
        x_ = x;
        y_ = y;
    }
    float x() const { return x_; }
    float y() const { return y_; }
    int width() const { return width_; }
    int height() const { return height_; }

private:
    float x_ = 0;
    float y_ = 0;

    int width_;
    int height_;
};