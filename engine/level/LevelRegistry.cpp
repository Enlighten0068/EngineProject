#include "level/LevelRegistry.h"
#include "diagnostics/Log.h"
#include <format>

/**
 * @brief Returns the singleton instance of LevelRegistry.
 * @return Reference to the LevelRegistry instance.
 */
LevelRegistry& LevelRegistry::GetInstance(){
    static LevelRegistry instance;
    return instance;
}

/**
 * @brief Registers a level.
 * @param def The level definition to register.
 */
void LevelRegistry::Register(const LevelDefinition& def){
    m_Levels.push_back(def);
    Log::Info(std::format("Level registered: {} ({})", def.Name, def.Description));
}
