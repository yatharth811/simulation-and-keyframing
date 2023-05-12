#include "../src/camera.hpp"
#include "../src/cloth.hpp"
#include "../src/obstacles.hpp"
#include <iostream>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;

GL::Rasterizer r;
GL::ShaderProgram program;
GL::Object object, object2, object3, object4;
GL::AttribBuf vertexBuf, normalBuf, vertexBuf2, vertexBuf3, vertexBuf4;

CameraControl camCtl;

void initializeScene(Cloth &pashmina, std::vector<obstacle*> obstacles) {
	object = r.createObject();
    glm::vec3 particles[pashmina.particles.size()];
    for (int i = 0; i < pashmina.particles.size(); i += 1) {
      particles[i] = pashmina.particles[i]->position;
    }  
	vertexBuf = r.createVertexAttribs(object, 0, pashmina.totalVertices, particles);
	r.createTriangleIndices(object, pashmina.totalTriangles, pashmina.triangles);

    // Sphere
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
	
	// std::cout << obstacles[1]->triangles.size() << std::endl;
    // Plane
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

    // // Cylinder
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

}

void updateScene(Cloth &pashmina, std::vector<obstacle*> &obstacles, float timestep) {
  // pashmina.update_points(timestep);
	pashmina.update_pbd_points(timestep, obstacles, true);
	glm::vec3 particles[pashmina.particles.size()];
	for (int i = 0; i < pashmina.particles.size(); i += 1) {
		particles[i] = pashmina.particles[i]->position;
	}
	r.updateVertexAttribs(vertexBuf, (int) pashmina.particles.size(), particles);

	// Sphere 
	obstacles[0]->update_pos(timestep);
	glm::vec3 new_particles[obstacles[0]->vertices.size()];
    for (int i = 0; i < obstacles[0]->vertices.size(); i += 1) {
      new_particles[i] = obstacles[0]->vertices[i];
    }

	r.updateVertexAttribs(vertexBuf2, (int) obstacles[0]->vertices.size(), new_particles);
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

	Cloth pashmina(1.0f, 1.0f, 25, 25);
	Sphere s(20,20,0.25f,glm::vec3(0.5f,-0.5f,0.2f));
	s.set_velocity(glm::vec3(0,1,0));
	Plane p;
    Cylinder c(glm::vec3(0.5f, -0.5f, 0.75f), 0.1f, 1.2f, 20, 10);
	std::vector<obstacle*> obstacles{&s,&p,&c};
	float timestep = 2*1e-3;
	initializeScene(pashmina,obstacles);

	while (!r.shouldQuit()) {
		
        updateScene(pashmina, obstacles,timestep);

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
		r.setUniform(program, "objectColor", vec3(1.0f, 0.5f, 0.0f));
		r.drawObject(object);
		r.setUniform(program, "objectColor", vec3(1.0f, 0.0f, 0.5f));
		r.drawObject(object2);
		r.setUniform(program, "objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
		r.drawObject(object3);
        r.setUniform(program, "objectColor", glm::vec3(0.0f, 0.8f, 0.5f));
		r.drawObject(object4);

		r.setupWireFrame();
		r.setUniform(program, "objectColor", vec3(0.0f, 0.0f, 0.0f));
        r.drawObject(object);
		r.drawObject(object4);
		r.drawObject(object2);
		// r.drawObject(object3);

		
		r.show();
	}
}
