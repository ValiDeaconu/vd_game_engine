//
// Created by Vali on 8/29/2020.
//

#include "GuiShader.hpp"

namespace mod::gui {

    GuiShader::GuiShader() : vd::component::IEntity2DShader() {
        std::string vsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/gui/gui_VS.glsl", vsSource);
        AddShader(vsSource, vd::gl::Shader::eVertexShader);

        std::string fsSource;
        vd::loader::ShaderLoader::Load("./resources/shaders/gui/gui_FS.glsl", fsSource);
        AddShader(fsSource, vd::gl::Shader::eFragmentShader);

        Compile();
    }

    GuiShader::~GuiShader() = default;

    void GuiShader::AddUniforms() {
        AddUniform("transform");
        AddUniform("guiTexture");
    }

    void GuiShader::InitUniforms(vd::object::Entity2DPtr pEntity) {
        AddUniforms();
    }

    void GuiShader::UpdateUniforms(vd::object::Entity2DPtr pEntity, uint32_t meshIndex) {
        SetUniform("transform", pEntity->LocalTransform().Get());

        vd::gl::ActiveTexture(1);
        pEntity->Meshes()[meshIndex]->Materials()[0].DiffuseMap()->Bind();
        SetUniform("guiTexture", 1);
    }
}
