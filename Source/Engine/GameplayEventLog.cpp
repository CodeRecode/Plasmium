#include "GameplayEventLog.h"

#include "Core.h"
#include "ResourceManager.h"
#include "Window.h"

namespace Plasmium {
    void GameplayEventLog::AddLog(StringId id) {
        while (logs.Size() >= LogSize) {
            logs.PopFront();
        }
        const auto& window = Core::GetInstance().GetWindow();
        const float LineStart = (float)window.GetHeight() - 100.0f;
        vec4 drawColor(1.0f, 1.0f, 1.0f, 1.0f);
        rect drawArea(20, LineStart + LineHeight * logs.Size(), 600, 50);

        auto& resourceManager = Core::GetInstance().GetResourceManager();
        auto& string = resourceManager.GetString(id);

        logs.PushBack(Text2D(string.Get(), drawArea, drawColor));

        uint32 index = 0;
        for (auto& log : logs) {
            log.ChangeDrawArea(rect(20, LineStart + LineHeight * index, 600, 50));
            ++index;
        }
    }

    void GameplayEventLog::ClearLog() {
        logs.Clear();
    }
}