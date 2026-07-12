#pragma once

#include "../../Core/GameConfig.h"
#include "../SpriteEntity.h"


class EnemyPlatform : public SpriteEntity {
public:
    EnemyPlatform();
    virtual ~EnemyPlatform() override;
    
    void Initialize(int cornerIndex);
    void Reset();
    virtual void Draw() override;

private:
    TextureId baseTexture;
};
