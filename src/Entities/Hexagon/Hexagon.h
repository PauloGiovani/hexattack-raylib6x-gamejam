#pragma once

#include "../../Core/GameConfig.h"
#include "../SpriteEntity.h"


class Hexagon : public SpriteEntity {

public:
    Hexagon();
    virtual ~Hexagon() override;

    void Initialize(Vector2 startPosition, float moveSpeed);

    bool IsLinked() const;
    void SetLinked(bool linked);
    void SetBaseColor(Color color);
    void IncreaseSpeed();

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

private:
    bool isLinked = false;
    float scale = Config::HexagonScale;
    float targetScale = Config::HexagonTargetScale;
    float growthSpeed = Config::HexagonGrowthSpeed;
    bool isReady = false;
    Color baseColor = WHITE;
};
