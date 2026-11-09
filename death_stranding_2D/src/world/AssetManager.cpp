#include "world/AssetManager.h"

AssetId initialAssetIds[] = {
    AssetId::TextSmall,
    AssetId::TextMedium,
    AssetId::TextLarge,
    AssetId::Cover,
    AssetId::TallyDefeat,
    AssetId::TallyVictory,
    AssetId::MonsterNestSprite,
    AssetId::DoorSprite,
    AssetId::KeySprite,
    AssetId::PotionSprite,
    AssetId::SwordsmanSprite,
    AssetId::ArcherSprite,
    AssetId::BeastSprite,
    AssetId::GhostSprite,
};

const char *getAssetPath(AssetId id)
{
    switch (id)
    {
    case AssetId::TextSmall:
        return "assets/font/font_9x18.png";
    case AssetId::TextMedium:
        return "assets/font/font_12x24.png";
    case AssetId::TextLarge:
        return "assets/font/font_18x36.png";
    case AssetId::Cover:
        return "assets/cover/game_cover.png";
    case AssetId::TallyDefeat:
        return "assets/cover/tally_defeat.png";
    case AssetId::TallyVictory:
        return "assets/cover/tally_victory.png";
    case AssetId::MonsterNestSprite:
        return "assets/monsters/nest/ghost_gate.png";
    case AssetId::DoorSprite:
        return "assets/objects/door.png";
    case AssetId::KeySprite:
        return "assets/objects/key.png";
    case AssetId::PotionSprite:
        return "assets/potion/health.png";
    case AssetId::SwordsmanSprite:
        return "assets/character/swordsman/swordsman-8-directions-96.png";
    case AssetId::ArcherSprite:
        return "assets/character/archer/archer-8-directions-96.png";
    case AssetId::BeastSprite:
        return "assets/monsters/werewolf/werewolf-8-directions-96.png";
    case AssetId::GhostSprite:
        return "assets/monsters/ghost/ghost-8-directions-96.png";
    default:
        return nullptr;
    }
}

AssetManager::~AssetManager()
{
    for (int i = 0; i < assets_.size(); ++i)
    {
        if (assets_[i].image != nullptr)
            delete assets_[i].image;
        assets_[i].image = nullptr;
    }
}

void AssetManager::loadImages()
{
    // Load each shared image once and keep it for later drawing.
    for (int i = 0; i < sizeof(initialAssetIds) / sizeof(initialAssetIds[0]); ++i)
    {
        AssetId id = initialAssetIds[i];
        const char *path = getAssetPath(id);
        if (path)
        {
            GEB::Image *image = new GEB::Image();
            if (!image->load(path))
            {
                delete image;
                image = nullptr;
            }
            else
            {
                Asset asset;
                asset.id = id;
                asset.image = image;
                assets_.pushBack(asset);
            }
        }
    }
}

GEB::Image *AssetManager::getImage(AssetId id)
{
    for (int i = 0; i < assets_.size(); ++i)
    {
        if (assets_[i].id == id)
            return assets_[i].image;
    }
    return nullptr;
}
