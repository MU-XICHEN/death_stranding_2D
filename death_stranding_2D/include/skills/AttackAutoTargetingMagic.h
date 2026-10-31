#pragma once

#include "skills/Attack.h"
#include "structure/DoublyLinkedList.h"

class AttackAutoTargetingMagicImpl;

/**
 * Required properties:
 */

class AttackAutoTargetingMagic : public Attack
{
    friend class AttackAutoTargetingMagicImpl;

public:
    AttackAutoTargetingMagic(int key);

    virtual ~AttackAutoTargetingMagic();

    virtual void tryHit(World &world, Layer &layer) override;
    virtual void render(World &world, Layer &layer) override;
    virtual void tryBegin(World &world, Layer &layer) override;
    virtual void end() override;

private:
    bool triggerOnce_ = false;

    AttackAutoTargetingMagicImpl *impl_;
    GEB::Structures::DoublyLinkedList<ItemEntity *> itemsWithMaxHealth_;
};