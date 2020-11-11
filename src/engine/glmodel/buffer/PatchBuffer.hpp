//
// Created by Vali on 9/21/2020.
//

#ifndef VD_GAME_ENGINE_PATCHBUFFER_HPP
#define VD_GAME_ENGINE_PATCHBUFFER_HPP

#include <memory>

#include "Buffer.hpp"

namespace vd::buffer {
    class PatchBuffer : public Buffer {
    public:
        PatchBuffer();
        ~PatchBuffer();

        void Allocate(const vd::model::MeshPtr& meshPtr) override;
        void Render() override;
        void CleanUp() override;
    private:
        GLuint m_VaoId;
        GLuint m_VboId;
        size_t m_Size;

        vd::model::MeshPtr meshPtr;
    };
    typedef std::shared_ptr<PatchBuffer>	PatchBufferPtr;
}


#endif //VD_GAME_ENGINE_PATCHBUFFER_HPP
