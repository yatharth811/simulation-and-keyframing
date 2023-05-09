#include "camera.hpp"

#include <iostream>

using namespace COL781;
namespace GL = COL781::OpenGL;
using namespace glm;

GL::Rasterizer r;
GL::ShaderProgram program;

const int nv = 4;
const int nt = 2;
vec3 vertices[nv];
vec3 normals[nv];
ivec3 triangles[nt];

GL::Object object;
GL::AttribBuf vertexBuf, normalBuf;

CameraControl camCtl;

void initializeScene() {
	object = r.createObject();
	vertices[0] = vec3(0, 0, 1);
	vertices[1] = vec3(1, 0, 1);
	vertices[2] = vec3(1, 0, 0);
	vertices[3] = vec3(0, 0, 0);
	vertexBuf = r.createVertexAttribs(object, 0, nv, vertices);
	normals[0] = vec3(0, 0, 1);
	normals[1] = vec3(0, 0, 1);
	normals[2] = vec3(0, 0, 1);
	normals[3] = vec3(0, 0, 1);
	normalBuf = r.createVertexAttribs(object, 1, nv, normals);
	triangles[0] = ivec3(0, 1, 2);
	triangles[1] = ivec3(0, 2, 3);
	r.createTriangleIndices(object, nt, triangles);
}

void updateScene(float t) {
	float freq = 2, amp = 1;
	float phase0 = 0, phase1 = 1;
	float theta0 = amp*cos(freq*t + phase0), theta1 = amp*cos(freq*t + phase1);
	vertices[0] = vec3(0, -cos(theta0), sin(theta0));
	vertices[1] = vec3(1, -cos(theta1), sin(theta1));
	r.updateVertexAttribs(vertexBuf, nv, vertices);
	normals[0] = glm::normalize(glm::cross(vertices[1]-vertices[0], vertices[3]-vertices[0]));
	normals[1] = glm::normalize(glm::cross(vertices[2]-vertices[1], vertices[0]-vertices[1]));
	normals[2] = glm::normalize(glm::cross(vertices[3]-vertices[2], vertices[1]-vertices[2]));
	normals[3] = glm::normalize(glm::cross(vertices[0]-vertices[3], vertices[2]-vertices[3]));
	r.updateVertexAttribs(normalBuf, nv, normals);
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

	initializeScene();

	while (!r.shouldQuit()) {
		float t = SDL_GetTicks64()*1e-3;
		updateScene(t);

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
