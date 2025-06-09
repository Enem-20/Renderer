#ifndef S_LOD_H
#define S_LOD_H

#include <cstddef>
#include <tsl/hopscotch_map.h>

class Mesh;

struct LOD {
    size_t level;
    float_t distance;
    Mesh* mesh;
    
    LOD(size_t level, float_t distance, Mesh* mesh) : level(level), distance(distance), mesh(mesh) {}
};

#endif