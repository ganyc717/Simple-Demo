#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "myTexture.h"


class myMesh;
class myShader;


struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};


struct myShader
{
	GLuint program;
};

static std::map<std::string, myTexture> global_texture;

class Texture
{
public:
	GLuint id = 0;
	aiTextureType type;
	GLuint load(std::string name);
};




class myModel
{
public:
	bool loadModel(const char* path);
	void processNode(aiNode* node, const aiScene* scene);
	myMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> myModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type);
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