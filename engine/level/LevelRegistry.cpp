#include "level/LevelRegistry.h"
#include "diagnostics/Log.h"
#include <format>

LevelRegistry& LevelRegistry::GetInstance(){
    static LevelRegistry instance;
    return instance;
}

void LevelRegistry::Register(const LevelDefinition& def){
    m_Levels.push_back(def);
    Log::Info(std::format("Level registered: {} ({})", def.Name, def.Description));
}
