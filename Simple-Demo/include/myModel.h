#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "myTexture.h"

class myMesh;
class myShader;

enum textureType
{
	MATERIAL,
	DIFFUSE,
	SPECULAR
};
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	myTexture texture;
	GLuint id;
	textureType type;
};

struct myShader
{
	GLuint program;
};

class myModel
{
public:
	/*
	myModel();
	~myModel();
	*/
	bool loadModel(const char* path);
	void processNode(aiNode* node, const aiScene* scene);
	myMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> myModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, textureType texType);
	void Draw(myShader shader);
private:
	std::vector<myMesh> meshes;
};


class myMesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	myMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture);
	void Draw(myShader shader);

private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};