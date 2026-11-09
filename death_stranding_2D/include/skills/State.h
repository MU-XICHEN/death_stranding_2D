#pragma once
#include "structure/Array.h"
#include "core/GameBase.h"

// Store character stats and the changes from skills or potions.
class State final
{
public:
    // Add new stats before _END_FLAG_ and add their getters and setters.
    enum PropertyIndex
    {
        HEALTH = 0,
        SPEED,
        PERCEPTION_RANGE,
        DEFENSE,
        MANA,
        SCORE,
        _END_FLAG_
    };
    static constexpr int propCount = static_cast<int>(PropertyIndex::_END_FLAG_);

    State()
        : properties_(propCount) {};
    State(const State &state)
        : properties_(propCount) { *this = state; }
    ~State() = default;

    // methods
    void initialize(GEB::Pair<int, float> *newProp, int size)
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

    inline const GEB::Structures::Array<float> &properties() const { return properties_; };

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
    // Add the effect values to the current stats.
    void apply(const State &sideEffectState)
    {
        const int size = properties_.size();
        const auto &newProperties = sideEffectState.properties();
        if (newProperties.size() != size)
            return;
        for (int i = 0; i < size; i++)
        {
            if (newProperties[i] == 0.0f)
                continue;
            properties_[i] += newProperties[i];
        }
    }
    // Subtract the same values when a temporary effect ends.
    void unDo(const State &sideEffectState)
    {
        const int size = properties_.size();
        const auto &newProperties = sideEffectState.properties();
        if (newProperties.size() != size)
            return;
        for (int i = 0; i < size; i++)
        {
            if (newProperties[i] == 0.0f)
                continue;
            properties_[i] -= newProperties[i];
        }
    }

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

    float mana() const { return properties_[MANA]; }
    void setMana(float mana) { properties_[MANA] = mana; }

    // Accumulated points for the hero; defeat reward for monsters.
    float score() const { return properties_[SCORE]; }
    void setScore(float score) { properties_[SCORE] = score; }

private:
    GEB::Structures::Array<float> properties_;
};
