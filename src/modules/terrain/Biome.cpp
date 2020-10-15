//
// Created by Vali on 9/22/2020.
//

#include "Biome.hpp"

namespace mod::terrain {

    Biome::Biome()
        : minHeight(0.0f)
        , maxHeight(0.0f)
        , name()
        , material()
    {
    }

    Biome::Biome(const std::string& name)
        : minHeight(0.0f)
        , maxHeight(0.0f)
        , name(name)
        , material()
    {
    }

    Biome::~Biome() = default;

    const std::string &Biome::getName() const {
        return name;
    }

    void Biome::setName(const std::string& name) {
        this->name = name;
    }

    float Biome::getMinHeight() const {
        return minHeight;
    }

    void Biome::setMinHeight(float minHeight) {
        this->minHeight = minHeight;
    }

    float Biome::getMaxHeight() const {
        return maxHeight;
    }

    void Biome::setMaxHeight(float maxHeight) {
        this->maxHeight = maxHeight;
    }

    vd::model::Material& Biome::getMaterial() {
        return material;
    }

    const vd::model::Material& Biome::getMaterial() const {
        return material;
    }

    void Biome::setMaterial(const vd::model::Material& material) {
        this->material = material;
    }

    std::vector<sobj::StaticObjectPtr> &Biome::getObjects() {
        return objects;
    }

    void Biome::addObject(const sobj::StaticObjectPtr &object) {
        objects.emplace_back(object);
    }
}