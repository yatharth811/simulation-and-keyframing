#include "obstacles.hpp"

#define ERR 0.02f

bool Sphere::detect_collision(ClothPoint *p) const{
    if(glm::length(p->position-center)<radius+ERR){
        return true;
    }
    return false;
}

void Sphere::handle_collision(ClothPoint *p) const{
    if(detect_collision(p)){
        glm::vec3 normal = glm::normalize(p->position-center);
        if(glm::dot(p->velocity,normal)<0){
            glm::vec3 v_normal = glm::dot(p->velocity,normal)*normal;
            glm::vec3 v_tan = p->velocity - v_normal;
            glm::vec3 imp_normal = -1*(1+eps)*p->mass*v_normal;
            glm::vec3 imp_tan; 
            if(glm::length(v_tan)>0.01f){
                imp_tan = -1*std::min(mu*glm::length(imp_normal), p->mass*glm::length(v_tan))*glm::normalize(v_tan);
            }
            else{
                imp_tan = glm::vec3(0.0f);
            }
            p->velocity += (1.0f/p->mass)*(imp_normal+imp_tan);
        }
        glm::vec3 dx = (ERR+radius-glm::length(p->position-center))*normal;
        p->position += dx;
    }
}

bool Plane::detect_collision(ClothPoint *p) const{
    if(glm::dot(p->position-ppoint,normal)<ERR){
        return true;
    }
    return false;
}

void Plane::handle_collision(ClothPoint *p) const{
    if(detect_collision(p)){
        if(glm::dot(p->velocity,normal)<0){
            glm::vec3 v_normal = glm::dot(p->velocity,normal)*normal;
            glm::vec3 v_tan = p->velocity - v_normal;
            glm::vec3 imp_normal = -1*(1+eps)*p->mass*v_normal;
            glm::vec3 imp_tan; 
            if(glm::length(v_tan)>0.01f){
                imp_tan = -1*std::min(mu*glm::length(imp_normal), p->mass*glm::length(v_tan))*glm::normalize(v_tan);
            }
            else{
                imp_tan = glm::vec3(0.0f);
            }
            p->velocity += (1.0f/p->mass)*(imp_normal+imp_tan);
        }
        glm::vec3 dx = (ERR-glm::dot(p->position-ppoint,normal))*normal;
        p->position += dx;
    }
}