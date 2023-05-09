#pragma once
#include "cloth_point.hpp"
enum springConstants {
  structural,
  shear,
  bending,
};

class Cloth {
  public:
    float length, width;
    int m, n;
    int totalVertices, totalTriangles;
    std::vector<ClothPoint*> particles;
    glm::ivec3* triangles;
    float mass;
    float ks[3];
    float ls[3];
    float cs[3];
    float time;
    
    // Constructor
    Cloth(float l, float w, int _m, int _n);

    std::vector<ClothPoint*> structural_neighbours(ClothPoint* particle);
    std::vector<ClothPoint*> shear_neighbours(ClothPoint* particle);
    std::vector<ClothPoint*> bending_neighbours(ClothPoint* particle);

    void update_points();
    void update_pbd_points();

};
