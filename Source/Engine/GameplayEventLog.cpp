#include "GameplayEventLog.h"
#include "Core.h"

namespace Plasmium {
    void GameplayEventLog::AddLog(StringId id) {
        while (logs.size() >= LogSize) {
            logs.pop_front();
        }
        const auto& window = Core::GetInstance().GetWindow();
        const float LineStart = (float)window.GetHeight() - 100.0f;
        vec4 drawColor(1.0f, 1.0f, 1.0f, 1.0f);
        rect drawArea(20, LineStart + LineHeight * logs.size(), 600, 50);

        auto& resourceManager = Core::GetInstance().GetResourceManager();
        auto& string = resourceManager.GetString(id);

        logs.push_back(Text2D(string.Get(), drawArea, drawColor));

        for (uint32 i = 0; i < logs.size(); ++i) {
            logs[i].ChangeDrawArea(rect(20, LineStart + LineHeight * i, 600, 50));
        }
    }

    void GameplayEventLog::ClearLog() {
        while (logs.size() > 0) {
            logs.pop_front();
        }
    }
}