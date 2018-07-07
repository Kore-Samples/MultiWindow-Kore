#include "pch.h"

#include <Kore/IO/FileReader.h>
#include <Kore/Graphics4/Graphics.h>
#include <Kore/Graphics4/PipelineState.h>
#include <Kore/Graphics4/Shader.h>
#include <Kore/Input/Keyboard.h>
#include <Kore/System.h>
#include <limits>
#include <stdlib.h>
#include <stdio.h>

using namespace Kore;

namespace {
	Graphics4::Shader* vertexShader;
	Graphics4::Shader* fragmentShader;
	Graphics4::PipelineState* pipeline;
	Graphics4::VertexBuffer* vertices;
	Graphics4::IndexBuffer* indices;
	Graphics4::ConstantLocation color;
	Window* window;

	void update() {
		Graphics4::begin(0);
		Graphics4::clear(Graphics4::ClearColorFlag);
		Graphics4::setPipeline(pipeline);
		Graphics4::setFloat3(color, vec3(1.0f, 0.0f, 0.0f));
		Graphics4::setVertexBuffer(*vertices);
		Graphics4::setIndexBuffer(*indices);
		Graphics4::drawIndexedVertices();
		Graphics4::end(0);

		Graphics4::begin(1);
		Graphics4::clear(Graphics4::ClearColorFlag);
		Graphics4::setPipeline(pipeline);
		Graphics4::setFloat3(color, vec3(0.0f, 0.0f, 1.0f));
		Graphics4::setVertexBuffer(*vertices);
		Graphics4::setIndexBuffer(*indices);
		Graphics4::drawIndexedVertices();
		Graphics4::end(1);

		Graphics4::swapBuffers();
	}

	void keyUp(Kore::KeyCode key) {
		if (key == KeyEscape) {
			System::stop();
		}
	}
}

int kore(int argc, char** argv) {
	Kore::WindowOptions options;
	options.title = "Window 1";
	options.x = 100;
	options.y = 100;
	window = Kore::System::init("MultiWindow", 800, 600, &options);

	options.title = "Window 2";
	options.width = 800;
	options.height = 600;
	options.x = 1000;
	options.y = 100;
	Kore::Window::create(&options);

	Kore::Keyboard::the()->KeyUp = keyUp;

	Kore::System::setCallback(update);

	FileReader vs("shader.vert");
	FileReader fs("shader.frag");
	vertexShader = new Graphics4::Shader(vs.readAll(), vs.size(), Graphics4::VertexShader);
	fragmentShader = new Graphics4::Shader(fs.readAll(), fs.size(), Graphics4::FragmentShader);
	Graphics4::VertexStructure structure;
	structure.add("pos", Graphics4::Float3VertexData);
	pipeline = new Graphics4::PipelineState();
	pipeline->vertexShader = vertexShader;
	pipeline->fragmentShader = fragmentShader;
	pipeline->inputLayout[0] = &structure;
	pipeline->inputLayout[1] = nullptr;
	pipeline->compile();

	color = pipeline->getConstantLocation("color");

	vertices = new Graphics4::VertexBuffer(3, structure);
	float* v = vertices->lock();
	v[0] = -1; v[1] = -1; v[2] = 0.5;
	v[3] = 1;  v[4] = -1; v[5] = 0.5;
	v[6] = -1; v[7] = 1;  v[8] = 0.5;
	vertices->unlock();

	indices = new Graphics4::IndexBuffer(3);
	int* i = indices->lock();
	i[0] = 0; i[1] = 1; i[2] = 2;
	indices->unlock();

	Kore::System::start();

	return 0;
}
