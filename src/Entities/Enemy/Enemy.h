#pragma once

#include "raylib.h"
#include <memory>
#include <cmath>
#include "../../Core/GameConfig.h"
#include "../../Core/Animator.h"
#include "../SpriteEntity.h"


enum class EnemyState {
    Idle = 0
};


class Enemy : public SpriteEntity {
public:

    Enemy();
    virtual ~Enemy() override;

    void Initialize(int cornerIndex);
    void Reset();
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:
    int corner;
    bool flipX = false;
    std::unique_ptr<Animator<EnemyState>> enemyAnimator;
};
