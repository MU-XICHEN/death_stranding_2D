#pragma once
#include "structure/Array.h"
#include "core/GameBase.h"

// update three sections when adding/removing properties:
// 1. PropertyIndex enum
// 2. propCount
// 3. Getter and setter methods
class State final
{
public:
    // update the property count while adding/removing properties.
    static constexpr int propCount = 6;

    enum PropertyIndex
    {
        HEALTH = 0,
        SPEED,
        DAMAGE,
        COLLITION_DAMAGE,
        PERCEPTION_RANGE,
        DEFENSE,
    };

    State() : properties_(State::propCount) {};
    ~State() = default;
    // properties
    // health
    float health() const { return properties_[HEALTH]; }
    void setHealth(float health) { properties_[HEALTH] = health; }
    // speed
    float speed() const { return properties_[SPEED]; }
    void setSpeed(float speed) { properties_[SPEED] = speed; }
    // perception range
    float perceptionRange() const { return properties_[PERCEPTION_RANGE]; }
    void setPerceptionRange(float perceptionRange) { properties_[PERCEPTION_RANGE] = perceptionRange; }
    // defense
    float defense() const { return properties_[DEFENSE]; }
    void setDefense(float defense) { properties_[DEFENSE] = defense; }
    // damage
    float damage() const { return properties_[DAMAGE]; }
    void setDamage(float damage) { properties_[DAMAGE] = damage; }
    // collition damage
    float collitionDamage() const { return properties_[COLLITION_DAMAGE]; }
    void setCollitionDamage(float collitionDamage) { properties_[COLLITION_DAMAGE] = collitionDamage; }

    // methods
    inline const GEB::Structures::Array<float> &properties() const { return properties_; };
    void initialize(
        GEB::Pair<int, float> *newProp,
        int size)
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
    State &operator=(const State &state)
    {
        const int size = properties_.size();
        const auto &newProperties = state.properties();
        if (newProperties.size() != size)
            return *this;
        for (int i = 0; i < size; i++)
        {
            properties_[i] = newProperties[i];
        }
        return *this;
    }
    void apply(const State &state)
    {
        const int size = properties_.size();
        const auto &newProperties = state.properties();
        if (newProperties.size() != size)
            return;
        for (int i = 0; i < size; i++)
        {
            properties_[i] += newProperties[i];
        }
    }

private:
    GEB::Structures::Array<float> properties_;
};
