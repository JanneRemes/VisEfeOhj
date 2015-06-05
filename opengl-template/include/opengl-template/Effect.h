/**
 * @file opengl-template/Effect.h
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

#include <string>
#include <opengl-template/EffectSource.h>
#include <opengl-template/GL.h>

namespace OpenGLTemplate
{
	class Effect
	{
	public:

		Effect(const EffectSource& source);

		~Effect();

		void apply() const;

		GLint getAttributeLocation(const std::string& attributeName) const;

		GLint getUniformLocation(const std::string& uniformName) const;

		void unapply() const;

	private:

		GLuint _program;
		GLuint _vertexShader;
		GLuint _geometryShader;
		GLuint _fragmentShader;

		Effect(const Effect& effect) = delete;
		Effect(Effect&& effect) = delete;

		void createProgram();
		GLuint createAndCompileShader(const ShaderType& type, const EffectSource& source) const;
		void linkProgram() const;
		void destroyShader(const GLuint shader) const;
		void destroyProgram() const;
		void apply(const GLuint program) const;
		GLuint createShader(const GLenum type) const;
		void compileShader(const GLuint shader, const std::string& source) const;
		void attachShader(const GLuint shader) const;
		void assertShaderCompilation(const GLuint shader) const;
		void assertProgramLinkage() const;
		
		Effect& operator =(const Effect& effect) = delete;
		Effect& operator =(Effect&& effect) = delete;
	};
}
