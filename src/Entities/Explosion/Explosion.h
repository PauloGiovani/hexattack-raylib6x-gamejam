#pragma once

#include <memory>
#include "../../Core/GameConfig.h"
#include "../../Core/Animator.h"
#include "../SpriteEntity.h"


enum class ExplosionState {
    Play = 0
};


class Explosion : public SpriteEntity {
public:

    Explosion();
    virtual ~Explosion() override;

    // Required by unique_ptr
    Explosion(Explosion&&) noexcept = default;
    Explosion& operator=(Explosion&&) noexcept = default;

    void Initialize(Vector2 pos);
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:
    // Animation manager for explosion
    std::unique_ptr<Animator<ExplosionState>> animator;
};
