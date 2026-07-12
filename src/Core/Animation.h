// ----------------------------------------------------------------------------
// Animation structure helper
// Created by: Paulo Giovani
// ----------------------------------------------------------------------------

#pragma once

// frameCount: number of frames
// frameTime: time per frame
// currentFrame: current frame
// timer: time control
// frameWidth: frame width
// frameHeight: frame height
// startY: row in the spritesheet
// loop: animation loop

struct Animation {
    int frameCount;
    float frameTime;
    int currentFrame;
    float timer;
    int frameWidth;
    int frameHeight;
    int startY;
    bool loop;
};
