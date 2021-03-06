//
// Created by Vali on 11/11/2020.
//

#ifndef VD_GAME_ENGINE_FRUSTUMCULLINGMANAGER_HPP
#define VD_GAME_ENGINE_FRUSTUMCULLINGMANAGER_HPP

#include <engine/component/IManager.hpp>

#include <engine/math/Plane.hpp>
#include <engine/math/Frustum.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/window/Window.hpp>

#include <memory>

namespace vd::culling {
    class FrustumCullingManager : public vd::component::IManager, public vd::injector::Injectable {
    public:
        FrustumCullingManager();

        void Link() override;

        void Init() override;
        void Update() override;
        void CleanUp() override;

        [[nodiscard]] const vd::math::Frustum& Frustum() const;
        [[nodiscard]] const vd::math::Bounds3& FrustumBounds() const;
    private:
        void UpdateVertices();
        void UpdatePlanes();
        void UpdateWidthsAndHeights();

        vd::math::Frustum m_Frustum;
        vd::math::Bounds3 m_FrustumBounds;

        struct { float width, height; } m_Near, m_Far;

        window::WindowPtr m_pWindow;
        camera::CameraPtr m_pCamera;
    };
    typedef std::shared_ptr<FrustumCullingManager>  FrustumCullingManagerPtr;
}

#endif //VD_GAME_ENGINE_FRUSTUMCULLINGMANAGER_HPP
