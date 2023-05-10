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
  cs[structural] = 50.0f*4;
  cs[shear] = 10.0f*4;
  cs[bending] = 1.0f*4;

  for (int i = 0; i <= n; i += 1) {
    for (int j = 0; j <= m; j += 1) {
      glm::vec3 v(i * temp, 0.0f, j * width / m);
      // bool isFixed = ((i == n || i == 0) && j == 0);
      bool isFixed = false;
      particles.push_back(new ClothPoint(mass, v, glm::vec3(0.0f), isFixed, std::make_pair(i, j)));
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

    if(i + 1 <= n) {
        ret.push_back(particles[(i+1)*(m+1)+(j)]);
    }

    if(j - 1 >= 0) {
        ret.push_back(particles[(i*(m+1))+(j-1)]);
    }

    if(i - 1 >= 0) {
        ret.push_back(particles[(i-1)*(m+1)+(j)]);
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


void Cloth::update_normal(){

  for(int i=0; i<totalVertices; i++){
    glm::vec3 tmp(0.0f), n0 = particles[i]->normal;
    int cnt = 0;
    auto neighbours = structural_neighbours(particles[i]);
    for(int j=0; j<neighbours.size()-1; j++){
      glm::vec3 n1 = neighbours[j]->normal, n2 = neighbours[j+1]->normal;
      tmp += glm::cross(n1-n0, n2-n0);
      cnt++;
    }
    particles[i]->normal = glm::normalize((1.0f/cnt)*tmp);
  }

}


void Cloth::update_points(float delta, std::vector<obstacle*> &obstacles, bool flag){
  glm::vec3 force(0.0f);

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
      // Old Position
      particles[i]->old_pos = particles[i] -> position;

      // Updating Velocity
      particles[i]->velocity = 0.997f*particles[i]->velocity + (particles[i]->acceleration*delta);

      // Updating Position
      particles[i]->position = particles[i]->position + (particles[i]->velocity*delta);

      if(flag){
        for(auto &k : obstacles){
          k->handle_collision(particles[i]);
        }
      }

    }
  }

  if(flag){
    // Updating Normals
    update_normal();
  }

  time += delta;

}

void Cloth::update_pbd_points(float delta, std::vector<obstacle*> &obstacles, bool flag) {
  std::set<std::pair<ClothPoint *,ClothPoint *>> constraints;
  
  // Structural Constraints
  for(int i=0; i<totalVertices; i++){
    std::vector<ClothPoint*> structural_springs = structural_neighbours(particles[i]);
    for(auto &j : structural_springs){
      std::pair<ClothPoint*,ClothPoint*> p1(j, particles[i]), p2(particles[i], j);
      if(constraints.find(p1) == constraints.end() && constraints.find(p2) == constraints.end()){
        constraints.insert(p1);
      }
    }
  }

  // Updating all velocities and position according to ext forces
  update_points(delta,obstacles,false);

  // Projecting Constraints
  for(int i=0; i<3; i++){
    for(auto &i : constraints){
      ClothPoint *p1 = i.first, *p2 = i.second;
      if(p1->isFixed && p2->isFixed){
        continue;
      }
      else if(p1->isFixed){
        glm::vec3 dp2 = (1 - (ls[structural]/glm::length(p1->position-p2->position)))*(p1->position-p2->position);
        p2->position += dp2;
      }
      else if(p2->isFixed){
        glm::vec3 dp1 = (-1 + (ls[structural]/glm::length(p1->position-p2->position)))*(p1->position-p2->position);
        p1->position += dp1;
      }
      else{
        glm::vec3 dp1 = (-1 + (ls[structural]/glm::length(p1->position-p2->position)))*(p1->position-p2->position);
        dp1 *= 0.5;
        glm::vec3 dp2 = (1 - (ls[structural]/glm::length(p1->position-p2->position)))*(p1->position-p2->position);
        dp2 *= 0.5;
        p1->position += dp1;
        p2->position += dp2;
      }
    }
  }

  // Changing Velocities
  for(int i=0; i<totalVertices; i++){
    if(!particles[i]->isFixed){
      // Updating Velocity
      particles[i]->velocity = (particles[i]->position - particles[i]->old_pos) / delta;
      if(flag){
        for(auto &k : obstacles){
          k->handle_collision(particles[i]);
        }
      }
    }
  }

  if(flag){

    // Updating Normals
    update_normal();

  }


}

