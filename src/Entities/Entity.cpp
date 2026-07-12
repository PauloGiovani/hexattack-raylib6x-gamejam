#include "Entity.h"


Entity::Entity() {}


Entity::~Entity() {}


void Entity::Update(float deltaTime) {
    (void) deltaTime;
}


void Entity::Draw() {}


void Entity::SetPosition(const Vector2& position) {
    this->position = position;
}


Vector2 Entity::GetPosition() const {
    return position;
}


void Entity::SetSpeed(const Vector2& speed) {
    this->speed = speed;
}


Vector2 Entity::GetSpeed() const {
    return speed;
}


void Entity::SetHeading(float heading) {
    this->heading = heading;
}


float Entity::GetHeading() const {
    return heading;
}


void Entity::SetAcceleration(float acceleration) {
    this->acceleration = acceleration;
}


float Entity::GetAcceleration() const {
    return acceleration;
}


void Entity::SetType(EntityType type) {
    this->type = type;
}


EntityType Entity::GetType() const {
    return type;
}


void Entity::SetActive(bool active) {
    this->active = active;
}


bool Entity::IsActive() const {
    return active;
}
