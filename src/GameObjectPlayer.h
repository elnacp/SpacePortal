#pragma once

using namespace std;

#include "GameObjectMesh.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "ResourceManager.h"

class GameObjectPlayer : public GameObjectMesh
{
public:

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector3 color;


	//GameObjectPlayer();
	//~GameObjectPlayer();
	void render();
	void update(float dt);
	void GameObjectPlayer::MoveObjectMesh(float x, float y, float z);
	float lerp(float a, float b, float t);

};



/*
GameObjectMesh::GameObjectMesh()
{
}

GameObjectMesh::~GameObjectMesh()
{
}
*/
