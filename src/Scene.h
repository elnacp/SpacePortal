#pragma once

using namespace std;

#include "GameObject.h"


class  Scene
{
public:
	 Scene();
	~ Scene();

	//scene graph starts from here
	GameObject* root;

	//the camera used to render the scene
	Camera* camera;

	//a list of all lights - could be within graph or not
	Vector3 light_pos;//(0.0f, 100.f, 0.0f);

	void render();
	void update(float);

	//neteja l'arbre, funcio recursiva
	//void clear();

};