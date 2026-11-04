#include "skills/AttackRemoteMulti.h"
#include "world/World.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"

// typedef GEB::Structures::DoublyLinkedList<AttackRemote>::ListNode<AttackRemote> *ListNodeP;

typedef GEB::Structures::DoublyLinkedList<AttackRemote *>::ListNode<AttackRemote *> *ListNodeP;

AttackRemoteMulti::~AttackRemoteMulti()
{
    end();
}

void AttackRemoteMulti::end()
{
    auto *head = bullets_.front();
    while (head != nullptr)
    {
        auto *tmp = head;
        head = head->next;
        if (tmp->data)
            delete tmp->data;
    }
    bullets_.clear();
    shotElapsedTime_ = std::numeric_limits<float>::max();
}

void AttackRemoteMulti::tryHit(World &world, Layer &layer)
{
    // check valid attack
    auto *head = bullets_.front();
    while (head != nullptr)
    {
        ListNodeP current = head;
        head = head->next;
        auto *atk = current->data;

        if (atk && atk->isActive())
            atk->tryHit(world, layer);

        if (!atk || !atk->isActive())
        {
            delete atk;
            bullets_.removeNode(current);
        }
    }
}
// add bullet
void AttackRemoteMulti::tryBegin(World &world, Layer &layer)
{
    float dt = world.gameInfo().dt();
    shotElapsedTime_ += dt;

    if (!owner_)
        return;
    if (shotElapsedTime_ < timeInterval_)
        return;
    if (triggerKey_ < 0 || !world.canvas().keyPressed(triggerKey_))
        return;

    AttackRemote *atkRmt = new AttackRemote(triggerKey_);
    if (atkRmt)
    {
        atkRmt->setOwner(owner_);
        atkRmt->tryBegin(world, layer);
        bullets_.pushBack(atkRmt);
    }

    shotElapsedTime_ = 0;
}

void AttackRemoteMulti::render(World &world, Layer &layer)
{
    auto *head = bullets_.front();
    while (head != nullptr)
    {
        if (head->data)
            head->data->render(world, layer);
        head = head->next;
    }
}
