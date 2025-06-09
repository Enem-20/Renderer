#ifndef C_RENDERABLE_H
#define C_RENDERABLE_H

#include <vector>

#include <glm/glm.hpp>

#include "LOD.h"

class Mesh;
class Material;

class Renderable {
public:
    Renderable(Mesh* mesh, Material* material, glm::mat4 transform) 
        : lodMeshes({LOD(0, 0.0f, mesh)})
        , material(material)

    {}

    void AddLOD(size_t level, float distance, Mesh* mesh) {
        lodMeshes.emplace_back(level, distance, mesh);
        std::sort(lodMeshes.begin(), lodMeshes.end(), [](const LOD& a, const LOD& b) {
            return a.distance < b.distance;
        });
    }

    void SetInstancing(bool enabled) {
        instancingEnabled = enabled;
    }

    void AddInstanceTransform(const glm::mat4& transform) {
        if (!instancingEnabled) return;
        transformInstances.push_back(transform);
    }

    void Update(const glm::vec3& cameraPosition) {
        currentLODMesh = SelectLOD(cameraPosition);
    }

    Mesh* GetMesh() const {
        return currentLODMesh ? currentLODMesh : (lodMeshes.empty() ? nullptr : lodMeshes.front().mesh);
    }

    const std::vector<glm::mat4>& GetTransforms() const {
        return transformInstances;
    }

    Material* GetMaterial() const {
        return material;
    }
private:
    bool instancingEnabled = false;

    Mesh* currentLODMesh = nullptr;

    std::vector<LOD> lodMeshes;
    std::vector<glm::mat4> transformInstances;
    Material* material = nullptr;

    Mesh* SelectLOD(const glm::vec3& cameraPosition) const {
        if (lodMeshes.empty() || transformInstances.empty())
            return nullptr;

        float distance = glm::length(glm::vec3(transformInstances[0][3]) - cameraPosition);

        for (const auto& lod : lodMeshes) {
            if (distance < lod.distance)
                return lod.mesh;
        }

        return lodMeshes.back().mesh;
    }
};

#endif //C_RENDERABLE_H