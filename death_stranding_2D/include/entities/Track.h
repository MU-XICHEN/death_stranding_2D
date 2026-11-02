#pragma once

#include "ItemEntity.h"

class World;
class Layer;

class Track : public ItemEntity
{
public:
    using ItemEntity::ItemEntity;

    virtual void update(World &world, Layer &layer) override;
    virtual void tryTriggerAttacks(World &world, Layer &layer) override;
};