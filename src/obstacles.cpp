#include "obstacles.hpp"

#define ERR 0.02f

void obstacle::update_pos(float time){
    glm::vec3 v = time*velocity;
    for(int i=0; i<vertices.size(); i++){
        vertices[i] += v;
    }
    center += v;
}

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

Cylinder::Cylinder(){
    radius = 0;
    center = glm::vec3(0);
    length = 0;
    nLat = 0;
    nLong = 0;

    uint nv = (nLat + 1) * nLong + 2;
    uint nn = nv;
    uint nt = nLat * nLong * 2 + nLong * 2;

    vertices.resize(nv);
    normals.resize(nn);
    triangles.resize(nt);

    float lat_angle = 2 * M_PI / nLong;
    float spacing = length / nLat;

    for (size_t i = 0; i < nLat + 1; i++) {
        for (size_t j = 0; j < nLong; j++) {
        float x = spacing * i;
        float y = radius * cos(lat_angle * j);
        float z = radius * sin(lat_angle * j);
        vertices[i * nLong + j] =
            glm::vec3(x, y, z) - glm::vec3(length / 2, 0, 0) + center;
        normals[i * nLong + j] = glm::vec3(0, y, z) / radius;
        }
    }

    vertices[nv - 2] = glm::vec3(-length / 2, 0, 0) + center;
    vertices[nv - 1] = glm::vec3(length / 2, 0, 0) + center;
    normals[nv - 2] = glm::vec3(-1, 0, 0);
    normals[nv - 1] = glm::vec3(1, 0, 0);

    for (size_t i = 0; i < nLat; i++) {
        for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (i * nLong + j)] = glm::ivec3(
            i * nLong + j, i * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        triangles[2 * (i * nLong + j) + 1] =
            glm::ivec3(i * nLong + (j + 1) % nLong,
                        (i + 1) * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        }
    }

    for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (nLat * nLong + j)] =
            glm::ivec3(nLat * nLong + j, nLat * nLong + (j + 1) % nLong, nv - 1);
        triangles[2 * (nLat * nLong + j) + 1] =
            glm::ivec3(j, nv - 2, (j + 1) % nLong);
    }

}

Cylinder::Cylinder(const glm::vec3 &c, float _r, float _l, uint _n1, uint _n2){
    radius = _r;
    center = c;
    length = _l;
    nLat = _n1;
    nLong = _n2;

    uint nv = (nLat + 1) * nLong + 2;
    uint nn = nv;
    uint nt = nLat * nLong * 2 + nLong * 2;

    vertices.resize(nv);
    normals.resize(nn);
    triangles.resize(nt);

    float lat_angle = 2 * M_PI / nLong;
    float spacing = length / nLat;

    for (size_t i = 0; i < nLat + 1; i++) {
        for (size_t j = 0; j < nLong; j++) {
        float x = spacing * i;
        float y = radius * cos(lat_angle * j);
        float z = radius * sin(lat_angle * j);
        vertices[i * nLong + j] =
            glm::vec3(x, y, z) - glm::vec3(length / 2, 0, 0) + center;
        normals[i * nLong + j] = glm::vec3(0, y, z) / radius;
        }
    }

    vertices[nv - 2] = glm::vec3(-length / 2, 0, 0) + center;
    vertices[nv - 1] = glm::vec3(length / 2, 0, 0) + center;
    normals[nv - 2] = glm::vec3(-1, 0, 0);
    normals[nv - 1] = glm::vec3(1, 0, 0);

    for (size_t i = 0; i < nLat; i++) {
        for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (i * nLong + j)] = glm::ivec3(
            i * nLong + j, i * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        triangles[2 * (i * nLong + j) + 1] =
            glm::ivec3(i * nLong + (j + 1) % nLong,
                        (i + 1) * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        }
    }

    for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (nLat * nLong + j)] =
            glm::ivec3(nLat * nLong + j, nLat * nLong + (j + 1) % nLong, nv - 1);
        triangles[2 * (nLat * nLong + j) + 1] =
            glm::ivec3(j, nv - 2, (j + 1) % nLong);
    }

}

bool Cylinder::detect_collision(ClothPoint *p) const{
    glm::vec3 bottom = vertices[vertices.size() - 2];
    glm::vec3 top = vertices[vertices.size() - 1];
    glm::vec3 axis = glm::normalize(top - bottom);

    float distAlongAxis = glm::dot(p->position - bottom, axis);
    float distFromAxis = glm::length(p->position - (bottom + distAlongAxis * axis));

    if (distAlongAxis < -ERR || distAlongAxis > length + ERR || distFromAxis > radius + ERR) {
        return false;
    }
    return true;
}

void Cylinder::handle_collision(ClothPoint *p) const{

    if(detect_collision(p)){
        glm::vec3 bottom = vertices[vertices.size() - 2];
        glm::vec3 top = vertices[vertices.size() - 1];
        glm::vec3 axis = glm::normalize(top - bottom);

        float distAlongAxis = glm::dot(p->position - bottom, axis);
        float distFromAxis = glm::length(p->position - (bottom + distAlongAxis * axis));

        glm::vec3 normal = glm::normalize(p->position - (bottom + distAlongAxis * axis));

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
        glm::vec3 dx = (ERR+radius-glm::length(p->position-(bottom + distAlongAxis * axis)))*normal;
        p->position += dx;

    }

}


