#pragma once

#include "core/GameBase.h"
#include "structure/Array.h"

// update three sections when adding/removing properties:
// 1. PropertyIndex enum
// 3. Getter and setter methods
class AttackInfo final
{
public:
    // update the property count while adding/removing properties.
    static constexpr int propCount = 6;

    enum PropertyIndex
    {
        DAMAGE = 0,
        DURATION,
        HIT_START_TIME,
        HIT_END_TIME,
        HIT_BOX_WIDTH,
        HIT_BOX_HEIGHT,
    };

    AttackInfo() : properties_(AttackInfo::propCount) {}
    AttackInfo(const GEB::Pair<int, float> *newProp, int size) : properties_(AttackInfo::propCount)
    {
        for (int i = 0; i < size; i++)
        {
            const int index = newProp[i].first;
            if (index >= 0 && index < propCount)
            {
                properties_[index] = newProp[i].second;
            }
        }
    }
    AttackInfo(const AttackInfo &attackInfo) : AttackInfo()
    {
        *this = attackInfo;
    }
    ~AttackInfo() = default;

    // properties getters and setters
    float damage() const { return properties_[DAMAGE]; }
    void setDamage(float damage) { properties_[DAMAGE] = damage; }
    float duration() const { return properties_[DURATION]; }
    void setDuration(float duration) { properties_[DURATION] = duration; }
    float hitStartTime() const { return properties_[HIT_START_TIME]; }
    void setHitStartTime(float hitStartTime) { properties_[HIT_START_TIME] = hitStartTime; }
    float hitEndTime() const { return properties_[HIT_END_TIME]; }
    void setHitEndTime(float hitEndTime) { properties_[HIT_END_TIME] = hitEndTime; }
    // actual size of the hit box
    int hitBoxWidth() const { return static_cast<int>(properties_[HIT_BOX_WIDTH]); }
    void setHitBoxWidth(int hitBoxWidth) { properties_[HIT_BOX_WIDTH] = static_cast<float>(hitBoxWidth); }
    int hitBoxHeight() const { return static_cast<int>(properties_[HIT_BOX_HEIGHT]); }
    void setHitBoxHeight(int hitBoxHeight) { properties_[HIT_BOX_HEIGHT] = static_cast<float>(hitBoxHeight); }

    // member variables getters and setters
    const GEB::Vec2 &originPoint() const { return originPoint_; }
    void setOriginPoint(const GEB::Vec2 &originPoint) { originPoint_ = originPoint; }
    GEB::Direction direction() const { return direction_; }
    void setDirection(GEB::Direction direction) { direction_ = direction; }

    // methods
    inline const GEB::Structures::Array<float> &properties() const { return properties_; }
    AttackInfo &operator=(const AttackInfo &attackInfo)
    {
        const auto &newProperties = attackInfo.properties();
        for (int i = 0; i < propCount; i++)
        {
            properties_[i] = newProperties[i];
        }
        originPoint_ = attackInfo.originPoint_;
        direction_ = attackInfo.direction_;
        return *this;
    }

private:
    GEB::Structures::Array<float> properties_;
    GEB::Vec2 originPoint_ = {0.0f, 0.0f};
    GEB::Direction direction_ = GEB::Direction::None;
};
