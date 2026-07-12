#pragma once

#include "Assets.h"

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    void Initialize();
    void Shutdown();
    Assets& GetAssets();
    const Assets& GetAssets() const;

private:

    Assets assets;
};
