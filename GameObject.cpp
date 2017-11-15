#include "GameObject.h"
#include <algorithm>


using namespace std;

void GameObject::render() {
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
}

void GameObject::update(float dt){
	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}
}

void GameObject::addChild(GameObject* obj){
	obj->parent = this;
	this->children.push_back(obj);
}

void GameObject::removeChild(GameObject* obj) {
	vector<GameObject*>::iterator i = remove(this->children.begin(), this->children.end(), obj);

	this->children.erase(i, this->children.end());

}


//global matriz is the model multiplied by all parent's models (IMPORTANT:the order of multiplication is important)
Matrix44 GameObject::getGlobalMatrix() {
	if (this->parent) {
		return this->parent->getGlobalMatrix()* this->model;
	}
	else {
		return this->model;
	}
}

