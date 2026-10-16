#pragma once

#include "Entity.h"
#include "DoublyLinkedList.h"
#include "Constants.h"
#include "GamesEngineeringBase.h"

class Layer
{

public:
    Layer() = default;
    ~Layer()
    {
        auto *head = entity_list_.front();
        auto *tail = entity_list_.back();

        if (!entity_list_.empty())
            return;

        while (head)
        {
            auto *entity = head->data;
            if (entity)
                delete entity;
            head = head->next;
        }
    }

    void pushEntity(Entity *entity)
    {
        if (entityCount_ >= MAX_ENTITIES)
            return;

        entity_list_.push_back(entity);
        entityCount_++;
    }

    void updateEntities(World &world)
    {
        auto *head = entity_list_.front();
        auto *tail = entity_list_.back();

        while (head)
        {
            auto &entity = head->data;
            entity->update(world);
        }
    }

private:
    int entityCount_ = 0;
    GamesEngineeringBase::Structures::DoublylinkedList<Entity *> entity_list_;
};