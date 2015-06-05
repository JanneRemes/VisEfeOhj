
#pragma once

#include <memory>
#include <opengl-template/Buffer.h>
#include <opengl-template/BufferState.h>
#include <opengl-template/Effect.h>
#include <opengl-template/GL.h>
#include <opengl-template/GLM.h>
#include <opengl-template/Mesh.h>
#include <opengl-template/Texture.h>

namespace Config
{
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const int MULTISAMPLING_SAMPLES = 16; // 0 = disabled
}

namespace OpenGLTemplate
{
	class Application
	{
	public:
		Application();
		~Application();

		void update();
	private:
		std::shared_ptr<Effect> _effect;
		std::shared_ptr<Mesh> _mesh_teapot;
		std::shared_ptr<Mesh> _mesh_cube;

		std::shared_ptr<Buffer> _vertexBuffer;
		std::shared_ptr<Buffer> _indexBuffer;
		std::shared_ptr<Texture> _texture;
		std::shared_ptr<BufferState> _bufferState;
		GLuint _worldTransformLocation;
		GLfloat _rotation;

		Application(const Application& application) = delete;
		Application(Application&& application) = delete;
		
		Application& operator =(const Application& application) = delete;
		Application& operator =(Application&& application) = delete;
	};
}
