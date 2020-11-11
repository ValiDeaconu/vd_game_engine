#include "SkyRenderer.hpp"

namespace mod::sky {
    SkyRenderer::SkyRenderer()
        : Renderer()
        , m_SkyPtr(nullptr)
    {
    }

    SkyRenderer::~SkyRenderer() = default;

    void SkyRenderer::Init() {
        m_SkyPtr->Init();
    }

    void SkyRenderer::Update() {
        m_SkyPtr->Update();
    }

    void SkyRenderer::Render(const vd::kernel::RenderingPass& renderingPass) {
        if (IsReady()) {
            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->enable();
            }

            auto _shaderPtr = renderingPass == vd::kernel::RenderingPass::eShadow ? this->GetShadowShader() : m_ShaderPtr;

            _shaderPtr->bind();
            vd::buffer::BufferPtrVec& buffers = m_SkyPtr->GetBuffers();
            for (size_t meshIndex = 0; meshIndex < buffers.size(); ++meshIndex) {
                _shaderPtr->updateUniforms(m_SkyPtr, meshIndex);
                buffers[meshIndex]->Render();
            }

            if (m_ConfigPtr != nullptr) {
                m_ConfigPtr->disable();
            }
        }
    }

    void SkyRenderer::CleanUp() {
        m_SkyPtr->CleanUp();
    }

    SkyPtr& SkyRenderer::GetSky() {
        return m_SkyPtr;
    }

    const SkyPtr& SkyRenderer::GetSky() const {
        return m_SkyPtr;
    }

    void SkyRenderer::SetSky(const SkyPtr& skyPtr) {
        this->m_SkyPtr = skyPtr;
    }

    bool SkyRenderer::IsReady()
    {
        return Renderer::IsReady() && m_SkyPtr != nullptr;
    }
}