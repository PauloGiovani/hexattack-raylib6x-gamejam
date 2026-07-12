#pragma once

#include <array>
#include <vector>
#include "../Core/GameConfig.h"
#include "../Core/Assets.h"
#include "../Entities/Player/Bullet.h"
#include "../Entities/Hexagon/Hexagon.h"
#include "../Entities/Explosion/Explosion.h"
#include "GameState.h"


class CollisionManager {
public:

    CollisionManager();
    ~CollisionManager();

    // Bullet x Hexagon
    void CheckBulletHexagonCollisions(
        std::vector<Bullet>& bullets, 
        std::vector<Hexagon>& hexagons, 
        std::vector<Explosion>& explosions,
        Assets* assets
    );

    // Hexagon x Player
    void CheckHexagonLinking(
        std::vector<Hexagon>& hexagons, 
        Vector2 playerPos,
        Assets* assets
    );

    // Hexagon x Corner Enemy
    void CheckHexagonEnemyCollisions(
        std::vector<Hexagon>& hexagons, 
        GameState& gameState
    );
};
