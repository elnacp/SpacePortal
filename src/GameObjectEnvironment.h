#pragma once

using namespace std;

#include "GameObject.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

class GameObjectEnvironment : public GameObject
{
public:

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector3 color;

	//GameObjectMesh();
	//~GameObjectMesh();
	void render();
	void update(float dt);

};



/*
GameObjectMesh::GameObjectMesh()
{
}

GameObjectMesh::~GameObjectMesh()
{
}
*/