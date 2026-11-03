#pragma once

#include <string>
#include "entities/Entity.h"

class Hero;

class InfoEntity : public Entity
{
public:
    explicit InfoEntity(const Hero &hero);

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void refreshText();

    // Observes the grid-owned hero until it becomes inactive.
    const Hero *hero_;
    std::string healthText_;
    std::string manaText_;
    std::string scoreText_;
    std::string killsText_;
};
