#pragma once

#include "../Core/Assets.h"
#include "Entity.h"


class SpriteEntity : public Entity {
public:

    SpriteEntity();
    virtual ~SpriteEntity();
    void SetAssets(Assets* assets);

protected:
    void DrawSprite(TextureId textureId);
    Assets* assets = nullptr;
};
