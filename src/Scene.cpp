#include "Scene.h"
#include "utils.h"




Scene::Scene() {
	this->root = new GameObject();
	this->root->parent = NULL;
	this->light_pos = Vector3(0, 100, 0);
}

Scene::~Scene() {
	//this->root->clear();
	//delete this->root;
}

void Scene::render() {
	this->root->render();
}

void Scene::update(float dt) {
	this->root->update(dt);
}