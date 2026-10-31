#pragma once

#include "core/GameBase.h"
#include "structure/Array.h"

// update three sections when adding/removing properties:
// 1. PropertyIndex enum
// 3. Getter and setter methods
class AttackInfo final
{
public:
    enum PropertyIndex
    {
        Damage = 0,
        Duration,
        HitStartTime,
        HitEndTime,
        HitBoxWidth,
        HitBoxHeight,
        RangedAttackSpeed,
        OriginPointX,
        OriginPointY,
        Direction,
        _END_FLAG_,
    };
    static constexpr int propCount = static_cast<int>(PropertyIndex::_END_FLAG_);

    AttackInfo() : properties_(AttackInfo::propCount) {}
    AttackInfo(const GEB::Pair<int, float> *newProp, int size)
        : properties_(AttackInfo::propCount)
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
    float damage() const { return properties_[Damage]; }
    void setDamage(float damage) { properties_[Damage] = damage; }
    float duration() const { return properties_[Duration]; }
    void setDuration(float duration) { properties_[Duration] = duration; }
    float hitStartTime() const { return properties_[HitStartTime]; }
    void setHitStartTime(float hitStartTime) { properties_[HitStartTime] = hitStartTime; }
    float hitEndTime() const { return properties_[HitEndTime]; }
    void setHitEndTime(float hitEndTime) { properties_[HitEndTime] = hitEndTime; }
    // actual size of the hit box
    int hitBoxWidth() const { return static_cast<int>(properties_[HitBoxWidth]); }
    void setHitBoxWidth(int hitBoxWidth) { properties_[HitBoxWidth] = static_cast<float>(hitBoxWidth); }
    int hitBoxHeight() const { return static_cast<int>(properties_[HitBoxHeight]); }
    void setHitBoxHeight(int hitBoxHeight) { properties_[HitBoxHeight] = static_cast<float>(hitBoxHeight); }
    float rangedAttackSpeed() const { return properties_[RangedAttackSpeed]; }
    void setRangedAttackSpeed(float rangedAttackSpeed) { properties_[RangedAttackSpeed] = rangedAttackSpeed; }
    GEB::Vec2 originPoint() const { return GEB::Vec2{properties_[OriginPointX], properties_[OriginPointY]}; }
    void setOriginPoint(const GEB::Vec2 &originPoint)
    {
        properties_[OriginPointX] = originPoint.x;
        properties_[OriginPointY] = originPoint.y;
    }
    GEB::Direction direction() const { return static_cast<GEB::Direction>(static_cast<int>(properties_[Direction])); }
    void setDirection(GEB::Direction direction) { properties_[Direction] = static_cast<float>(direction); }

    // methods
    inline const GEB::Structures::Array<float> &properties() const { return properties_; }
    AttackInfo &operator=(const AttackInfo &attackInfo)
    {
        const auto &newProperties = attackInfo.properties();
        for (int i = 0; i < propCount; i++)
        {
            properties_[i] = newProperties[i];
        }
        return *this;
    }

private:
    GEB::Structures::Array<float> properties_;
};
