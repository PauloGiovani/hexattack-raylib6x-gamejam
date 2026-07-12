#pragma once

#include "raylib.h"
#include "../Core/GameConfig.h"

enum class EntityType {
    Hexagon = 0,
    Player,
    Bullet,
    Enemy
};


class Entity {
public:

    Entity();
    virtual ~Entity();

    virtual void Update(float deltaTime);
    virtual void Draw();

    // Position
    void SetPosition(const Vector2& position);
    Vector2 GetPosition() const;

    // Speed
    void SetSpeed(const Vector2& speed);
    Vector2 GetSpeed() const;

    // Heading
    void SetHeading(float heading);
    float GetHeading() const;

    // Acceleration
    void SetAcceleration(float acceleration);
    float GetAcceleration() const;

    // Type
    void SetType(EntityType type);
    EntityType GetType() const;

    // Active
    void SetActive(bool active);
    bool IsActive() const;

protected:
    Vector2 position {};
    Vector2 speed {};
    float heading = Config::EntityHeading;
    float acceleration = Config::EntityAcceleration;
    EntityType type = EntityType::Player;
    bool active = true;
};
