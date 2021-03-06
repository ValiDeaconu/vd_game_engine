//
// Created by Vali on 10/6/2020.
//

#include "TerrainNode.hpp"

#include <utility>

namespace mod::terrain {
    TerrainNode::TerrainNode(const TerrainNode* parent,
                             const glm::vec2& topLeft,
                             const glm::vec2& bottomRight,
                             WorldCoordinatesConvertor worldCoordinatesConvertor,
                             int maxLevel,
                             const std::vector<int>* lodRangesPtr,
                             int level,
                             NodeIndex nodeIndex)
            : vd::datastruct::Quadtree(parent, level, nodeIndex)
            , m_kTopLeft(topLeft)
            , m_kBottomRight(bottomRight)
            , m_WorldCoordinatesConvertor(std::move(worldCoordinatesConvertor))
            , m_kMaxLevelOfDetail(maxLevel)
            , m_kLevelOfDetailRangesPtr(lodRangesPtr)
            , m_kCenter((topLeft.x + bottomRight.x) / 2.0f, (topLeft.y + bottomRight.y) / 2.0f)
            , m_TessFactor(1.0f)
    {
        assert((bottomRight.x - topLeft.x) == (bottomRight.y - topLeft.y));

        m_Transform.Translation() = glm::vec3(topLeft.x, 0.0f, topLeft.y);
        m_Transform.Scale() = glm::vec3(bottomRight.x - topLeft.x, 1.0f, bottomRight.x - topLeft.x);

        ComputeEdgeMiddles();
        ComputeBounds();
    }

    void TerrainNode::Update(const vd::camera::CameraPtr& cameraPtr) {
        // Set this node as leaf
        this->Clear();

        // If this node's level is greater or equal to the maximum level, then just ignore it
        if (m_Level < m_kMaxLevelOfDetail) {
            const auto& cameraPosition = cameraPtr->Position();

            // Compute if this node exceeds his level of detail threshold
            const auto levelMinRange = float(m_kLevelOfDetailRangesPtr->at(m_Level));

            // Compute distance from each point to get the minimum distance from the patch to the camera position
            float nodeDistance;
            for (int i = 0; i < 4; ++i) {
                float distance = glm::length(cameraPosition - m_EdgeMid[i]);
                nodeDistance = (i == 0) ? distance : std::min(nodeDistance, distance);
            }

            // If the distance is less than the current level minimum range, this node should divide
            if (nodeDistance <= levelMinRange) {
                // Set this node as being a parent, and create its children
                Populate();

                // Update all children
                for (auto& childPtr : m_Children) {
                    auto terrainNodePtr = std::dynamic_pointer_cast<TerrainNode>(childPtr);
                    terrainNodePtr->Update(cameraPtr);
                }
            }
        }
    }

    void TerrainNode::UpdateNeighbours() {
        // Update neighbours only if this node is not a root node (root node neighbours never changes!)
        // Also, there's no point in computing neighbours if this node is not a leaf, because the responsability
        // to check the border is passed to the children
        if (m_kNodeIndex != eRootNode && m_Leaf) {
            ComputeNeighbours();

            // Check if any neighbour has a different level of detail
            // In that case, compute the difference between levels and set tessellation factor on that edge
            // to be equal to 2 to the power of the difference

            // TODO: What happens if the tessellation factor goes over limit (gl_MaxTessLevel)?
            //  Is that possible?
            for (int i = 0; i < 4; ++i) {
                if (m_Neighbours[i] != nullptr && m_Neighbours[i]->Level() >= m_Level) {
                    int count = m_Neighbours[i]->Level() - m_Level;
                    for (const vd::datastruct::Quadtree* r = m_Neighbours[i]; !r->Leaf(); r = r->Child(0).get())
                        ++count;
                    m_TessFactor[i] = float(1 << count);
                }
            }
        } else if (!m_Leaf) {
            // Update all children' neighbours
            for (auto& childPtr : m_Children) {
                auto terrainNodePtr = std::dynamic_pointer_cast<TerrainNode>(childPtr);
                terrainNodePtr->UpdateNeighbours();
            }
        }
    }

    const vd::math::Bounds3& TerrainNode::Bounds() const {
        return m_Bounds;
    }

    const glm::vec2& TerrainNode::TopLeft() const {
        return m_kTopLeft;
    }

    const vd::math::Transform& TerrainNode::Transform() const {
        return m_Transform;
    }

    const TerrainNode::PointVec& TerrainNode::EdgeMiddles() const {
        return m_EdgeMid;
    }

    const glm::vec4& TerrainNode::TessFactors() const {
        return m_TessFactor;
    }

    void TerrainNode::ComputeEdgeMiddles() {
        m_EdgeMid[eTop] = m_WorldCoordinatesConvertor(m_kCenter.x, m_kTopLeft.y);
        m_EdgeMid[eRight] = m_WorldCoordinatesConvertor(m_kBottomRight.x, m_kCenter.y);
        m_EdgeMid[eBottom] = m_WorldCoordinatesConvertor(m_kCenter.x, m_kBottomRight.y);
        m_EdgeMid[eLeft] = m_WorldCoordinatesConvertor(m_kTopLeft.x, m_kCenter.y);
    }

