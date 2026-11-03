

#include "Attack.h"
#include "AttackRemote.h"
#include "skills/AttackInfo.h"
#include "structure/DoublyLinkedList.h"

class World;
class Layer;

/**
 * Required properties:
 * All properties for AttachRemote.h, plus:
 */
class AttackRemoteMulti : public Attack
{
public:
    AttackRemoteMulti(int key, float itv)
        : Attack(key),
          timeInterval_(itv)
    {
        // AttackRemoteMulti is always alive.
        active_ = true;
    };
    ~AttackRemoteMulti();
    virtual void tryHit(World &world, Layer &layer) override;
    // add bullet
    virtual void tryBegin(World &world, Layer &layer) override;

    virtual void end() override;
    virtual void render(World &world, Layer &layer) override;

private:
    GEB::Structures::DoublyLinkedList<AttackRemote *> bullets_;
    float timeInterval_ = 0.0f;
    // make sure to trigger at the first time.
    float shotElapsedTime_ = std::numeric_limits<float>::max();
};
