//
// Created by Vali on 9/21/2020.
//

#include "Terrain.hpp"

namespace mod::terrain {

    Terrain::Terrain(const std::string& propsFilePath)
        : m_pProps(vd::loader::PropertiesLoader::Load(propsFilePath))
    {
    }

    Terrain::~Terrain() = default;

    void Terrain::Link() {
        m_pCamera = vd::ObjectOfType<vd::camera::Camera>::Find();
    }

    void Terrain::Init() {
        CreateProps();

        // Create world transform
        const auto scaleXZ = m_pProps->Get<float>("ScaleXZ");
        const auto scaleY = m_pProps->Get<float>("ScaleY");
        WorldTransform().Scale() = glm::vec3(scaleXZ, scaleY, scaleXZ);
        WorldTransform().Translation() = glm::vec3(-scaleXZ / 2.0f, 0.0f, -scaleXZ / 2.0f);

        PopulateBiomes();

        ComputeMaps();

        auto convertToWorldCoords = [&](float x, float y) {
            const auto h = m_pHeightImg->Get<float, vd::math::Interpolation::eBilinear>(glm::vec2(x, y));
            return glm::vec3(WorldTransform() * glm::vec4(x, h, y, 1.0f));
        };

        m_RootNode = std::make_shared<TerrainNode>(nullptr,
                                                   glm::vec2(0.0f, 0.0f),
                                                   glm::vec2(1.0f, 1.0f),
                                                   convertToWorldCoords,
                                                   m_pProps->Get<int>("MaxLevelOfDetail"),
                                                   &m_LevelOfDetailRanges,
                                                   0,
                                                   TerrainNode::eRootNode);

        PopulateTree(m_RootNode);

        GeneratePatch();

        Entity::Init();
    }

