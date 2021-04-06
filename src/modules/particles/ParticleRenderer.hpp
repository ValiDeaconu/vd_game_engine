//
// Created by Vali on 4/3/2021.
//

#include <memory>
#include <list>

#include <engine/component/IRenderer.hpp>

#include <engine/injector/Injectable.hpp>
#include <engine/context/Context.hpp>
#include <engine/camera/Camera.hpp>
#include <modules/player/Player.hpp>

#include <engine/api/gl/Context.hpp>

#include <modules/water/WaterRenderer.hpp>

#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "ParticleShader.hpp"

namespace mod::particles {
    class ParticleRenderer : public vd::component::IRenderer, public vd::injector::Injectable {
    public:
        static const int kPriority = water::WaterRenderer::kPriority + 1;

        ParticleRenderer(ParticleSystemPtr particleSystem);

        void Link() override;

        void OnInit() override;
        void OnUpdate() override;
        void OnRender(const params_t& params) override;
        void OnCleanUp() override;
    private:
        bool Precondition(const params_t& params) override;
        void Prepare() override;
        void Finish() override;

        static glm::mat4 BuildModelViewMatrix(const glm::vec3& position, float scale, float rotation, const glm::mat4& view);

        size_t                      m_DataLength;
        size_t                      m_ParticleCount;

        vd::gl::BufferPtr           m_pBuffer;

        std::list<ParticlePtr>      m_Batch;
        std::vector<float>          m_BufferData;

        vd::context::ContextPtr     m_pContext;
        vd::camera::CameraPtr       m_pCamera;
        mod::player::PlayerPtr      m_pPlayer;

        ParticleShaderPtr           m_pShader;
        ParticleSystemPtr           m_pSystem;
    };
    typedef std::shared_ptr<ParticleRenderer>	ParticleRendererPtr;
}