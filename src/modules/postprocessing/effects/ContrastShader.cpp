//
// Created by Vali on 2/19/2021.
//

#include "ContrastShader.hpp"

namespace mod::postprocessing {

    ContrastShader::ContrastShader() {
        Create();

        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/pp_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/postprocessing/contrast_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    void ContrastShader::AddUniforms() {
        AddUniform("colorMap");
    }

    void ContrastShader::InitUniforms(vd::component::IRenderingEffectPtr pEffect) {
        AddUniforms();
    }

    void ContrastShader::UpdateUniforms(vd::component::IRenderingEffectPtr pEffect) {
        auto pContrast = std::dynamic_pointer_cast<vd::component::ConcreteEffect>(pEffect);

        pContrast->InputFrameBuffer()->ColorTexture()->BindToUnit(0);
        SetUniform("colorMap", 0);
    }


}

