#pragma once

using namespace std;

#include "GameObjectMesh.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "ResourceManager.h"

class GameObjectMeteor : public GameObjectMesh
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
	void GameObjectMeteor::MoveObjectMeteor(float x, float y, float z);

};

