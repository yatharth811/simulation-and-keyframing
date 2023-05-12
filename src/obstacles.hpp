#pragma once
#include<bits/stdc++.h>
#include "glm/glm.hpp"
#include "cloth_point.hpp"


class obstacle{
  public:

    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::ivec3> triangles;
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 center = glm::vec3(0.0f);

    
    void set_velocity(const glm::vec3 &vel){
        velocity = vel;
    }
    void update_pos(float time);
    virtual bool detect_collision(ClothPoint *p) const = 0;
    virtual void handle_collision(ClothPoint *p) const = 0;

    // virtual ~obstacle() {}

};

class Sphere: public obstacle{
  public:
    int m,n;
    float radius;
    float eps = 0.5, mu = 0.3;
    

    // virtual void update_pos(float time) const override;
    virtual bool detect_collision(ClothPoint *p) const override;
    virtual void handle_collision(ClothPoint *p) const override;

    Sphere(int _m, int _n, float r, const glm::vec3 &c){
        radius = r;
        center = c;
        m = _m;
        n = _n;

        float sliceStep = M_PI/m, stackStep = 2*M_PI/n;

        // Creating Vertices
        for(int i=0; i<=m; i++){
            for(int j=0; j<n; j++){
                float x,y,z,phi,theta;
                phi = M_PI/2 - i*sliceStep;
                theta = j*stackStep;
                x = cos(phi)*cos(theta);
                y = cos(phi)*sin(theta);
                z = sin(phi);
                // std::cout << x << " " << y << " " << z << std::endl;
                glm::vec3 v(x,y,z);
                vertices.push_back(radius*v+center);
                normals.push_back(v);
            }
        }

        // Creating Faces
        for(int j=0; j<n; j++){
            uint a = n+j, b = a+1;
            if(j==n-1){
                triangles.push_back(glm::ivec3(b-n,a,0));    
            }
            else{
                triangles.push_back(glm::ivec3(b,a,0));    
            }

        }

        for(int i=1; i<m-1; i++){
            for(int j=0; j<n; j++){
                uint a = i*(n)+j, b = a+1, c = (i+1)*n+j, d = c+1;
                if(j==n-1){
                    b = i*n;
                    d = (i+1)*n;
                }
                triangles.push_back(glm::ivec3(b,d,c));    
                triangles.push_back(glm::ivec3(b,c,a));
            }
        }

        for(int j=0; j<n; j++){
            uint a = (m-1)*n+j, b = a+1, c = m*n;
            if(j==n-1){
                triangles.push_back(glm::ivec3(c,a,b-n));    
            }
            else{
                triangles.push_back(glm::ivec3(c,a,b));    
            }
        }

    }
};

class Plane : public obstacle{
  public:
    glm::vec3 ppoint,normal;

    float eps = 0.25f, mu = 0.3;

    virtual bool detect_collision(ClothPoint *p) const override;
    virtual void handle_collision(ClothPoint *p) const override;

    Plane(){
        vertices = std::vector<glm::vec3>{glm::vec3(-10,-1.2,-10), glm::vec3(10, -1.2, -10), glm::vec3(-10, -1.2, 10), glm::vec3(10,-1.2,10)};
        normals = std::vector<glm::vec3>{glm::vec3(0,1,0), glm::vec3(0,1,0), glm::vec3(0,1,0), glm::vec3(0,1,0) };
        triangles = std::vector<glm::ivec3>{glm::ivec3(0,1,2), glm::ivec3(1,3,2)};
        ppoint = vertices[0];
        normal = glm::vec3(0,1,0);
    }


};


class Cylinder : public obstacle{
  public:
    float radius, length;
    uint nLat, nLong;

    float eps = 0.25f, mu = 0.3;

    Cylinder(const glm::vec3 &c, float _r, float _l, uint _n1, uint _n2);

    virtual bool detect_collision(ClothPoint *p) const override;
    virtual void handle_collision(ClothPoint *p) const override;

};