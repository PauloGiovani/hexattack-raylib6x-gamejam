#include <cmath>
#include "CollisionManager.h"


CollisionManager::CollisionManager() {}


CollisionManager::~CollisionManager() {}


// Bullet x Hexagon
void CollisionManager::CheckBulletHexagonCollisions(
    std::vector<Bullet>& bullets, 
    std::vector<Hexagon>& hexagons, 
    std::vector<Explosion>& explosions,
    Assets* assets
) {
    

    for (auto& bullet : bullets) {

        if (!bullet.IsActive()) {
            continue;
        }

        for (auto& hexagon : hexagons) {

            if (!hexagon.IsActive() || hexagon.IsLinked()) {
                continue;
            }

            // Collision radius
            float bulletRadius = Config::CollisionBulletRadius;
            float hexagonRadius = Config::CollisionHexagonRadius; 

            // Check collision using circles
            if (CheckCollisionCircles(
                    bullet.GetPosition(),
                    bulletRadius, 
                    hexagon.GetPosition(), 
                    hexagonRadius
                )
            ) {

                // Destroy bullet
                bullet.SetActive(false); 

                if (!hexagon.IsLinked()) {

                    // Destroy hexagon
                    hexagon.SetActive(false);

                    if (assets != nullptr) {
                        PlaySound(assets->GetSound(SoundId::Explosion));
                    }

                    Explosion newExplosion;
                    newExplosion.SetAssets(assets);
                    newExplosion.Initialize(hexagon.GetPosition());

                    // Add / move explosion to list
                    //explosions.push_back(newExplosion);
                    explosions.push_back(std::move(newExplosion));
                }

                break; 
            }
        }
    }
}