    void TerrainNode::ComputeBounds() {
        glm::mat4 localTransform = m_Transform.Get();

        glm::vec4 localTL = glm::vec4(m_kTopLeft.x, 0.0f, m_kTopLeft.y, 1.0f);
        glm::vec4 localBR = glm::vec4(m_kBottomRight.x, 0.0f, m_kBottomRight.y, 1.0f);

        glm::vec3 worldTL = m_WorldCoordinatesConvertor(localTL.x, localTL.z);
        glm::vec3 worldBR = m_WorldCoordinatesConvertor(localBR.x, localBR.z);

        // glm::vec2 mn(std::min(worldTL.x, worldBR.x), std::min(worldTL.y, worldBR.y));
        // glm::vec2 mx(std::max(worldTL.x, worldBR.x), std::max(worldTL.y, worldBR.y));

        // m_Bounds = vd::math::Bounds2(mn, mx);

        glm::vec3 mn, mx;

        if (worldTL.x >= worldBR.x) {
            mn.x = worldBR.x;
            mx.x = worldTL.x;
        } else {
            mn.x = worldTL.x;
            mx.x = worldBR.x;
        }

        if (worldTL.z >= worldBR.z) {
            mn.z = worldBR.z;
            mx.z = worldTL.z;
        } else {
            mn.z = worldTL.z;
            mx.z = worldBR.z;
        }

        // TODO: Optimization: Implement 2D RMQ to query in O(1) min & max height for this patch
        mn.y = 0.0f;
        mx.y = 600.0f;

        m_Bounds = vd::math::Bounds3(mn, mx);
    }

    void TerrainNode::Populate() {
        if (!m_Leaf) {
            Clear();
        }

        // Set this as parent
        m_Leaf = false;

        // Top Left child
        m_Children[eTopLeft] = std::make_shared<TerrainNode>(this,
                                                             m_kTopLeft,
                                                             m_kCenter,
                                                             m_WorldCoordinatesConvertor,
                                                             m_kMaxLevelOfDetail,
                                                             m_kLevelOfDetailRangesPtr,
                                                             m_Level + 1,
                                                             eTopLeft);

        // Top Right child
        m_Children[eTopRight] = std::make_shared<TerrainNode>(this,
                                                              glm::vec2(m_kCenter.x, m_kTopLeft.y),
                                                              glm::vec2(m_kBottomRight.x, m_kCenter.y),
                                                              m_WorldCoordinatesConvertor,
                                                              m_kMaxLevelOfDetail,
                                                              m_kLevelOfDetailRangesPtr,
                                                              m_Level + 1,
                                                              eTopRight);

        // Bottom Left child
        m_Children[eBottomLeft] = std::make_shared<TerrainNode>(this,
                                                                glm::vec2(m_kTopLeft.x, m_kCenter.y),
                                                                glm::vec2(m_kCenter.x, m_kBottomRight.y),
                                                                m_WorldCoordinatesConvertor,
                                                                m_kMaxLevelOfDetail,
                                                                m_kLevelOfDetailRangesPtr,
                                                                m_Level + 1,
                                                                eBottomLeft);

        // Bottom Right child
        m_Children[eBottomRight] = std::make_shared<TerrainNode>(this,
                                                                 m_kCenter,
                                                                 m_kBottomRight,
                                                                 m_WorldCoordinatesConvertor,
                                                                 m_kMaxLevelOfDetail,
                                                                 m_kLevelOfDetailRangesPtr,
                                                                 m_Level + 1,
                                                                 eBottomRight);
    }

