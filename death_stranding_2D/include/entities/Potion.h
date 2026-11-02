#pragma once

#include "Entity.h"
#include "ItemEntity.h"
#include "skills/State.h"
class World;
class Layer;

class Potion : public ItemEntity
{
public:
    using ItemEntity::ItemEntity;
    virtual void update(World &, Layer &) override;

    const State &effectState() const { return effectState_; }

protected:
    State effectState_;

private:
    virtual void tryApplyTo(ItemEntity *item);
};