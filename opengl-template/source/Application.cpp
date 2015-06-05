
#include <cassert>
#include <vector>
#include <opengl-template/Application.h>
#include <opengl-template/EffectSource.h>
#include <ctime>

using namespace glm;
using namespace OpenGLTemplate;
using namespace std;

// Public

Application::Application()
	: _worldTransformLocation(0u),
	  _rotation(0.0f)
{
	// Initialisation

	EffectSource effectSource;
	effectSource.load(ShaderType::Vertex, "assets/shaders/vertex.glsl");
	effectSource.load(ShaderType::Fragment, "assets/shaders/fragment.glsl");
	_effect.reset(new Effect(effectSource));
	_effect->apply();

	mat4 transform = perspective(radians(90.0f),
		static_cast<GLfloat>(Config::WINDOW_WIDTH) / Config::WINDOW_HEIGHT, 0.01f, 1000.0f);

	GLint location = _effect->getUniformLocation("unifProjectionTransform");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));

	transform = translate(vec3(0.0f, 0.0f, -150.0f)) * rotate(radians(-45.0f), vec3(-1.0f, 0.0f, 0.0f));
	location = _effect->getUniformLocation("unifViewTransform");
	assert(location != -1);
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(transform));

	location = _effect->getUniformLocation("unifWorldTransform");
	assert(location != -1);
	_worldTransformLocation = location;
	glUniformMatrix4fv(_worldTransformLocation, 1, GL_FALSE, value_ptr(mat4()));
	_effect->unapply();

	// Add a texture to the path below
	_texture = Texture::load(TextureType::Texture2D, "assets/textures/texture2.png");

	// Add a model file to the path below
	_mesh_teapot = Mesh::load("assets/models/teapot.obj", _effect, _texture);
	_mesh_cube = Mesh::load("assets/models/cube.obj", _effect, _texture);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

Application::~Application()
{
	// Deinitialisation
}

void Application::update()
{
	// Updating and drawing

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static const uint TYPE_LIGHT_POINT = 0;
	static const uint TYPE_LIGHT_SPOT = 1;
	_rotation += 0.005f;

	// Teapot
	_effect->apply();
	{
		glUniform1ui(_effect->getUniformLocation("l_type"), TYPE_LIGHT_SPOT);
		glUniform1f(_effect->getUniformLocation("l_range"), 1000.0);
		glUniform3f(_effect->getUniformLocation("l_position"), 100, 0, 100);
		glUniform4f(_effect->getUniformLocation("lc_ambient"),  0.0, 0.0, 0.0, 1.0);
		glUniform4f(_effect->getUniformLocation("lc_diffuse"),  1.0, 0.0, 1.0, 1.0);
		glUniform4f(_effect->getUniformLocation("lc_specular"), 1.0, 1.0, 1.0, 1.0);
		glUniform1f(_effect->getUniformLocation("sl_cutoff"), radians(15.0));

		const mat4 worldTransform =
			rotate(_rotation, vec3(0.0f, 1.0f, 0.0f))
			* translate(vec3(-50, 0, 0));
		glUniformMatrix4fv(_worldTransformLocation, 1, GL_FALSE, value_ptr(worldTransform));
	}
	_mesh_teapot->draw();

	// Cube
	_effect->apply();
	{
		const float phase_x =
			5.00f * sinf(1.00f * clock() / (float)CLOCKS_PER_SEC);
		const float phase_y =
			5.00f * cosf(1.00f * clock() / (float)CLOCKS_PER_SEC);
		const float phase_z =
			1.00f * sinf(0.25f * clock() / (float)CLOCKS_PER_SEC);

		glUniform1ui(_effect->getUniformLocation("l_type"), TYPE_LIGHT_POINT);
		glUniform1f(_effect->getUniformLocation("l_range"), 1000.0);
		glUniform3f(_effect->getUniformLocation("l_position"), phase_x, phase_y, phase_z);
		glUniform4f(_effect->getUniformLocation("lc_ambient"), 0.0, 0.0, 0.0, 1.0);
		glUniform4f(_effect->getUniformLocation("lc_diffuse"), 1.0, 0.0, 1.0, 1.0);
		glUniform4f(_effect->getUniformLocation("lc_specular"), 1.0, 1.0, 1.0, 1.0);

		const mat4 worldTransform =
			rotate(_rotation, vec3(0.0f, 1.0f, 0.0f))
			* translate(vec3(50, 0, 50))
			* scale(vec3(5, 5, 5));
		glUniformMatrix4fv(_worldTransformLocation, 1, GL_FALSE, value_ptr(worldTransform));
	}
	_mesh_cube->draw();
}
