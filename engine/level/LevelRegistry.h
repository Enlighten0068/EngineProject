#pragma once

#include "level/CameraMode.h"
#include "systems/LevelSystem.h"
#include <functional>
#include <string>
#include <vector>

/**
 * @brief Definition of a level: metadata + builder function.
 */
struct LevelDefinition{
    std::string Name; //Display name in the selector
    std::string Description; //Short description
    std::function<void(LevelSystem&)> Build; //Builder function
    CameraMode Mode = CameraMode::Fixed;
    bool IsNew = false; //Highlight flag
};

/**
 * @brief Global registry of all levels.
 *
 * Levels register themselves at program startup via static initialization
 * (see LevelMacros.h). This allows adding new levels without modifying any
 * central file — just create a new .cpp and add it to CMakeLists.txt.
 *
 * @see REGISTER_LEVEL macro
 */
class LevelRegistry{
public:
    /**
     * @brief Gets the singleton instance.
     */
    static LevelRegistry& GetInstance();

    /**
     * @brief Registers a level. Called automatically by REGISTER_LEVEL macro.
     */
    void Register(const LevelDefinition& def);

    /**
     * @brief Gets all registered levels, in registration order.
     */
    const std::vector<LevelDefinition>& GetLevels() const{ return m_Levels; }

    /**
     * @brief Gets the number of registered levels.
     */
    size_t Count() const{ return m_Levels.size(); }

private:
    LevelRegistry() = default;
    std::vector<LevelDefinition> m_Levels;
};
