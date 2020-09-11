//
// Created by Vali on 8/26/2020.
//

#include "ShadowShader.hpp"

namespace vd::shadow {
    ShadowShader::ShadowShader() : shader::Shader() {
        loadAndAddShader("./resources/shaders/shadow/shadow_VS.glsl", vd::shader::eVertexShader);
        loadAndAddShader("./resources/shaders/shadow/shadow_FS.glsl", vd::shader::eFragmentShader);
        compileShader();

        addUniform("model");
        addUniform("view");
        addUniform("projection");

        addUniform("diffuseMap");
    }

    ShadowShader::~ShadowShader() = default;

    void ShadowShader::updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) {
        setUniform("model", entityPtr->getWorldTransform().get());

        auto shadowManagerPtr = entityPtr->getParentEngine()->getShadowManager();
        setUniform("view", shadowManagerPtr->getViewMatrix());
        setUniform("projection", shadowManagerPtr->getProjectionMatrix());

        vd::model::MeshPtr& meshPtr = entityPtr->getMeshes()[meshIndex];

        if (!meshPtr->materials.empty())
        {
            vd::model::Material& meshMaterial = meshPtr->materials.front();

            if (meshMaterial.diffusemap != nullptr)
            {
                vd::model::activeTexture(0);
                meshMaterial.diffusemap->bind();
                setUniformi("diffuseMap", 0);
            }
        }
    }

    ShadowShaderPtr& GetShadowShader() {
        static ShadowShaderPtr shadowShaderPtr = std::make_shared<ShadowShader>();

        return shadowShaderPtr;
    }
}