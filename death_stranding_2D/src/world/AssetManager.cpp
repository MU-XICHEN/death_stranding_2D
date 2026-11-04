#include "world/AssetManager.h"

AssetId initialAssetIds[] = {
    AssetId::TextSmall,
    AssetId::TextMedium,
    AssetId::TextLarge,
    AssetId::Cover,
    AssetId::SwordsmanSprite,
    AssetId::ArcherSprite,
    AssetId::MageSprite,
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
    case AssetId::SwordsmanSprite:
        return "assets/character/swordsman/swordsman-8-directions-96.png";
    case AssetId::ArcherSprite:
        return "assets/character/archer/archer-8-directions-96.png";
    case AssetId::MageSprite:
        return "assets/character/mage/mage-8-directions-96.png";
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
