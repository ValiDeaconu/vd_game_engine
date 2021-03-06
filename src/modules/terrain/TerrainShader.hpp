//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINSHADER_HPP
#define VD_GAME_ENGINE_TERRAINSHADER_HPP

#include <engine/component/IEntityShader.hpp>
#include <engine/loader/ShaderLoader.hpp>

#include <engine/injector/Injectable.hpp>

#include <engine/context/Context.hpp>
#include <engine/window/Window.hpp>
#include <engine/camera/Camera.hpp>
#include <engine/light/LightManager.hpp>
#include <engine/fog/FogManager.hpp>
#include <modules/shadow/ShadowManager.hpp>
#include <modules/biomes/BiomeManager.hpp>

#include <memory>

#include "Terrain.hpp"

namespace mod::terrain {
    class TerrainShader
            : public vd::component::IEntity2DShader
            , public vd::injector::Injectable
            , public std::enable_shared_from_this<TerrainShader> {
    public:
        void Link() override;
        void Init() override;

        void InitUniforms(vd::object::Entity2DPtr pEntity) override;
        void UpdateUniforms(vd::object::Entity2DPtr pEntity, uint64_t levelOfDetail, uint32_t meshIndex) override;
    private:
        uint32_t m_BiomeCount;

        TerrainPtr                      m_pTerrain;
        biomes::BiomeManagerPtr         m_pBiomeManager;
        vd::light::LightManagerPtr      m_pLightManager;
        vd::fog::FogManagerPtr          m_pFogManager;
        vd::window::WindowPtr           m_pWindow;
        vd::camera::CameraPtr           m_pCamera;
        vd::context::ContextPtr         m_pContext;
        mod::shadow::ShadowManagerPtr   m_pShadowManager;
    };
    typedef std::shared_ptr<TerrainShader>  TerrainShaderPtr;
}


#endif //VD_GAME_ENGINE_TERRAINSHADER_HPP
