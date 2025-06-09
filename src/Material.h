#ifndef C_MATERIAL_H
#define C_MATERIAL_H

#include <string>

class Material {
public:
    Material(const std::string& name) : name(name) {}

    //void SetShader(Shader* shader) {
    //    this->shader = shader;
    //}
//
    //Shader* GetShader() const {
    //    return shader;
    //}
//
    //const std::string& GetName() const {
    //    return name;
    //}

    std::string name;
};

#endif