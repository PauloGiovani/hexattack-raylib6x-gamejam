#include "GameState.h"


GameState::GameState() {
    Reset();
}


GameState::~GameState() {}


void GameState::Reset() {
    elapsedTime = 0.0f;
    status = GameStatus::Playing;
    currentWave = 1;
    waveDuration = 30.0f; 
    waveTimer = waveDuration;
}


int GameState::GetCurrentWave() const {
    return currentWave; 
}


float GameState::GetWaveTimer() const { 
    return waveTimer; 
}


float GameState::GetWaveDuration() const { 
    return waveDuration; 
}


void GameState::DecreaseWaveTimer(float deltaTime) {
    waveTimer -= deltaTime;
    if (waveTimer < 0.0f) {
        waveTimer = 0.0f;
    }
}


void GameState::NextWave() {
    currentWave++;
    waveDuration += 10.0f;
    waveTimer = waveDuration;

}


void GameState::SetElapsedTime(float time) {
    elapsedTime = time;
}


void GameState::AddElapsedTime(float deltaTime) {
    elapsedTime += deltaTime;
}


float GameState::GetElapsedTime() const {
    return elapsedTime;
}


void GameState::SetStatus(GameStatus status) {
    this->status = status;
}


GameStatus GameState::GetStatus() const {
    return status;
}


bool GameState::IsPlaying() const {
    return status == GameStatus::Playing;
}


bool GameState::IsPaused() const {
    return status == GameStatus::Paused;
}


bool GameState::IsGameOver() const {
    return status == GameStatus::GameOver;
}


