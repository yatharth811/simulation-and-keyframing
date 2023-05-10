#pragma once
#include "glm/glm.hpp"
#include "vector"
#include "set"

class ClothPoint{
  public:
    float mass;
    glm::vec3 old_pos;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool isFixed;
    std::pair<int, int> index;

    ClothPoint(float _mass, const glm::vec3 &pos, const glm::vec3 &vel, bool flag, const std::pair<int, int> &ind) {
      mass = _mass;
      position = pos;
      normal = glm::vec3(0.0f);
      velocity = vel;
      acceleration = glm::vec3(0.0f);
      isFixed = flag;
      index = ind;
    }
};
