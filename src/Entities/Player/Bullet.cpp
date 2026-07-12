#include <cmath>
#include "Bullet.h"


Bullet::Bullet() {
    type = EntityType::Bullet;
    active = false;
}


Bullet::~Bullet() {}


void Bullet::Initialize() {
    float rad = heading * DEG2RAD;
    speed.x = cosf(rad) * bulletSpeed;
    speed.y = sinf(rad) * bulletSpeed;
    active = true;
}


void Bullet::Update(float deltaTime) {
    if (!active) {
        return;
    }

    position.x += speed.x * deltaTime;
    position.y += speed.y * deltaTime;
}


void Bullet::Draw() {
    if (!active || assets == nullptr) {
        return;
    }

    DrawSprite(TextureId::PlayerBullet);
}
