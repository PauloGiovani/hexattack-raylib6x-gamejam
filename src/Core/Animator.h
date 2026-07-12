// ----------------------------------------------------------------------------
// Animator helper
// Created by: Paulo Giovani
// ----------------------------------------------------------------------------

#pragma once

#include "raylib.h"
#include <map>
#include "Animation.h"

// Generic state-based animation class
template <typename StateType>
class Animator {
public:

    Animator(Texture2D texture) : texture(texture) {
        sourceRect = {0, 0, 0, 0};
    }


    // Add an animation associated with a state
    void AddAnimation(StateType state, const Animation& animation) {
        animations[state] = animation;
    }


    // Set current state
    void SetState(StateType state) {
        // Safety, to avoid unnecessary resets
        if (currentState == state && !hasFinished) {
            return;
        }
        // State update
        currentState = state;
        // Animation reset
        animations[state].currentFrame = 0;
        animations[state].timer = 0;
        hasFinished = false;
    }


    // Get the current state
    StateType GetState() const {
        return currentState;
    }


    // Check if a animation loop has finished
    bool IsAnimationFinished() const {
        return hasFinished;
    }


    // Get the size of the current frame
    Vector2 GetFrameSize() const {
        return { sourceRect.width, sourceRect.height };
    }


    // Update animator
    void Update(float deltaTime) {

        // Reference for current animation
        Animation& anim = animations[currentState];

        // Animation loop safety
        if (!anim.loop && hasFinished) {
            return;
        }

        // Update timer
        anim.timer += deltaTime;

        // Update animation frame
        if (anim.timer >= anim.frameTime) {
            anim.timer = 0;
            // Check if there are more frames
            if (anim.currentFrame < anim.frameCount - 1) {
                anim.currentFrame++;
            } 
            else {
                // Reached the last frame
                if (anim.loop) {
                    // Restart loop
                    anim.currentFrame = 0; 
                } 
                else {
                    // Stop here and flag as finished
                    hasFinished = true;
                }
            }
        }

        // Update the rectangle's position with the animation frame
        sourceRect.x = anim.currentFrame * anim.frameWidth;
        sourceRect.y = anim.startY * anim.frameHeight;

        // Update the width and height of the rectangle with the animation frame
        sourceRect.width = anim.frameWidth;
        sourceRect.height = anim.frameHeight;
    }


    // Render animator
    void Draw(Vector2 position, bool flip, Color tint = WHITE) {
        // Rectangle with the object's position
        Rectangle destRect = {
            position.x,
            position.y,
            (float) sourceRect.width,
            (float) sourceRect.height
        };

        // Rectangle with a copy of the object's texture
        Rectangle src = sourceRect;

        // Reverse the horizontal direction
        if (flip) {
            src.width = -src.width;
        }

        // Render object
        DrawTexturePro(
            texture,
            src,
            destRect,
            {0, 0},
            0.0f,
            tint
        );
    }


    // Render animator with rotation
    void DrawWithRotation(
        Vector2 position, 
        float rotation, 
        Vector2 origin, 
        float scale = 1.0f, 
        bool flip = false, 
        Color tint = WHITE
    ) {

        // Rectangle with the object's position on screen
        Rectangle destRect = {
            position.x,
            position.y,
            (float) sourceRect.width * scale,
            (float) sourceRect.height * scale
        };

        // Rectangle with a copy of the object's texture
        Rectangle src = sourceRect;

        // Reverse the horizontal direction
        if (flip) {
            src.width = -src.width;
        }

        // Render object with rotation and custom origin
        DrawTexturePro(
            texture,
            src,
            destRect,
            origin,
            rotation,
            tint
        );
    }

private:
    Texture2D texture;
    std::map<StateType, Animation> animations;
    StateType currentState{};
    Rectangle sourceRect;

    // Track animation loop
    bool hasFinished = false;
};
