//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINRENDERER_HPP
#define VD_GAME_ENGINE_TERRAINRENDERER_HPP

#include <engine/component/IRenderer.hpp>
#include <engine/component/IEntityShader.hpp>

#include <engine/collision/Detector.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/culling/FrustumCullingManager.hpp>

#include "Terrain.hpp"
#include "TerrainNode.hpp"

namespace mod::terrain {
    class TerrainRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const vd::datastruct::Observable::priority_t kPriority = kDefaultPriority - 10;

        TerrainRenderer(TerrainPtr terrain, vd::component::IEntityShaderPtr shader);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;
    private:
        void CollectData(const TerrainNode::ptr_type_t& pNode, std::vector<float>& data, size_t& leafCount);

        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        size_t                                  m_LeafCount;
        std::vector<float>                      m_BufferData;
        TerrainPtr                              m_pTerrain;
        vd::component::IEntityShaderPtr         m_pShader;
        vd::culling::FrustumCullingManagerPtr   m_pFrustumCullingManager;
    };
    typedef std::shared_ptr<TerrainRenderer>	TerrainRendererPtr;
}


#endif //VD_GAME_ENGINE_TERRAINRENDERER_HPP
