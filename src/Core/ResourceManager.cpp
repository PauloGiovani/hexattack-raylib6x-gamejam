#include "ResourceManager.h"


ResourceManager::ResourceManager() {}


ResourceManager::~ResourceManager() {}


void ResourceManager::Initialize() {
    assets.Load();
}


void ResourceManager::Shutdown() {
    assets.Unload();
}


Assets& ResourceManager::GetAssets() {
    return assets;
}


const Assets& ResourceManager::GetAssets() const {
    return assets;
}
