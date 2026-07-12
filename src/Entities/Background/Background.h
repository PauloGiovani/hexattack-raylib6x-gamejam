#pragma once

#include "raylib.h"
#include "../../Core/GameConfig.h"


class Background {
public:
    Background();
    ~Background();

    void Initialize();
    void Update(float deltaTime);
    void Draw();

private:
    // Shader
    Shader spaceShader;
    int timeLoc;
    int resolutionLoc;
    float runTime;
};
