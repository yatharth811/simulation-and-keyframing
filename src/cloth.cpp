#include "cloth.hpp"

Cloth::Cloth(float l, float w, int _m, int _n) {
  length = l;
  width = w;
  m = _m;
  n = _n;
   
  totalVertices = (m + 1) * (n + 1);
  totalTriangles = 2 * m * n;
  mass = 0.2;

  ks[structural] = 2e3;
  ks[shear] = 5e2;
  ks[bending] = 1e2;

  float temp = length / n;
  ls[structural] = 1.0f * temp;
  ls[shear] = sqrtf(2) * temp;
  ls[bending] = 2.0f * temp;

  time = 0.0f;
  cs[structural] = 50.0f;
  cs[shear] = 10.0f;
  cs[bending] = 1.0f;

  for (int i = 0; i <= n; i += 1) {
    for (int j = 0; j <= m; j += 1) {
      glm::vec3 v(i * temp - (l / 2), 0.5f, j * width / m);
      bool isFixed = ((i == n || i == 0) && j == 0);
      particles.push_back(new ClothPoint(v, glm::vec3(0.0f), isFixed, std::make_pair(i, j)));
    }
  }

  triangles = new glm::ivec3[2 * m * n];
  for (int i = 0, cnt = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      uint a = i * (m + 1) + j;
      uint b = (i + 1) * (m + 1) + j;
      uint c = a + 1;
      uint d = b + 1;
      triangles[cnt++] = glm::ivec3(a, c, b);
      triangles[cnt++] = glm::ivec3(c, d, b);
    }
  }

}


std::vector<ClothPoint *> Cloth::structural_neighbours(ClothPoint* particle) {
    std::vector<ClothPoint*> ret;
    int i = particle->index.first, j = particle->index.second;

    if(i - 1 >= 0) {
        ret.push_back(particles[(i-1)*(m+1)+(j)]);
    } 
    
    if(i + 1 <= n) {
        ret.push_back(particles[(i+1)*(m+1)+(j)]);
    }
    
    if(j - 1 >= 0) {
        ret.push_back(particles[(i*(m+1))+(j-1)]);
    }

    if(j + 1 <= m){
        ret.push_back(particles[(i*(m+1))+(j+1)]);
    }

    return ret;
}

std::vector<ClothPoint *> Cloth::shear_neighbours(ClothPoint* particle) {
    std::vector<ClothPoint *> ret;
    int i = particle->index.first, j = particle->index.second;

    if(i - 1 >= 0 && j - 1 >= 0){
        ret.push_back(particles[((i-1)*(m+1))+(j-1)]);
    }

    if(i + 1 <= n && j - 1 >= 0){
        ret.push_back(particles[((i+1)*(m+1))+(j-1)]);
    }

    if(i + 1 <= n && j + 1 <= m){
        ret.push_back(particles[((i+1)*(m+1))+(j+1)]);
    }

    if(i - 1 >= 0 && j + 1 <= m){
        ret.push_back(particles[((i-1)*(m+1))+(j+1)]);
    }

    return ret;
}

std::vector<ClothPoint *> Cloth::bending_neighbours(ClothPoint* particle) {
    std::vector<ClothPoint *> ret;
    int i = particle->index.first, j = particle->index.second;
    
    if(i - 2 >= 0){
        ret.push_back(particles[((i-2)*(m+1))+(j)]);
    }
    
    if(i + 2 <= n){
        ret.push_back(particles[((i+2)*(m+1))+(j)]);
    }
    
    if(j - 2 >= 0){
        ret.push_back(particles[(i*(m+1))+(j-2)]);
    }

    if(j + 2 <= m){
        ret.push_back(particles[(i*(m+1))+(j+2)]);
    }

    return ret;
}


void Cloth::update_points(){
  glm::vec3 force(0.0f);
  float delta = 1e-3;

  glm::vec3 gravity(0.0f,-9.81f,0.0f);
  std::vector<glm::vec3> newPositions(totalVertices);

  for(int i=0; i<totalVertices; i++){
    if(!particles[i]->isFixed){

      glm::vec3 currentPosition = particles[i]->position;
      glm::vec3 currentVelocity = particles[i]->velocity;

      // Gravitational Force
      force += mass*gravity; 

      // Structural Spring Force
      std::vector<ClothPoint*> structural_springs = structural_neighbours(particles[i]);
      for(auto &i : structural_springs){
          glm::vec3 x = i->position-currentPosition;
          glm::vec3 diff_vel = currentVelocity - i->velocity;
          force += glm::normalize(x)*(ks[structural] * (glm::length(x) - ls[structural]));
          force -= cs[structural] * (glm::dot(diff_vel, glm::normalize(x))) * glm::normalize(x);
      }

      // Shear Spring Force
      std::vector<ClothPoint*> shear_springs = shear_neighbours(particles[i]);
      for(auto &i : shear_springs){
          glm::vec3 x = i->position - currentPosition;
          glm::vec3 diff_vel = currentVelocity - i->velocity;
          force += glm::normalize(x) * (ks[shear] * (glm::length(x) - ls[shear]));
          force -= cs[shear] * (glm::dot(diff_vel, glm::normalize(x))) * glm::normalize(x);
      }

      // Bend Spring Force
      std::vector<ClothPoint*> bend_springs = bending_neighbours(particles[i]);
      for(auto &i : bend_springs){
          glm::vec3 x = i->position-currentPosition;
          glm::vec3 diff_vel = currentVelocity - i->velocity;
          force += glm::normalize(x) * (ks[bending] * (glm::length(x) - ls[bending]));
          force -= cs[bending] * (glm::dot(diff_vel, glm::normalize(x))) * glm::normalize(x);
      }

      // Damping Force
      // force -= particles[i]->velocity * (damping_constant);

      // Particles Acceleration
      particles[i]->acceleration = force/mass;
      
      force = glm::vec3(0.0f);

    }
  }

  for(int i=0; i<totalVertices; i++){
    if(!particles[i]->isFixed){
      // Updating Velocity
      particles[i]->velocity = particles[i]->velocity + (particles[i]->acceleration*delta);

      // Updating Position
      particles[i]->position = particles[i]->position + (particles[i]->velocity*delta);
    }
  }

  time += delta;

}

