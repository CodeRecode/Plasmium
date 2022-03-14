#include "LevelManager.h"

#include "Core.h"

namespace Plasmium {
    void LevelManager::LoadLevelFile(FileResource levelFile)
    {
        auto & resourceManager = Core::GetInstance().GetResourceManager();
        currentLevel = &resourceManager.GetStaticLevelResource(levelFile);
        currentLevel->Load();
    }

    void LevelManager::ProcessEvent(const GenericEvent& event)
    {
        if ((EventType)event.index() == EventType::Input) {
            auto& inputEvent = std::get<InputEvent>(event);
            if (inputEvent.GetKeyDown(InputKey::F9)) {
                currentLevel->Unload();
                currentLevel->Load();
            }
        }
    }
}