#pragma once

#include "../../Core/GameConfig.h"
#include "../SpriteEntity.h"

class Bullet : public SpriteEntity {
public:

    Bullet();
    virtual ~Bullet() override;

    void Initialize();
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:
    float bulletSpeed = Config::BulletSpeed;
};