    void Terrain::Update() {
        if (m_pProps->Get<bool>("LevelOfDetailEnabled")) {
            if (m_pCamera->CameraMoved() || m_pCamera->CameraRotated()) {
                for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                    imaginaryRootNode->Update(m_pCamera);
                }

                for (const auto& imaginaryRootNode : m_ImaginaryRootNodes) {
                    imaginaryRootNode->UpdateNeighbours();
                }
            }
        }
    }

    void Terrain::CleanUp() {
        m_RootNode->Clear();
        m_ImaginaryRootNodes.clear();

        m_RootNode = nullptr;

        Entity::CleanUp();
    }

    const vd::property::PropertiesPtr& Terrain::Properties() const {
        return m_pProps;
    }

    const std::vector<TerrainNode::ptr_type_t>& Terrain::RootNodes() const {
        return m_ImaginaryRootNodes;
    }

    const BiomePtrVec& Terrain::Biomes() const {
        return m_Biomes;
    }

    const vd::gl::Texture2DPtr& Terrain::HeightMap() const {
        return m_pHeightMap;
    }

    const vd::gl::Texture2DPtr& Terrain::NormalMap() const {
        return m_pNormalMap;
    }

    const vd::gl::Texture2DPtr& Terrain::SplatMap() const {
        return m_pSplatMap;
    }

    float Terrain::HeightAt(float x, float z) const {
        const auto scaleXZ = m_pProps->Get<float>("ScaleXZ");
        const auto scaleY = m_pProps->Get<float>("ScaleY");

        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return 0.0f;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto height = m_pHeightImg->Get<float, vd::math::Interpolation::eBilinear>(glm::vec2(rz, rx));

        return height * scaleY;
    }

    BiomePtrVec Terrain::BiomesAt(float x, float z) const {
        const auto scaleXZ = m_pProps->Get<float>("ScaleXZ");

        float upperBound = scaleXZ / 2.0f;
        float lowerBound = -upperBound;

        BiomePtrVec output;

        if (x < lowerBound || z < lowerBound || x >= upperBound || z >= upperBound)
            return output;

        // reverse transform
        float rx = (x + (scaleXZ / 2.0f)) / scaleXZ;
        float rz = (z + (scaleXZ / 2.0f)) / scaleXZ;

        const auto mask = m_pSplatImg->Get<uint32_t, vd::math::Interpolation::eNearestNeighbour>(glm::vec2(rz, rx));

        for (size_t k = 0; k < m_Biomes.size(); ++k) {
            if ((mask & (1 << k)) != 0) {
                output.emplace_back(m_Biomes[k]);
            }
        }

        return output;
    }

    void Terrain::CreateProps() {
        // Level offset (Jump over imaginary root nodes)
        const auto kRootNodes = m_pProps->Get<int>("RootNodes");
        const int kLevelOffset = int(std::log(kRootNodes) / std::log(4));
        if (!m_pProps->Set("LevelOffset", std::to_string(kLevelOffset))) {
            vd::Logger::terminate("Could not add level offset to the properties", 1);
        }
        // Max Level of detail
        for (int i = 0; ; ++i) {
            try {
                m_LevelOfDetailRanges.emplace_back(m_pProps->Get<int>("Lod." + std::to_string(i) + ".Range"));
            } catch (std::invalid_argument& e) {
                if (!m_pProps->Set("MaxLevelOfDetail", std::to_string(i))) {
                    vd::Logger::terminate("Could not add max level of detail to the properties", 1);
                }
                break;
            }
        }
    }

    void Terrain::PopulateBiomes() {
        const auto scaleY = m_pProps->Get<float>("ScaleY");
        for (int i = 0; ; ++i) {
            try {
                const std::string prefix = "Biome." + std::to_string(i);

                BiomePtr biomePtr = std::make_shared<Biome>();

                biomePtr->Name() = m_pProps->Get<std::string>(prefix + ".Name");
                biomePtr->MinimumHeight() = m_pProps->Get<float>(prefix + ".MinHeight") * scaleY;
                biomePtr->MaximumHeight() = m_pProps->Get<float>(prefix + ".MaxHeight") * scaleY;

                const std::string materialPrefix = prefix + ".Material";
                biomePtr->Material().diffuseMap = vd::gl::TextureService::Get(m_pProps->Get<std::string>(materialPrefix + ".Diffuse"));
                biomePtr->Material().diffuseMap->Bind();
                biomePtr->Material().diffuseMap->TrilinearFilter();
                biomePtr->Material().diffuseMap->Unbind();

                biomePtr->Material().normalMap = vd::gl::TextureService::Get(m_pProps->Get<std::string>(materialPrefix + ".Normal"));
                biomePtr->Material().normalMap->Bind();
                biomePtr->Material().normalMap->BilinearFilter();
                biomePtr->Material().normalMap->Unbind();

                biomePtr->Material().displaceMap = vd::gl::TextureService::Get(m_pProps->Get<std::string>(materialPrefix + ".Displace"));
                biomePtr->Material().displaceMap->Bind();
                biomePtr->Material().displaceMap->BilinearFilter();
                biomePtr->Material().displaceMap->Unbind();

                biomePtr->Material().displaceScale = m_pProps->Get<float>(materialPrefix + ".HeightScaling");
                biomePtr->Material().horizontalScale = m_pProps->Get<float>(materialPrefix + ".HorizontalScaling");

                PopulateBiomeWithObjects(biomePtr, prefix);

                m_Biomes.emplace_back(std::move(biomePtr));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Terrain::ComputeMaps() {
        m_pHeightImg = vd::loader::ImageLoader::Load<float, vd::model::ImageFormat::eR>(m_pProps->Get<std::string>("HeightMap"));

        // TODO: Use TextureService
        m_pHeightMap = std::make_shared<vd::gl::Texture2D>(m_pHeightImg);

        const int size = int(m_pHeightMap->Width());
        const auto strength = m_pProps->Get<float>("NormalStrength");
        const auto scaleY = m_pProps->Get<float>("ScaleY");

        normalmap::NormalMapBuilder NMBuilder;
        NMBuilder.Create(m_pHeightMap, size, strength, m_pNormalMap);

        splatmap::SplatMapBuilder SMBuilder;
        SMBuilder.Create(m_pHeightMap, size, scaleY, m_Biomes, m_pSplatMap, m_pSplatImg);
    }

    void Terrain::PopulateBiomeWithObjects(BiomePtr& biomePtr, const std::string& biomePrefix) {
        for (int i = 0; ; ++i) {
            try {
               const std::string objectPrefix = biomePrefix + ".Object." + std::to_string(i);

               const auto kLocation = m_pProps->Get<std::string>(objectPrefix + ".Location");
               const auto kFile = m_pProps->Get<std::string>(objectPrefix + ".File");
               const auto kScale = m_pProps->Get<float>(objectPrefix + ".Scale");

                sobj::StaticObjectPtr staticObjectPtr = std::make_shared<sobj::StaticObject>(kLocation, kFile);
                staticObjectPtr->WorldTransform().Scale() = glm::vec3(kScale, kScale, kScale);
                staticObjectPtr->Init();

                biomePtr->Objects().emplace_back(std::move(staticObjectPtr));
            } catch (std::invalid_argument& e) {
                break;
            }
        }
    }

    void Terrain::GeneratePatch() {
        vd::model::MeshPtr meshPtr = std::make_shared<vd::model::Mesh>();

        auto& vertices = meshPtr->vertices;
        vertices.resize(16);

        vertices[0] = vd::model::Vertex(glm::vec3(0.0f, 0.0f, 0.0f));
        vertices[1] = vd::model::Vertex(glm::vec3(0.333f, 0.0f, 0.0f));
        vertices[2] = vd::model::Vertex(glm::vec3(0.666f, 0.0f, 0.0f) );
        vertices[3] = vd::model::Vertex(glm::vec3(1.0f, 0.0f, 0.0f) );

        vertices[4] = vd::model::Vertex(glm::vec3(0.0f, 0.0f, 0.333f));
        vertices[5] = vd::model::Vertex(glm::vec3(0.333f, 0.0f, 0.333f));
        vertices[6] = vd::model::Vertex(glm::vec3(0.666f, 0.0f, 0.333f));
        vertices[7] = vd::model::Vertex(glm::vec3(1.0f, 0.0f, 0.333f));

        vertices[8] = vd::model::Vertex(glm::vec3(0.0f, 0.0f, 0.666f));
        vertices[9] = vd::model::Vertex(glm::vec3(0.333f, 0.0f, 0.666f));
        vertices[10] = vd::model::Vertex(glm::vec3(0.666f, 0.0f, 0.666f));
        vertices[11] = vd::model::Vertex(glm::vec3(1.0f, 0.0f, 0.666f));

        vertices[12] = vd::model::Vertex(glm::vec3(0.0f, 0.0f, 1.0f));
        vertices[13] = vd::model::Vertex(glm::vec3(0.333f, 0.0f, 1.0f));
        vertices[14] = vd::model::Vertex(glm::vec3(0.666f, 0.0f, 1.0f));
        vertices[15] = vd::model::Vertex(glm::vec3(1.0f, 0.0f, 1.0f));

        SetBufferGenerationStrategy(ePatch);

        Meshes().push_back(meshPtr);
    }

    void Terrain::PopulateTree(const TerrainNode::ptr_type_t& root) {
        const auto kLevelOffset = m_pProps->Get<int>("LevelOffset");

        if (root->Level() < kLevelOffset) {
            root->Populate();

            for (auto& child : root->Children()) {
                auto terrainNodeChild = std::dynamic_pointer_cast<TerrainNode>(child);
                PopulateTree(terrainNodeChild);
            }
        } else {
            m_ImaginaryRootNodes.emplace_back(std::dynamic_pointer_cast<TerrainNode>(root));
            m_ImaginaryRootNodes.back()->ResetLevel();
        }
    }
}