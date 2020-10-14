//
// Created by Vali on 9/19/2020.
//

#ifndef VD_GAME_ENGINE_UTIL_HPP
#define VD_GAME_ENGINE_UTIL_HPP

#include <glm/glm.hpp>

#include "Bounds.hpp"

namespace vd::math {
    bool isPointInsideBoundingBox(const glm::vec3& point, const Bounds3& bounds);
}

#endif //VD_GAME_ENGINE_UTIL_HPP
