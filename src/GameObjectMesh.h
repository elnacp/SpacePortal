#pragma once

using namespace std;

#include "GameObject.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "ResourceManager.h"

class GameObjectMesh: public GameObject 
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
	void GameObjectMesh::MoveObject(float x, float y, float z);

};



/*
GameObjectMesh::GameObjectMesh()
{
}

GameObjectMesh::~GameObjectMesh()
{
}
*/