#include "../src/camera.hpp"
#include "../src/cloth.hpp"
#include "../src/obstacles.hpp"
#include "../src/bone.hpp"
#include <iostream>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;

GL::Rasterizer r;
GL::ShaderProgram program;
GL::Object object, object2, object3, object4, object5, object6;
GL::AttribBuf vertexBuf, normalBuf, vertexBuf2, vertexBuf3, vertexBuf4, vertexBuf5, vertexBuf6;

CameraControl camCtl;

void initializeScene(std::vector<obstacle*> obstacles) {
	object2 = r.createObject();
	glm::vec3 new_particles[obstacles[0]->vertices.size()];
    for (int i = 0; i < obstacles[0]->vertices.size(); i += 1) {
      new_particles[i] = obstacles[0]->vertices[i];
    }  
	glm::ivec3 new_triangles[obstacles[0]->triangles.size()];
	for(int i=0; i<obstacles[0]->triangles.size(); i++){
		new_triangles[i] = obstacles[0]->triangles[i];
	}
	vertexBuf2 = r.createVertexAttribs(object2, 0, obstacles[0]->vertices.size(), new_particles);
	r.createTriangleIndices(object2, obstacles[0]->triangles.size(), new_triangles);

	///
	object3 = r.createObject();
	glm::vec3 new_particles2[obstacles[1]->vertices.size()];
    for (int i = 0; i < obstacles[1]->vertices.size(); i += 1) {
      new_particles2[i] = obstacles[1]->vertices[i];
    }  
	glm::ivec3 new_triangles2[obstacles[1]->triangles.size()];
	for(int i=0; i<obstacles[1]->triangles.size(); i++){
		new_triangles2[i] = obstacles[1]->triangles[i];
	}
	vertexBuf3 = r.createVertexAttribs(object3, 0, obstacles[1]->vertices.size(), new_particles2);
	r.createTriangleIndices(object3, obstacles[1]->triangles.size(), new_triangles2);

	///
	object4 = r.createObject();
	glm::vec3 new_particles3[obstacles[2]->vertices.size()];
    for (int i = 0; i < obstacles[2]->vertices.size(); i += 1) {
      new_particles3[i] = obstacles[2]->vertices[i];
    }  
	glm::ivec3 new_triangles3[obstacles[2]->triangles.size()];
	for(int i=0; i<obstacles[2]->triangles.size(); i++){
		new_triangles3[i] = obstacles[2]->triangles[i];
	}
	vertexBuf4 = r.createVertexAttribs(object4, 0, obstacles[2]->vertices.size(), new_particles3);
	r.createTriangleIndices(object4, obstacles[2]->triangles.size(), new_triangles3);


	///
	object5 = r.createObject();
	glm::vec3 new_particles4[obstacles[3]->vertices.size()];
    for (int i = 0; i < obstacles[3]->vertices.size(); i += 1) {
      new_particles4[i] = obstacles[3]->vertices[i];
    }  
	glm::ivec3 new_triangles4[obstacles[3]->triangles.size()];
	for(int i=0; i<obstacles[3]->triangles.size(); i++){
		new_triangles4[i] = obstacles[3]->triangles[i];
	}
	vertexBuf5 = r.createVertexAttribs(object5, 0, obstacles[3]->vertices.size(), new_particles4);
	r.createTriangleIndices(object5, obstacles[3]->triangles.size(), new_triangles4);

	///
	///
	object6 = r.createObject();
	glm::vec3 new_particles5[obstacles[4]->vertices.size()];
    for (int i = 0; i < obstacles[4]->vertices.size(); i += 1) {
      new_particles5[i] = obstacles[4]->vertices[i];
    }  
	glm::ivec3 new_triangles5[obstacles[4]->triangles.size()];
	for(int i=0; i<obstacles[4]->triangles.size(); i++){
		new_triangles5[i] = obstacles[4]->triangles[i];
	}
	vertexBuf6 = r.createVertexAttribs(object6, 0, obstacles[4]->vertices.size(), new_particles5);
	r.createTriangleIndices(object6, obstacles[4]->triangles.size(), new_triangles5);

}

