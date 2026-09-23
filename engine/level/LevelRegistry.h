#pragma once

#include "level/CameraMode.h"
#include "systems/LevelSystem.h"
#include <functional>
#include <string>
#include <vector>

/**
 * @brief Definition of a level: metadata + builder function.
 *
 * Each level is described by a display name, a short description, a
 * builder function (that fills a LevelSystem), and a camera mode.
 *
 * @see LevelRegistry, REGISTER_LEVEL
 */
struct LevelDefinition{
    std::string Name; //Display name in the selector
    std::string Description; //Short description
    std::function<void(LevelSystem&)> Build; //Builder function
    CameraMode Mode = CameraMode::Fixed; //Camera behavior for current level
    bool IsNew = false; //Highlight flag
};

/**
 * @brief Global registry of all levels.
 *
 * Levels register themselves at program startup via static initialization
 * (see LevelMacros.h). This allows adding new levels without modifying any
 * central file — just create a new .cpp, add it to CMakeLists.txt, and the
 * selector will list it automatically.
 *
 * @note This class is a singleton.
 * @see REGISTER_LEVEL, LevelSelectorScene
 */
class LevelRegistry{
public:
    /**
     * @brief Gets the singleton instance.
     * @return Reference to the LevelRegistry.
     */
    static LevelRegistry& GetInstance();

    /**
     * @brief Registers a level.
     *
     * Called automatically by the REGISTER_LEVEL macro at program startup.
     *
     * @param def The level definition to register.
     */
    void Register(const LevelDefinition& def);

    /**
     * @brief Gets all registered levels, in registration order.
     * @return Const reference to the vector of level definitions.
     */
    const std::vector<LevelDefinition>& GetLevels() const{ return m_Levels; }

    /**
     * @brief Gets the number of registered levels.
     * @return Number of levels.
     */
    size_t Count() const{ return m_Levels.size(); }
private:
    LevelRegistry() = default;
    std::vector<LevelDefinition> m_Levels;
};
