//
// Created by Vali on 11/12/2020.
//

#ifndef VD_GAME_ENGINE_THIRDPERSONCAMERA_HPP
#define VD_GAME_ENGINE_THIRDPERSONCAMERA_HPP

#include <engine/camera/ICamera.hpp>
#include <engine/math/Transform.hpp>

#include <functional>

namespace vd::camera::impl {
    class ThirdPersonCamera : public ICamera {
    public:
        typedef std::function<vd::math::Transform(void)> PlayerTransformGetter;
        typedef std::function<float(float, float)> HeightGetter;

        ThirdPersonCamera();
        ~ThirdPersonCamera();

        void Link();

        void Reflect(const Axis& axis, float amount) override;

        float& Pitch();
        float& DistanceFromPlayer();
        float& AngleAroundPlayer();

    private:
        friend class camera::CameraManager;

        void Update();

        float HorizontalDistance() const;
        float VerticalDistance() const;

        glm::vec3 CameraPosition(const glm::vec3& playerPosition, float playerAngle, float horizontalDistance, float verticalDistance) const;

        float m_Pitch;
        float m_DistanceFromPlayer;
        float m_AngleAroundPlayer;

        PlayerTransformGetter m_PlayerTransformGetter;
        HeightGetter m_HeightGetter;

        glm::vec3 m_Offset;
    };
}

#endif // !VD_GAME_ENGINE_THIRDPERSONCAMERA_HPP