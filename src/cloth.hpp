#pragma once
#include "cloth_point.hpp"
#include "obstacles.hpp"

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
    float ks[3];  // Spring Constants
    float ls[3];  // Rest Lengths
    float cs[3];  // Damping Constants
    float time;
    
    // Constructor
    Cloth(float l, float w, int _m, int _n);

    std::vector<ClothPoint*> structural_neighbours(ClothPoint* particle);
    std::vector<ClothPoint*> shear_neighbours(ClothPoint* particle);
    std::vector<ClothPoint*> bending_neighbours(ClothPoint* particle);

    // Normal Update
    inline void update_normal();
  
    // Function to update scene with every timestep
    void update_points(float delta, std::vector<obstacle*> &obstacles, bool flag);
    void update_pbd_points(float delta, std::vector<obstacle*> &obstacles, bool flag);

};
