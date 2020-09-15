#include "Renderer.hpp"

namespace vd::component
{
    Renderer::Renderer()
        : renderConfigPtr(nullptr)
        , shaderPtr(nullptr)
    {
    }

    Renderer::~Renderer() = default;

    config::MetaConfigPtr& Renderer::getRenderConfig()
    {
        return renderConfigPtr;
    }

    const config::MetaConfigPtr& Renderer::getRenderConfig() const
    {
        return renderConfigPtr;
    }

    void Renderer::setRenderConfig(const config::MetaConfigPtr& renderConfigPtr)
    {
        this->renderConfigPtr = renderConfigPtr;
    }

    vd::shader::ShaderPtr& Renderer::getShader()
    {
        return shaderPtr;
    }

    const vd::shader::ShaderPtr& Renderer::getShader() const
    {
        return shaderPtr;
    }

    void Renderer::setShader(const vd::shader::ShaderPtr& shaderPtr)
    {
        this->shaderPtr = shaderPtr;
    }

    bool Renderer::isReady()
    {
        return shaderPtr != nullptr;
    }

    shadow::ShadowShaderPtr& Renderer::getShadowShader() const {
        return shadow::GetShadowShader();
    }
}