// Hexagon x Player
void CollisionManager::CheckHexagonLinking(
    std::vector<Hexagon>& hexagons, 
    Vector2 playerPos,
    Assets* assets
) {

    // Collision radius
    float playerRadius = Config::CollisionPlayerRadius;
    float hexagonRadius = Config::CollisionHexagonRadius;

    // Collision padding
    float padding = Config::CollisionHexagonPadding;

    // Get distances to fix hexagons after collision
    float distToPlayer = playerRadius + hexagonRadius + padding;
    float distToHexagon = hexagonRadius + hexagonRadius + padding;

    // Collision with player platform
    for (auto& hexagon : hexagons) {

        if (!hexagon.IsActive() || hexagon.IsLinked()) {
            continue;
        }

        if (CheckCollisionCircles(
                hexagon.GetPosition(), 
                hexagonRadius, 
                playerPos, 
                playerRadius
            )
        ) {

            // Get new values to fix hexagon
            float dx = hexagon.GetPosition().x - playerPos.x;
            float dy = hexagon.GetPosition().y - playerPos.y;
            float distance = sqrtf(dx * dx + dy * dy);

            if (distance > 0.0f) {
                // Normalize
                float nx = dx / distance;
                float ny = dy / distance;
                // Set hexagon new position
                hexagon.SetPosition({ 
                    playerPos.x + nx * distToPlayer, 
                    playerPos.y + ny * distToPlayer 
                });
            }

            hexagon.SetLinked(true);

            if (assets != nullptr) {
                PlaySound(assets->GetSound(SoundId::HexagonHit));
            }
        }
    }

    // Collision with a linked hexagon
    // Track linked hexagons using a chain reaction method
    bool chainReaction = true;

    while (chainReaction) {

        chainReaction = false;
        
        for (auto& hexagonA : hexagons) {

            if (!hexagonA.IsActive() || hexagonA.IsLinked()) {
                continue;
            }

            for (auto& hexagonB : hexagons) {

                if (!hexagonB.IsActive() || !hexagonB.IsLinked()) {
                    continue;
                }

                // Check collision using circles (Hexagon A and Hexagon B)
                if (CheckCollisionCircles(
                        hexagonA.GetPosition(), 
                        hexagonRadius, 
                        hexagonB.GetPosition(), 
                        hexagonRadius
                    )
                ) {

                    hexagonA.SetLinked(true);

                    if (assets != nullptr) {
                        PlaySound(assets->GetSound(SoundId::HexagonHit));
                    }

                    // Enable checking with another
                    chainReaction = true;

                    break;
                }
            }

            // If failed, break loop
            if (chainReaction) {
                break;
            }
        }

        // Adjust hexagons position, adding padding
        int iterationSteps = Config::CollisionIterationSteps;

        // For each iteration
        for (int step = 0; step < iterationSteps; step++) {

            // For each not linked hexagon
            for (auto& hexagonA : hexagons) {
            
                if (!hexagonA.IsActive() || !hexagonA.IsLinked()) {
                    continue;
                }

                // Get distances to fix hexagons after collision with player
                float dx = hexagonA.GetPosition().x - playerPos.x;
                float dy = hexagonA.GetPosition().y - playerPos.y;
                float distanceToPlayer = sqrtf(dx * dx + dy * dy);

                if (distanceToPlayer < distToPlayer && distanceToPlayer > 0.0f) {
                    // Normalize
                    float nx = dx / distanceToPlayer;
                    float ny = dy / distanceToPlayer;
                    // Get overlap with player
                    float overlap = distToPlayer - distanceToPlayer;
                    // Set hexagon new position
                    hexagonA.SetPosition({
                        hexagonA.GetPosition().x + nx * overlap,
                        hexagonA.GetPosition().y + ny * overlap
                    });
                }

                // Solve overlap
                for (auto& hexagonB : hexagons) {

                    if (!hexagonB.IsActive() || !hexagonB.IsLinked() || &hexagonA == &hexagonB) {
                        continue;
                    }

                    // Get distances to fix hexagons after collision with others
                    float hx = hexagonA.GetPosition().x - hexagonB.GetPosition().x;
                    float hy = hexagonA.GetPosition().y - hexagonB.GetPosition().y;
                    float distanceToHex = sqrtf(hx * hx + hy * hy);

                    if (distanceToHex < distToHexagon && distanceToHex > 0.0f) {
                        // Normalize
                        float nx = hx / distanceToHex;
                        float ny = hy / distanceToHex;
                        // Get overlap with other
                        float overlap = distToHexagon - distanceToHex;
                        // Set hexagon A new position
                        hexagonA.SetPosition({
                            hexagonA.GetPosition().x + nx * (overlap * 0.5f),
                            hexagonA.GetPosition().y + ny * (overlap * 0.5f)
                        });
                        // Set hexagon B new position
                        hexagonB.SetPosition({
                            hexagonB.GetPosition().x - nx * (overlap * 0.5f),
                            hexagonB.GetPosition().y - ny * (overlap * 0.5f)
                        });
                    }
                }
            }
        }
    }
}


// Hexagon x Corner Enemy
void CollisionManager::CheckHexagonEnemyCollisions(
    std::vector<Hexagon>& hexagons, 
    GameState& gameState
) {
    
    float offsetX = Config::CollisionEnemyPlatformOffsetX;
    float offsetY = Config::CollisionEnemyPlatformOffsetY;
    float enemyPlatformRadius = Config::CollisionEnemyPlatformRadius;
    float hexagonRadius = Config::CollisionEnemyPlatformHexagonRadius;

    // Top-Left, Top-Right, Bottom-Right, Bottom-Left
    Vector2 cornerPositions[4] = {
        { offsetX, offsetY },
        { Config::ScreenWidth - offsetX, offsetY },
        { Config::ScreenWidth - offsetX, Config::ScreenHeight - offsetY },
        { offsetX, Config::ScreenHeight - offsetY }
    };

    for (auto& hexagon : hexagons) {

        if (!hexagon.IsActive() || !hexagon.IsLinked()) {
            continue;
        }

        // For each enemy
        for (int i = 0; i < 4; i++) {

            // Check collision using circles
            if (CheckCollisionCircles(
                hexagon.GetPosition(), 
                hexagonRadius, 
                cornerPositions[i], 
                enemyPlatformRadius
                )
            ) {

                // Change enemy color
                for (auto& hexToPaint : hexagons) {
                    if (hexToPaint.IsActive() && hexToPaint.IsLinked()) {
                        hexToPaint.SetBaseColor(RED);
                    }
                }

                // Game over
                gameState.SetStatus(GameStatus::GameOver); 

                return;
            }
        }
    }
}

