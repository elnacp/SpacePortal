#pragma once

#include "utils.h"
#include "camera.h"


using namespace std;

class GameObject
{
public:
	string name;
	Matrix44 model; 

	GameObject* parent;
	vector <GameObject*> children;

	

	//GameObject();
	//~GameObject();


	void addChild(GameObject* obj);
	void removeChild(GameObject* obj);
	void clear();


	virtual void render();
	virtual void update(float elapsed_time);



	Matrix44 getGlobalMatrix();

	

};
/*
GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
*/