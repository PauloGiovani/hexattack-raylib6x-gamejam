#pragma once

#include "../Core/GameConfig.h"


enum class GameStatus {
    Playing = 0,
    Paused,
    GameOver
};


class GameState {
public:

    GameState();
    ~GameState();

    void Reset();

    // Wave control
    int GetCurrentWave() const;
    float GetWaveTimer() const;
    float GetWaveDuration() const;
    void DecreaseWaveTimer(float deltaTime);
    void NextWave();

    // Timer
    void SetElapsedTime(float time);
    void AddElapsedTime(float deltaTime);
    float GetElapsedTime() const;

    void SetStatus(GameStatus status);
    GameStatus GetStatus() const;

    // Track game state
    bool IsPlaying() const;
    bool IsPaused() const;
    bool IsGameOver() const;

private:

    float elapsedTime = Config::GameStateElapsedTime;
    GameStatus status = GameStatus::Playing;
    int currentWave = Config::GameStateCurrentWave;
    float waveDuration = Config::GameStateWaveDuration;
    float waveTimer = Config::GameStateWaveTimer;
};