XCylinder::XCylinder(const glm::vec3 &c, float _r, float _l, uint _n1, uint _n2){
    radius = _r;
    center = c;
    length = _l;
    nLat = _n1;
    nLong = _n2;

    uint nv = (nLat + 1) * nLong + 2;
    uint nn = nv;
    uint nt = nLat * nLong * 2 + nLong * 2;

    vertices.resize(nv);
    normals.resize(nn);
    triangles.resize(nt);

    float lat_angle = 2 * M_PI / nLong;
    float spacing = length / nLat;

    for (size_t i = 0; i < nLat + 1; i++) {
        for (size_t j = 0; j < nLong; j++) {
        float x = spacing * i;
        float y = radius * cos(lat_angle * j);
        float z = radius * sin(lat_angle * j);
        vertices[i * nLong + j] =
            glm::vec3(x, y, z) - glm::vec3(length / 2, 0, 0) + center;
        normals[i * nLong + j] = glm::vec3(0, y, z) / radius;
        }
    }

    vertices[nv - 2] = glm::vec3(-length / 2, 0, 0) + center;
    vertices[nv - 1] = glm::vec3(length / 2, 0, 0) + center;
    normals[nv - 2] = glm::vec3(-1, 0, 0);
    normals[nv - 1] = glm::vec3(1, 0, 0);

    for (size_t i = 0; i < nLat; i++) {
        for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (i * nLong + j)] = glm::ivec3(
            i * nLong + j, i * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        triangles[2 * (i * nLong + j) + 1] =
            glm::ivec3(i * nLong + (j + 1) % nLong,
                        (i + 1) * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        }
    }

    for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (nLat * nLong + j)] =
            glm::ivec3(nLat * nLong + j, nLat * nLong + (j + 1) % nLong, nv - 1);
        triangles[2 * (nLat * nLong + j) + 1] =
            glm::ivec3(j, nv - 2, (j + 1) % nLong);
    }

}

YCylinder::YCylinder(const glm::vec3 &c, float _r, float _l, uint _n1, uint _n2){
    radius = _r;
    center = c;
    length = _l;
    nLat = _n1;
    nLong = _n2;

    uint nv = (nLat + 1) * nLong + 2;
    uint nn = nv;
    uint nt = nLat * nLong * 2 + nLong * 2;

    vertices.resize(nv);
    normals.resize(nn);
    triangles.resize(nt);

    float lat_angle = 2 * M_PI / nLong;
    float spacing = length / nLat;

    for (size_t i = 0; i < nLat + 1; i++) {
        for (size_t j = 0; j < nLong; j++) {
        float x = radius * cos(lat_angle * j);
        float y = spacing * i;
        float z = radius * sin(lat_angle * j);
        vertices[i * nLong + j] =
            glm::vec3(x, y, z) - glm::vec3(0, length/2, 0) + center;
        normals[i * nLong + j] = glm::vec3(x, 0, z) / radius;
        }
    }

    vertices[nv - 2] = glm::vec3(0, -length / 2, 0) + center;
    vertices[nv - 1] = glm::vec3(0, length / 2, 0) + center;
    normals[nv - 2] = glm::vec3(0, -1, 0);
    normals[nv - 1] = glm::vec3(0, 1, 0);

    for (size_t i = 0; i < nLat; i++) {
        for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (i * nLong + j)] = glm::ivec3(
            i * nLong + j, i * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        triangles[2 * (i * nLong + j) + 1] =
            glm::ivec3(i * nLong + (j + 1) % nLong,
                        (i + 1) * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        }
    }

    for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (nLat * nLong + j)] =
            glm::ivec3(nLat * nLong + j, nLat * nLong + (j + 1) % nLong, nv - 1);
        triangles[2 * (nLat * nLong + j) + 1] =
            glm::ivec3(j, nv - 2, (j + 1) % nLong);
    }

}

ZCylinder::ZCylinder(const glm::vec3 &c, float _r, float _l, uint _n1, uint _n2){
    radius = _r;
    center = c;
    length = _l;
    nLat = _n1;
    nLong = _n2;

    uint nv = (nLat + 1) * nLong + 2;
    uint nn = nv;
    uint nt = nLat * nLong * 2 + nLong * 2;

    vertices.resize(nv);
    normals.resize(nn);
    triangles.resize(nt);

    float lat_angle = 2 * M_PI / nLong;
    float spacing = length / nLat;

    for (size_t i = 0; i < nLat + 1; i++) {
        for (size_t j = 0; j < nLong; j++) {
        float x = radius * cos(lat_angle * j);
        float y = radius * sin(lat_angle * j);
        float z = spacing * i;
        vertices[i * nLong + j] =
            glm::vec3(x, y, z) - glm::vec3(0, 0, length/2) + center;
        normals[i * nLong + j] = glm::vec3(x, y, 0) / radius;
        }
    }

    vertices[nv - 2] = glm::vec3( 0, 0, -length / 2) + center;
    vertices[nv - 1] = glm::vec3( 0, 0, length / 2) + center;
    normals[nv - 2] = glm::vec3(0, 0, -1);
    normals[nv - 1] = glm::vec3(0, 0, 1);

    for (size_t i = 0; i < nLat; i++) {
        for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (i * nLong + j)] = glm::ivec3(
            i * nLong + j, i * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        triangles[2 * (i * nLong + j) + 1] =
            glm::ivec3(i * nLong + (j + 1) % nLong,
                        (i + 1) * nLong + (j + 1) % nLong, (i + 1) * nLong + j);
        }
    }

    for (size_t j = 0; j < nLong; j++) {
        triangles[2 * (nLat * nLong + j)] =
            glm::ivec3(nLat * nLong + j, nLat * nLong + (j + 1) % nLong, nv - 1);
        triangles[2 * (nLat * nLong + j) + 1] =
            glm::ivec3(j, nv - 2, (j + 1) % nLong);
    }

}