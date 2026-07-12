#include "SpriteEntity.h"


SpriteEntity::SpriteEntity() {}


SpriteEntity::~SpriteEntity() {}


void SpriteEntity::SetAssets(Assets* assets) {
    this->assets = assets;
}


// Draw sprite
void SpriteEntity::DrawSprite(TextureId textureId) {

    if (!active) {
        return;
    }

    if (assets == nullptr) {
        return;
    }

    // Get texture
    Texture2D& texture = assets->GetTexture(textureId);

    // Draw
    DrawTexturePro(
        texture,
        Rectangle {
            0.0f,
            0.0f,
            static_cast<float>(texture.width),
            static_cast<float>(texture.height)
        },
        Rectangle {
            position.x,
            position.y,
            static_cast<float>(texture.width) / 2.0f,
            static_cast<float>(texture.height) / 2.0f
        },
        Vector2 {
            static_cast<float>(texture.width) / 4.0f,
            static_cast<float>(texture.height) / 4.0f
        },
        heading,
        RAYWHITE
    );
}
