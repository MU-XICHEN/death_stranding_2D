#pragma once

class State
{
public:
    // explicit State(int id, int health = 0) : id_(id), health_(health) {};
    State() = default;
    virtual ~State() = default;

private:
    unsigned int id_;
    unsigned int health_;
};