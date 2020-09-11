#include "SkyShader.hpp"

namespace mod::sky
{
    SkyShader::SkyShader()
        : vd::shader::Shader()
    {
        loadAndAddShader("./resources/shaders/sky/sky_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/sky/sky_FS.glsl", vd::shader::eFragmentShader);
        compileShader();

        addUniform("view");
        addUniform("projection");

        addUniform("fogDensity");
        addUniform("fogColor");
    }

    SkyShader::~SkyShader() = default;

    void SkyShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex)
    {
        auto& enginePtr = entityPtr->getParentEngine();
        setUniform("view", glm::mat4(glm::mat3(enginePtr->getCamera()->getViewMatrix())));
        setUniform("projection", enginePtr->getWindow()->getProjectionMatrix());

        static bool loadedBasics = false;
        if (!loadedBasics)
        {
            auto& engineConfigPtr = enginePtr->getEngineConfig();
            setUniformf("fogDensity", engineConfigPtr->getFogSkyDensity());
            setUniform("fogColor", engineConfigPtr->getFogColor());
            loadedBasics = true;
        }
    }
}