    void TerrainNode::ComputeNeighbours() {
        if (m_kNodeIndex == eRootNode) {
            return;
        }

        const float localEdgeLength = m_kBottomRight.x - m_kTopLeft.x;

        const vd::math::Bounds2 northEstimatedBounds(glm::vec2(m_kTopLeft.x, m_kTopLeft.y - localEdgeLength),
                                                     glm::vec2(m_kBottomRight.x, m_kTopLeft.y));

        const vd::math::Bounds2 eastEstimatedBounds(glm::vec2(m_kBottomRight.x, m_kTopLeft.y),
                                                    glm::vec2(m_kBottomRight.x + localEdgeLength, m_kBottomRight.y));

        const vd::math::Bounds2 southEstimatedBounds(glm::vec2(m_kTopLeft.x, m_kBottomRight.y),
                                                     glm::vec2(m_kBottomRight.x, m_kBottomRight.y  + localEdgeLength));

        const vd::math::Bounds2 westEstimatedBounds(glm::vec2(m_kTopLeft.x - localEdgeLength, m_kTopLeft.y),
                                                    glm::vec2(m_kTopLeft.x, m_kBottomRight.y));

        auto grandfatherPtr = (TerrainNode*) m_kpParent->Parent();

        switch (m_kNodeIndex) {
            case eTopLeft: {
                m_Neighbours[eRight] = (TerrainNode*) m_kpParent->Child(eTopRight).get();
                m_Neighbours[eBottom] = (TerrainNode*) m_kpParent->Child(eBottomLeft).get();

                m_Neighbours[eLeft] = SearchNeighbour(grandfatherPtr,
                                                      westEstimatedBounds,
                                                      true,
                                                      (NodeIndex) m_kpParent->NodeIndex());
                m_Neighbours[eTop] = SearchNeighbour(grandfatherPtr,
                                                     northEstimatedBounds,
                                                     true,
                                                     (NodeIndex) m_kpParent->NodeIndex());
                break;
            }
            case eTopRight: {
                m_Neighbours[eLeft] = (TerrainNode*) m_kpParent->Child(eTopLeft).get();
                m_Neighbours[eBottom] = (TerrainNode*) m_kpParent->Child(eBottomRight).get();

                m_Neighbours[eRight] = SearchNeighbour(grandfatherPtr,
                                                       eastEstimatedBounds,
                                                       true,
                                                       (NodeIndex) m_kpParent->NodeIndex());

                m_Neighbours[eTop] = SearchNeighbour(grandfatherPtr,
                                                     northEstimatedBounds,
                                                     true,
                                                     (NodeIndex) m_kpParent->NodeIndex());
                break;
            }
            case eBottomLeft: {
                m_Neighbours[eRight] = (TerrainNode*) m_kpParent->Child(eBottomRight).get();
                m_Neighbours[eTop] = (TerrainNode*) m_kpParent->Child(eTopLeft).get();

                m_Neighbours[eLeft] = SearchNeighbour(grandfatherPtr,
                                                      westEstimatedBounds,
                                                      true,
                                                      (NodeIndex) m_kpParent->NodeIndex());
                m_Neighbours[eBottom] = SearchNeighbour(grandfatherPtr,
                                                        southEstimatedBounds,
                                                        true,
                                                        (NodeIndex) m_kpParent->NodeIndex());
                break;
            }
            case eBottomRight: {
                m_Neighbours[eLeft] = (TerrainNode*) m_kpParent->Child(eBottomLeft).get();
                m_Neighbours[eTop] = (TerrainNode*) m_kpParent->Child(eTopRight).get();

                m_Neighbours[eRight] = SearchNeighbour(grandfatherPtr,
                                                       eastEstimatedBounds,
                                                       true,
                                                       (NodeIndex) m_kpParent->NodeIndex());
                m_Neighbours[eBottom] = SearchNeighbour(grandfatherPtr,
                                                        southEstimatedBounds,
                                                        true,
                                                        (NodeIndex) m_kpParent->NodeIndex());
                break;
            }
        }
    }

    TerrainNode::MatchingResult TerrainNode::ContainsSquare(const vd::math::Bounds2& bounds) const {
        if (bounds.Empty()) {
            glm::vec2 point = bounds.Left();

            if (point == m_kTopLeft && point == m_kBottomRight) {
                return ePerfectMatch;
            } else if (point.x >= m_kTopLeft.x && point.x <= m_kBottomRight.x &&
                       point.y >= m_kTopLeft.y && point.y <= m_kBottomRight.y) {
                return eInside;
            }

            return eOutside;
        }

        if (!bounds.Valid()) {
            return eOutside;
        }

        if (bounds.Left() == m_kTopLeft && bounds.Right() == m_kBottomRight) {
            return ePerfectMatch;
        }

        if (bounds.Left().x >= m_kTopLeft.x &&
                bounds.Right().x <= m_kBottomRight.x &&
                bounds.Left().y >= m_kTopLeft.y &&
                bounds.Right().y <= m_kBottomRight.y) {
            return eInside;
        }

        return eOutside;
    }

    const TerrainNode* TerrainNode::SearchNeighbour(const TerrainNode* nodePtr,
                                                    const vd::math::Bounds2& searchingBounds,
                                                    bool parentSearch,
                                                    NodeIndex caller) {
        if (nodePtr == nullptr) {
            return nullptr;
        }

        bool skipToParentSearch = false;
        switch (nodePtr->ContainsSquare(searchingBounds)) {
            case eOutside:
                skipToParentSearch = true;
                break;
            case ePerfectMatch: return nodePtr;
            case eInside:
                if (nodePtr->Leaf()) {
                    return nodePtr;
                }
                break;
        }

        if (!skipToParentSearch) {
            for (int nodeIndex = 0; nodeIndex < 4; ++nodeIndex) {
                if (caller == eRootNode || caller != nodeIndex) {
                    const auto &childPtr = std::dynamic_pointer_cast<TerrainNode>(nodePtr->Child(nodeIndex));
                    if (childPtr != nullptr) {
                        auto resultPtr = SearchNeighbour(childPtr.get(), searchingBounds);
                        if (resultPtr != nullptr) {
                            return resultPtr;
                        }
                    }
                }
            }
        }

        if (parentSearch && nodePtr->m_kpParent != nullptr) {
            auto parentPtr = (TerrainNode*) nodePtr->m_kpParent;
            return SearchNeighbour(parentPtr, searchingBounds, true, (NodeIndex) nodePtr->m_kNodeIndex);
        }

        return nullptr;
    }

}