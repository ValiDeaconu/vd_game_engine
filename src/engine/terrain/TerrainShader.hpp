//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_TERRAINSHADER_HPP
#define VD_GAME_ENGINE_TERRAINSHADER_HPP

#include <engine/shader/Shader.hpp>

#include <memory>

#include "Terrain.hpp"

namespace vd::terrain {
    class TerrainShader : public shader::Shader {
    public:
        TerrainShader();
        ~TerrainShader();

        void updateUniforms(vd::object::EntityPtr entityPtr, size_t meshIndex) override;
    private:
        const int kBiomeCount;
    };
    typedef std::shared_ptr<TerrainShader>  TerrainShaderPtr;
}


#endif //VD_GAME_ENGINE_TERRAINSHADER_HPP