#pragma once
#include "structure/Array.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"

enum AssetId
{
    None = 0,
    TextSmall = 1,
    TextMedium = 2,
    TextLarge = 3,
    Cover = 4,
    SwordsmanSprite = 5,
    ArcherSprite = 6,
    BeastSprite = 8,
    GhostSprite = 9,
    TallyDefeat = 10,
    TallyVictory = 13,
    MonsterNestSprite = 11,
    DoorSprite = 14,
    KeySprite = 15,
    PotionSprite = 16,
};

struct Asset
{
    AssetId id = AssetId::None;
    GEB::Image *image = nullptr;
};

class AssetManager
{
public:
    AssetManager() = default;
    virtual ~AssetManager();

    void loadImages();

    GEB::Image *getImage(AssetId id);

private:
    GEB::Structures::Array<Asset> assets_;
};
