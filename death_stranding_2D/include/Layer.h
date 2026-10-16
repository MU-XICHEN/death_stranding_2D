#pragma once

#include "Entity.h"
#include "DoublyLinkedList.h"
#include "GameBase.h"
#include "GamesEngineeringBase.h"

class Layer
{

public:
    Layer() = default;
    ~Layer()
    {
        if (entity_list_.empty())
            return;

        auto *headPointer = entity_list_.front();
        while (headPointer)
        {
            // delete list nodes
            auto *temp = headPointer->next;

            headPointer->next = nullptr;
            headPointer->prev = nullptr;

            if (headPointer->data)
                delete headPointer->data;

            delete headPointer;

            headPointer = temp;
        }
    }

    void pushEntity(Entity *entity)
    {
        entity_list_.push_back(entity);
        entityCount_++;
    }

    // Iterate over the list in insertion order, which means the entity added first is updated first.
    void updateEntities(World &world)
    {
        if (entity_list_.empty())
            return;

        auto *head = entity_list_.front();
        while (head)
        {
            auto &entity = head->data;
            entity->update(world);

            head = head->next;
        }
    }

    // Iterate over the list in reverse order, which means the entity pushed last is rendered first.
    void renderEntities(World &world)
    {
        if (entity_list_.empty())
            return;
        auto *tail = entity_list_.back();

        while (tail)
        {
            auto &entity = tail->data;
            entity->render(world);

            tail = tail->prev;
        }
    }

private:
    int entityCount_ = 0;
    // List includes various types of Entity, so pass <Entity *>
    GEB::Structures::DoublylinkedList<Entity *> entity_list_;
};