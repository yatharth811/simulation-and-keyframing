#include <bits/stdc++.h>
#include "obstacles.hpp"
#include "glm/gtx/transform.hpp"

class Bone{
  public:
    Bone* parent;
    std::vector<Bone *> children;
    obstacle *bone_shape;
    glm::vec3 position, rotationAxis;
    std::vector<std::pair<float, float>> timeSteps;
    float theta;

    Bone() {
        parent = NULL;
        bone_shape = NULL;
        position = glm::vec3(0);
        rotationAxis = glm::vec3(0);
    }

    Bone(Bone* _parent, obstacle* _bone_shape, glm::vec3 _position, glm::vec3 _rotationAxis) {
        parent = _parent;
        bone_shape = _bone_shape;
        position = _position;
        rotationAxis = _rotationAxis;
    }

    void add_child(Bone *b) {
        children.push_back(b);
    }

    void update_time(float delta, glm::mat4 p) {
        glm::mat4 m = glm::translate(p, position);

        if (timeSteps.size() > 0) {
            int lastTime = ceil(timeSteps[timeSteps.size() - 1].first + 1.0f);
            int currentTime = ceil(delta);
            int cycle = currentTime / lastTime;
            delta -= cycle * lastTime;
            for (size_t i = 0; i < timeSteps.size(); i++) {
                if (delta < timeSteps[i].first) {
                    float t0 = 0.0f;
                    float t1 = 0.0f;
                    float t2 = timeSteps[i].first;
                    float t3 = 0.0f;
                    float a0 = 0.0f;
                    float a1 = 0.0f;
                    float a2 = timeSteps[i].second;
                    float a3 = 0.0f;

                    if (i == 0) {
                        theta = timeSteps[i].second * delta / t2;
                        break;
                    }
                    if (i > 0) {
                        t1 = timeSteps[i - 1].first;
                        a1 = timeSteps[i - 1].second;
                    }
                    if (i > 1) {
                        t0 = timeSteps[i - 2].first;
                        a0 = timeSteps[i - 2].second;
                    }
                    if (i < timeSteps.size() - 1) {
                        t3 = timeSteps[i + 1].first;
                        a3 = timeSteps[i + 1].second;
                    } else {
                        t3 = t2 + (t2 - t1);
                        a3 = a2;
                    }

                    float dq_dt_1 = (a2 - a1) / (t2 - t1) * (t1 - t0) / (t2 - t0) +
                                    (a1 - a0) / (t1 - t0) * (t2 - t1) / (t2 - t0);
                    float dq_dt_2 = (a3 - a2) / (t3 - t2) * (t2 - t1) / (t3 - t1) +
                                    (a2 - a1) / (t2 - t1) * (t3 - t2) / (t3 - t1);
                    float dt = (delta - t1) / (t2 - t1);
                    float dt_2 = dt * dt;
                    float dt_3 = dt_2 * dt;
                    theta = (2 * dt_3 - 3 * dt_2 + 1) * a1 +
                            (dt_3 - 2 * dt_2 + dt) * dq_dt_1 + (-2 * dt_3 + 3 * dt_2) * a2 +
                            (dt_3 - dt_2) * dq_dt_2;

                    break;
                }
            }
        }

        m = glm::rotate(m, glm::radians(theta), rotationAxis);

        for (auto &x : bone_shape->vertices) {
            glm::vec4 sx = glm::vec4(x, 1);
            sx = m * sx;
            x = glm::vec3(sx.x, sx.y, sx.z);
        }

        for (auto &x : children) {
            x->update_time(delta, m);
        }

    }

};