void updateScene(std::vector<obstacle*> &obstacles, float timestep) {

	glm::vec3 new_particles1[obstacles[0]->vertices.size()];
    for (int i = 0; i < obstacles[0]->vertices.size(); i += 1) {
      new_particles1[i] = obstacles[0]->vertices[i];
    }
	r.updateVertexAttribs(vertexBuf2, (int) obstacles[0]->vertices.size(), new_particles1);

	glm::vec3 new_particles2[obstacles[1]->vertices.size()];
    for (int i = 0; i < obstacles[1]->vertices.size(); i += 1) {
      new_particles2[i] = obstacles[1]->vertices[i];
    }
	r.updateVertexAttribs(vertexBuf3, (int) obstacles[1]->vertices.size(), new_particles2);

	glm::vec3 new_particles3[obstacles[2]->vertices.size()];
    for (int i = 0; i < obstacles[2]->vertices.size(); i += 1) {
      new_particles3[i] = obstacles[2]->vertices[i];
    }
	r.updateVertexAttribs(vertexBuf4, (int) obstacles[2]->vertices.size(), new_particles3);

	glm::vec3 new_particles4[obstacles[3]->vertices.size()];
    for (int i = 0; i < obstacles[3]->vertices.size(); i += 1) {
      new_particles4[i] = obstacles[3]->vertices[i];
    }
	r.updateVertexAttribs(vertexBuf5, (int) obstacles[3]->vertices.size(), new_particles4);

	glm::vec3 new_particles5[obstacles[4]->vertices.size()];
    for (int i = 0; i < obstacles[4]->vertices.size(); i += 1) {
      new_particles5[i] = obstacles[4]->vertices[i];
    }
	r.updateVertexAttribs(vertexBuf6, (int) obstacles[4]->vertices.size(), new_particles5);

}

