#include <cmath>
#include "Hexagon.h"


Hexagon::Hexagon() : isLinked(false), scale(0.0f), isReady(false) {
    type = EntityType::Hexagon;
    active = false;
}


Hexagon::~Hexagon() {}


void Hexagon::Initialize(Vector2 startPosition, float moveSpeed) {

    position = startPosition;

    // Get player position
    Vector2 center = { 
        static_cast<float>(Config::ScreenWidth) / 2.0f, 
        static_cast<float>(Config::ScreenHeight) / 2.0f 
    };

    float rad = atan2f(center.y - position.y, center.x - position.x);
    
    // Update heading
    heading = rad * RAD2DEG;

    speed.x = cosf(rad) * moveSpeed;
    speed.y = sinf(rad) * moveSpeed;

    isLinked = false;
    active = true;
    scale = 0.0f;
    isReady = false;
}


// Track hexagon link
bool Hexagon::IsLinked() const { 
    return isLinked; 
}


void Hexagon::SetLinked(bool linked) { 
    isLinked = linked; 
    speed = {0,0}; 
}


void Hexagon::SetBaseColor(Color color) { 
    baseColor = color; 
}


void Hexagon::Update(float deltaTime) {
    
    if (!active) {
        return;
    }

    // Growth
    if (!isReady) {
        // Update scale
        scale += growthSpeed * deltaTime;
        // Check if ready to move
        if (scale >= targetScale) {
            scale = targetScale;
            isReady = true;
        }
    }

    // Only move if not linked and ready to move
    else if (!isLinked) {
        position.x += speed.x * deltaTime;
        position.y += speed.y * deltaTime;
    }
}


void Hexagon::Draw() {

    if (!active || assets == nullptr) {
        return;
    }

    // Get hexagon texture
    Texture2D& hexagonTexture = assets->GetTexture(TextureId::Hexagon);

    // Origin
    Rectangle source = { 
        0.0f, 
        0.0f, 
        static_cast<float>(hexagonTexture.width), 
        static_cast<float>(hexagonTexture.height) 
    };

    // Destiantion
    Rectangle dest = { 
        position.x, 
        position.y, 
        static_cast<float>(hexagonTexture.width) * scale, 
        static_cast<float>(hexagonTexture.height) * scale 
    };

    // Rotation origin and scale
    Vector2 origin = { 
        (static_cast<float>(hexagonTexture.width) * scale) / 2.0f, 
        (static_cast<float>(hexagonTexture.height) * scale) / 2.0f 
    };

    // Fade
    float progress = scale / targetScale;

    if (progress > 1.0f) {
        progress = 1.0f;
    }

    // Convert to Raylib color
    unsigned char alpha = static_cast<unsigned char>(progress * 255.0f);

    // Set a color with alpha
    // Color tint = { 255, 255, 255, alpha };
    Color tint = { baseColor.r, baseColor.g, baseColor.b, alpha };

    // Draw hexagon
    DrawTexturePro(hexagonTexture, source, dest, origin, 0.0f, tint);
}
