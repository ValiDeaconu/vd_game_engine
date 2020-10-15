//
// Created by Vali on 9/21/2020.
//

#include "SplatMapRenderer.hpp"

namespace mod::terrain::splatmap {

    SplatMapRenderer::SplatMapRenderer(int size)
            : size(size)
            , kBiomeCount(5)
    {
        shaderPtr = std::make_shared<SplatMapShader>();
        shaderPtr->addUniform("heightMap");
        shaderPtr->addUniform("size");
        shaderPtr->addUniform("scaleY");

        for (int i = 0; i < kBiomeCount; ++i) {
            shaderPtr->addUniform("biomes[" + std::to_string(i) + "].minHeight");
            shaderPtr->addUniform("biomes[" + std::to_string(i) + "].maxHeight");
        }

        splatMap = std::make_shared<vd::model::Texture2D>(size, size);

        splatMap->generate();
        splatMap->bind();
        splatMap->bilinearFilter();

        glTexStorage2D(GL_TEXTURE_2D, int(std::log(size) / std::log(2)), GL_R32UI, size, size);

        splatMap->unbind();
    }

    SplatMapRenderer::~SplatMapRenderer() = default;

    void SplatMapRenderer::render(const vd::model::Texture2DPtr& normalMap, float scaleY, const BiomePtrVec& biomes) {
        shaderPtr->bind();

        vd::model::activeTexture(0);
        normalMap->bind();
        shaderPtr->setUniformi("normalMap", 0);
        shaderPtr->setUniformi("size", size);
        shaderPtr->setUniformf("scaleY", scaleY);

        for (int i = 0; i < kBiomeCount; ++i) {
            const auto& biomePtr = biomes[i];
            shaderPtr->setUniformf("biomes[" + std::to_string(i) + "].minHeight", biomePtr->getMinHeight());
            shaderPtr->setUniformf("biomes[" + std::to_string(i) + "].maxHeight", biomePtr->getMaxHeight());
        }

        glBindImageTexture(0, splatMap->getId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);
        glDispatchCompute(size/16, size/16, 1);
        glFinish();

        normalMap->unbind();

        splatMap->bind();
        splatMap->bilinearFilter();

        std::vector<uint32_t> outBuffer(size * size);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &outBuffer[0]);

        if (std::any_of(outBuffer.begin(), outBuffer.end(), [](auto x) { return x == 2; })) {
            int a = 0;
            a++;
        }

        splatImg = std::make_shared<vd::img::ImageI>(size, size);
        for (auto r : outBuffer) {
            splatImg->expand(vd::img::Pixel<uint32_t>(r, 0, 0, 0));
        }

        splatMap->unbind();
    }

    const vd::model::Texture2DPtr& SplatMapRenderer::getSplatMap() const {
        return splatMap;
    }

    const vd::img::ImageIPtr &SplatMapRenderer::getSplatData() const {
        return splatImg;
    }
}