int main() {
	int width = 640, height = 480;
	if (!r.initialize("Animation", width, height)) {
		return EXIT_FAILURE;
	}
	camCtl.initialize(width, height);
	camCtl.camera.setCameraView(vec3(0.5, -0.5, 1.5), vec3(0.5, -0.5, 0.0), vec3(0.0, 1.0, 0.0));
	program = r.createShaderProgram(
		r.vsPhongShading(),
		r.fsPhongShading()
	);

	glm::vec3 hip_center = glm::vec3(0.5f,-0.5f+0.1f,0.0f);
	float hip_radius = 0.3f, hip_length = 0.2;
    YCylinder hip(hip_center, hip_radius, hip_length, 20, 10);
    Bone hip_bone(nullptr, &hip, hip_center, glm::vec3(1.0f,0.0f,0.0f));


	float u_l_leg_radius = 0.075, u_l_leg_length = 0.3f;
	glm::vec3 u_l_leg_pos = glm::vec3(-0.15,-hip_length/2,0);
	glm::vec3 u_l_leg_center = hip_center + u_l_leg_pos + glm::vec3(0,-u_l_leg_length/2,0);
	YCylinder u_l_leg(u_l_leg_center, u_l_leg_radius, u_l_leg_length, 20, 10);
	Bone u_l_leg_bone(&hip_bone, &u_l_leg, u_l_leg_pos, glm::vec3(1.0,0.0f,0.0f));
	u_l_leg_bone.timeSteps = std::vector<std::pair<float, float>>{{0.5, 0.0f}, {0.6, -120.0f}, {0.7, -70.0f}, {0.8, -90.0f}, {0.9, 0.0f}};
	hip_bone.add_child(&u_l_leg_bone);


	float l_l_leg_radius = 0.075, l_l_leg_length = 0.35f;
	glm::vec3 l_l_leg_pos = glm::vec3(0.0f,-u_l_leg_length,0.0f);
	glm::vec3 l_l_leg_center = u_l_leg_center + glm::vec3(0.0f,-(u_l_leg_length+l_l_leg_length)/2, 0.0f);
	YCylinder l_l_leg(l_l_leg_center, l_l_leg_radius, l_l_leg_length, 20, 10);
	Bone l_l_leg_bone(&u_l_leg_bone, &l_l_leg, l_l_leg_pos, glm::vec3(1.0f,0.0f,0.0f));
	u_l_leg_bone.add_child(&l_l_leg_bone);



	float u_r_leg_radius = 0.075, u_r_leg_length = 0.3f;
	glm::vec3 u_r_leg_pos = glm::vec3(0.15,-hip_length/2,0);
	glm::vec3 u_r_leg_center = hip_center + u_r_leg_pos + glm::vec3(0,-u_r_leg_length/2,0);
	YCylinder u_r_leg(u_r_leg_center, u_r_leg_radius, u_r_leg_length, 20, 10);
	Bone u_r_leg_bone(&hip_bone, &u_r_leg, u_r_leg_pos, glm::vec3(1.0,0.0f,0.0f));
	hip_bone.add_child(&u_r_leg_bone);


	float l_r_leg_radius = 0.075, l_r_leg_length = 0.35f;
	glm::vec3 l_r_leg_pos = glm::vec3(0.0f,-u_r_leg_length,0.0f);
	glm::vec3 l_r_leg_center = u_r_leg_center + glm::vec3(0.0f,-(u_r_leg_length+l_r_leg_length)/2, 0.0f);
	YCylinder l_r_leg(l_r_leg_center, l_r_leg_radius, l_r_leg_length, 20, 10);
	Bone l_r_leg_bone(&u_r_leg_bone, &l_r_leg, l_r_leg_pos, glm::vec3(1.0f,0.0f,0.0f));
	u_r_leg_bone.add_child(&l_r_leg_bone);


	std::vector<obstacle*> obstacles{&hip, &u_l_leg, &l_l_leg, &u_r_leg, &l_r_leg};
	float timestep = 2*1e-3;
	initializeScene(obstacles);

	float time = 0.0;

	while (!r.shouldQuit()) {
		
		hip_bone.update_time(time,glm::mat4(1.0f));
		updateScene(obstacles,timestep);
		
		camCtl.update();
		Camera &camera = camCtl.camera;

		r.clear(vec4(1.0, 1.0, 1.0, 1.0));
		r.enableDepthTest();
		r.useShaderProgram(program);

		r.setUniform(program, "model", glm::mat4(1.0));
		r.setUniform(program, "view", camera.getViewMatrix());
		r.setUniform(program, "projection", camera.getProjectionMatrix());
		r.setUniform(program, "lightPos", camera.position);
		r.setUniform(program, "viewPos", camera.position);
		r.setUniform(program, "lightColor", vec3(1.0f, 1.0f, 1.0f));

		r.setupFilledFaces();
		// r.setUniform(program, "objectColor", vec3(1.0f, 0.5f, 0.0f));
		// r.drawObject(object);
		r.setUniform(program, "objectColor", vec3(1.0f, 0.0f, 0.5f));
		r.drawObject(object2);
		// r.setUniform(program, "objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
		r.drawObject(object3);
        // r.setUniform(program, "objectColor", glm::vec3(0.0f, 0.8f, 0.5f));
		r.drawObject(object5);
		r.setUniform(program, "objectColor", vec3(1.0f, 0.0f, 1.0f));
		r.drawObject(object4);
		r.drawObject(object6);



		r.setupWireFrame();
		r.setUniform(program, "objectColor", vec3(0.0f, 0.0f, 0.0f));
        // r.drawObject(object);
		r.drawObject(object4);
		r.drawObject(object2);
		r.drawObject(object3);
		r.drawObject(object5);
		r.drawObject(object6);

		
		r.show();
		time += timestep;
	}
}
