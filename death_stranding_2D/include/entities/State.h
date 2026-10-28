#pragma once

class State
{
public:
    // explicit State(int id, int health = 0) : id_(id), health_(health) {};
    State() = default;
    State(int id, int health = 0) : id_(id), health_(health) {}
    virtual ~State() = default;

    unsigned int getId() const { return id_; }
    unsigned int getHealth() const { return health_; }

private:
    unsigned int id_ = 0;
    unsigned int health_ = 0;
};