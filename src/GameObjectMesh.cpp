#include "GameObjectMesh.h"
#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "rendertotexture.h"
#include "shader.h"
#include "camera.h"


#include "managers.h"

#include <cmath>


 void GameObjectMesh::render() {

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render();
	}
}

void GameObjectMesh::update(float dt) {

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->update(dt);
	}

}

void GameObjectMesh::MoveObject(float x, float y, float z) {

	this->model.traslate(x, y, z);

}
