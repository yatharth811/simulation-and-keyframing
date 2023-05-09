#include "camera.hpp"
#include "cloth.hpp"
#include <iostream>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;

GL::Rasterizer r;
GL::ShaderProgram program;
GL::Object object;
GL::AttribBuf vertexBuf, normalBuf;

CameraControl camCtl;

void initializeScene(Cloth &pashmina) {
	object = r.createObject();
  glm::vec3 particles[pashmina.particles.size()];
  for (int i = 0; i < pashmina.particles.size(); i += 1) {
    particles[i] = pashmina.particles[i]->position;
  }  
	vertexBuf = r.createVertexAttribs(object, 0, pashmina.totalVertices, particles);
	r.createTriangleIndices(object, pashmina.totalTriangles, pashmina.triangles);
}

void updateScene(Cloth &pashmina) {
  pashmina.update_points();
  glm::vec3 particles[pashmina.particles.size()];
  for (int i = 0; i < pashmina.particles.size(); i += 1) {
    particles[i] = pashmina.particles[i]->position;
  }
	r.updateVertexAttribs(vertexBuf, (int) pashmina.particles.size(), particles);
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
	initializeScene(pashmina);

	while (!r.shouldQuit()) {
		
    updateScene(pashmina);

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

		r.setupWireFrame();
		r.setUniform(program, "objectColor", vec3(0.0f, 0.0f, 0.0f));
		r.drawObject(object);


		r.show();
	}
}
