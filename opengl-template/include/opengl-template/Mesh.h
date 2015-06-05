/**
 * @file opengl-template/Mesh.h
 *
 * OpenGL Template
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 *
 * OpenGL Template is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <opengl-template/Buffer.h>
#include <opengl-template/BufferState.h>
#include <opengl-template/Effect.h>
#include <opengl-template/GL.h>
#include <opengl-template/Texture.h>

namespace OpenGLTemplate
{
	class Mesh
	{
	public:

		void draw() const;

		static std::shared_ptr<Mesh> load(const std::string& filepath, const std::shared_ptr<Effect>& effect,
			const std::shared_ptr<Texture>& texture);

	private:

		Buffer _vertexBuffer;
		Buffer _indexBuffer;
		BufferState _bufferState;
		std::shared_ptr<Effect> _effect;
		std::shared_ptr<Texture> _texture;

		Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices);
		Mesh(const Mesh& mesh) = delete;
		Mesh(Mesh&& mesh) = delete;

		Mesh& operator =(const Mesh& mesh) = delete;
		Mesh& operator =(Mesh&& mesh) = delete;
	};
}
