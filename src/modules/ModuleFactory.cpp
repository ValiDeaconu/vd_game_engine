//
// Created by Vali on 11/19/2020.
//

#include "ModuleFactory.hpp"

// Factories
#include <modules/gui/GuiFactory.hpp>
#include <modules/player/PlayerFactory.hpp>
#include <modules/shadow/ShadowFactory.hpp>
#include <modules/sky/SkyFactory.hpp>
#include <modules/sobj/StaticObjectFactory.hpp>
#include <modules/terrain/TerrainFactory.hpp>
#include <modules/water/WaterFactory.hpp>

namespace mod {

    void ModuleFactory::Create(const vd::EnginePtr& pEngine) {
        std::make_shared<shadow::ShadowFactory>()->Create(pEngine);

        std::make_shared<terrain::TerrainFactory>()->Create(pEngine);
        std::make_shared<player::PlayerFactory>()->Create(pEngine);
        std::make_shared<water::WaterFactory>()->Create(pEngine);
        std::make_shared<sky::SkyFactory>()->Create(pEngine);
        std::make_shared<sobj::StaticObjectFactory>()->Create(pEngine);

        std::make_shared<gui::GuiFactory>()->Create(pEngine);
    }

}
