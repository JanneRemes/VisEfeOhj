/**
 * @file opengl-template/Mesh.cpp
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

#include <cassert>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <opengl-template/Mesh.h>


using namespace OpenGLTemplate;
using namespace std;

// Public

void Mesh::draw() const
{
	_effect->apply();
	_bufferState.apply();
	_texture->apply();
	glDrawElements(GL_TRIANGLES, _indexBuffer.elementCount(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
	_texture->unapply();
	_bufferState.unapply();
	_effect->unapply();
}

// Static

shared_ptr<Mesh> Mesh::load(const string& filepath, const shared_ptr<Effect>& effect,
	const shared_ptr<Texture>& texture)
{
	const aiScene* scene = aiImportFile(filepath.c_str(), 0u);
	aiMesh* mesh = scene->mMeshes[0];
	vector<GLfloat> vertices(8u * mesh->mNumVertices);
	vector<GLuint> indices;

	for(GLuint i = 0u; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3u);

		for(GLuint j = 0u; j < face.mNumIndices; ++j)
		{
			const GLuint index = face.mIndices[j];
			vertices[8u * index]	  = mesh->mVertices[index].x;
			vertices[8u * index + 1u] = mesh->mVertices[index].y;
			vertices[8u * index + 2u] = mesh->mVertices[index].z;
			vertices[8u * index + 3u] = mesh->mTextureCoords[0][index].x;
			vertices[8u * index + 4u] = mesh->mTextureCoords[0][index].y;
			vertices[8u * index + 5u] = mesh->mNormals[index].x;
			vertices[8u * index + 6u] = mesh->mNormals[index].y;
			vertices[8u * index + 7u] = mesh->mNormals[index].z;
			indices.push_back(index);
		}
	}

	aiReleaseImport(scene);
	Mesh* ogltMesh = new Mesh(vertices, indices);
	ogltMesh->_effect = effect;
	ogltMesh->_texture = texture;

	return shared_ptr<Mesh>(ogltMesh);
}

// Private

Mesh::Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
	: _vertexBuffer(vertices.size(), sizeof(GLfloat), BufferType::Vertex, BufferUsage::DrawStatic),
	  _indexBuffer(indices.size(), sizeof(GLuint), BufferType::Index, BufferUsage::DrawStatic)
{
	_vertexBuffer.setData(vertices);
	_indexBuffer.setData(indices);

	VertexFormat vertexFormat
	{
		{ 0u, 3u, VertexElementType::Float32, GL_FALSE }, // Position
		{ 1u, 2u, VertexElementType::Float32, GL_FALSE }, // TexCoord
		{ 2u, 3u, VertexElementType::Float32, GL_FALSE }  // Normal
	};

	_bufferState.initialise(vertexFormat, &_vertexBuffer, &_indexBuffer);
}
