#include "Background.h"


Background::Background() : runTime(0.0f) {}


Background::~Background() {
    UnloadShader(spaceShader);
}


void Background::Initialize() {

// Shader version
#if defined(__EMSCRIPTEN__) || defined(PLATFORM_WEB)
    spaceShader = LoadShader(0, "resources/shaders/glsl100/background.fs");
#else
    spaceShader = LoadShader(0, "resources/shaders/glsl330/background.fs");
#endif

    // Get shader IDs
    timeLoc = GetShaderLocation(spaceShader, "time");
    resolutionLoc = GetShaderLocation(spaceShader, "resolution");

    // Get screen resolution
    float resolution[2] = { 
        static_cast<float>(Config::ScreenWidth), 
        static_cast<float>(Config::ScreenHeight) 
    };

    // Set shader uniform
    SetShaderValue(spaceShader, resolutionLoc, resolution, SHADER_UNIFORM_VEC2);
    
    // Reset time
    runTime = 0.0f;

}


void Background::Update(float deltaTime) {
    runTime += deltaTime;
    // Update shader
    SetShaderValue(spaceShader, timeLoc, &runTime, SHADER_UNIFORM_FLOAT);
}


void Background::Draw() {
    BeginShaderMode(spaceShader);
        // Draw shader area
        DrawRectangle(0, 0, Config::ScreenWidth, Config::ScreenHeight, WHITE);
    EndShaderMode();
}
