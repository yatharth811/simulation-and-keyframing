#include<bits/stdc++.h>
#include "obstacles.hpp"

class Bone{
  public:
    std::vector<Bone *> children;
    obstacle *bone_shape;

    Bone(obstacle *_bone_shape){
        bone_shape = _bone_shape;
    }

    void add_child(Bone *b){
        children.push_back(b);
    }

};