#include "LevelParser.h"
#include "utils.h"
#include "game.h"
#include "managers.h"

#include "GameObjectMesh.h"
#include "GameObjectPlayer.h"
#include "GameObjectEnvironment.h"
#include "GameObjectMeteor.h"
#include "GameObjectPortal.h"
#include "GameObjectBonus.h"
#include "GameObjectEnemy.h"
#include "GameObject.h"

#include "BulletManager.h"

using namespace LevelParser;

/*void loadObject(xml_node<> *mesh_node, GameObject* parent) {

}*/

void LevelParser::loadLevel(char *level_url) {

	Game* game = Game::instance;

	rapidxml::file <> XMLFile(level_url);
	rapidxml::xml_document<> doc;
	doc.parse<0>(XMLFile.data());

	xml_node<> *scene = doc.first_node();
	xml_node<> *resources = scene->first_node("resources");
	xml_node<> *resource_meshes = resources->first_node("meshes");
	
	xml_node<> *mesh = resource_meshes->first_node("mesh");
	for (xml_node<> *mesh = resource_meshes->first_node("mesh"); mesh;
	mesh = mesh->next_sibling()) {
			xml_node<> *name = mesh->first_node("name");
			xml_node<> *url = mesh->last_node("url");
			
			cout << url->value() << endl;

			g_resourceManager.loadMesh(name->value(), url->value());
	}

	xml_node<> *resource_textures = resources->first_node()->next_sibling();
	xml_node<> *texture = resource_textures->first_node("texture");
	for (xml_node<> *texture = resource_textures->first_node("texture"); texture;
	texture = texture->next_sibling()) {
			xml_node<> *name = texture->first_node("name");
			xml_node<> *url = texture->last_node("url");

			cout << url->value() << endl;
	
			g_resourceManager.loadTexture(name->value(), url->value());
	}

	xml_node<> *resource_shaders = resources->first_node()->next_sibling("shaders");
	xml_node<> *shader = resource_shaders->first_node("shader");
	for (xml_node<> *shader = resource_shaders->first_node("shader"); shader;
	shader = shader->next_sibling()) {

		xml_node<> *name = shader->first_node("name");
		xml_node<> *vertexshader = shader->first_node("url-vert");
		xml_node<> *fragshader = shader->last_node("url-frag");
			
		g_resourceManager.loadShader(name->value(), vertexshader->value(), fragshader->value());


		cout << vertexshader->value() << endl;
		cout << fragshader->value() << endl;

		}

	xml_node<> *root = scene->first_node()->next_sibling("root");
	xml_node<> *xml_env = root->first_node("environment");
	
	/*
	cout << xml_env->first_node("mesh")->value() << endl;
	cout << xml_env->first_node("shader")->value() << endl;
	cout << xml_env->first_node("texture")->value() << endl;
	*/

	GameObjectEnvironment* environment = new GameObjectEnvironment();
	environment->name = xml_env->first_node("name")->value();
	environment->mesh = g_resourceManager.getMesh(xml_env->first_node("mesh")->value());
	environment->shader = g_resourceManager.getShader(xml_env->first_node("shader")->value());
	environment->texture = g_resourceManager.getTexture(xml_env->first_node("texture")->value());
	environment->model.setScale(1, 1, 1);
	g_scene.root->addChild(environment);

	xml_node<> *light = root->first_node()->next_sibling("light");
	xml_node<> *position = light->first_node();

	xml_node<> *x = position->first_node("x");
	xml_node<> *y = position->first_node("y");
	xml_node<> *z = position->last_node("z");
	
	Vector3 light_pos((int)atoi(position->first_node()->value()), (int)atoi(position->first_node()->value()), (int)atoi(position->last_node()->value()));
	g_scene.light_pos = light_pos;
	
	xml_node<> *camera = root->first_node()->next_sibling()->next_sibling("camera");
	xml_node<> *fov = camera->first_node("fov");
	xml_node<> *nearcam = fov->next_sibling("nearcam");
	xml_node<> *farcam = fov->next_sibling()->next_sibling("far");

	/*
	cout << fov->first_node()->value() << endl;
	cout << fov->next_sibling()->value() << endl;
	cout << fov->next_sibling()->next_sibling()->value() << endl;
	*/

	xml_node<> *eye_pos = camera->first_node("eye");
	
	Vector3 eye((float)atof(eye_pos->first_node("x")->value()), (float)atof(eye_pos->first_node("y")->value()), (float)atof(eye_pos->first_node("z")->value()));	
	
	/*
	cout << eye_pos->first_node("x")->value() << endl;
	cout << eye_pos->first_node("y")->value() << endl;
	cout << eye_pos->first_node("z")->value() << endl;
	*/

	xml_node<> *target_pos = camera->first_node("target");
		
	Vector3 target((float)atof(target_pos->first_node("x")->value()), (float)atof(target_pos->first_node("y")->value()), (float)atof(target_pos->first_node("z")->value()));

	/*
	cout << target_pos->first_node("x")->value() << endl;
	cout << target_pos->first_node("y")->value() << endl;
	cout << target_pos->first_node("z")->value() << endl;
	*/

	xml_node<> *up_pos = camera->first_node("up");

	Vector3 up((float)atof(up_pos->first_node("x")->value()), (float)atof(up_pos->first_node("y")->value()), (float)atof(up_pos->first_node("z")->value()));

	/*
	cout << up_pos->first_node("x")->value() << endl;
	cout << up_pos->first_node("y")->value() << endl;
	cout << up_pos->first_node("z")->value() << endl;
	*/

	Camera* cam = new Camera();
	cam->lookAt(eye, target, up); //position the camera and point to 0,0,0
	cam->setPerspective((float)atof(fov->first_node()->value()), (float)800/600, (float)atof(fov->next_sibling()->value()), (float)atof(fov->next_sibling()->next_sibling()->value())); //set the projection, we want to be perspective

	g_scene.camera = cam;

	/*
	cout << (float)atof(fov->first_node()->value()) << endl;
	cout << (float)atof(fov->next_sibling()->value()) << endl;
	cout << (float)atof(fov->next_sibling()->next_sibling()->value()) << endl;
	*/

	xml_node<> *node = camera->next_sibling("node");
	xml_node<> *fathernode = node->first_node();

	/*
	cout << camera->next_sibling()->name() << endl;
	cout << node->first_node("name")->value() << endl;
	cout << node->first_node("mesh")->value() << endl;
	cout << node->first_node("shader")->value() << endl;
	cout << node->first_node("texture")->value() << endl;
	*/
	
	GameObjectMesh* father = new GameObjectMesh();
	father->name = node->first_node("name")->value();
	father->mesh = g_resourceManager.getMesh(node->first_node("mesh")->value());
	father->shader = g_resourceManager.getShader(node->first_node("shader")->value());
	father->texture = g_resourceManager.getTexture(node->first_node("texture")->value());
	father->model.setScale(1, 1, 1);
	g_scene.root->addChild(father);
	
	
	xml_node<> *x3_fighter = node->first_node("node");

	/*
	cout << x3_fighter->first_node("name")->value() << endl;
	cout << x3_fighter->first_node("mesh")->value() << endl;
	cout << x3_fighter->first_node("shader")->value() << endl;
	cout << x3_fighter->first_node("texture")->value() << endl;
	*/

	GameObjectPlayer* child = new GameObjectPlayer();
	child->name = x3_fighter->first_node("name")->value();
	child->mesh = g_resourceManager.getMesh(x3_fighter->first_node("mesh")->value());
	child->shader = g_resourceManager.getShader(x3_fighter->first_node("shader")->value());
	child->texture = g_resourceManager.getTexture(x3_fighter->first_node("texture")->value());
	child->model.setScale(1, 1, 1);
	father->addChild(child);
	child->model.traslate(40, 0, 0);
	
	xml_node<> *meteorxml = x3_fighter->next_sibling("node");

	/*
	cout << meteorxml->first_node("name")->value() << endl;
	cout << meteorxml->first_node("mesh")->value() << endl;
	cout << meteorxml->first_node("shader")->value() << endl;
	cout << meteorxml->first_node("texture")->value() << endl;
	*/

	GameObjectMeteor* meteor = new GameObjectMeteor();
	meteor->name = "meteor";
	meteor->mesh = g_resourceManager.getMesh(meteorxml->first_node("mesh")->value());
	meteor->shader = g_resourceManager.getShader(meteorxml->first_node("shader")->value());
	meteor->texture = g_resourceManager.getTexture(meteorxml->first_node("texture")->value());
	meteor->model.setScale(1, 1, 1);
	father->addChild(meteor);

	xml_node<> *bulletxml = meteorxml->next_sibling("node");
	
	BulletManager* bullets = new BulletManager(g_resourceManager.getMesh(bulletxml->first_node("mesh")->value()),100, g_resourceManager.getShader(bulletxml->first_node("shader")->value()));
	Game* g = Game::instance;
	g->bullet_manager = bullets;
/*
	Vector3 player_pos = Vector3(child->model.m[12], child->model.m[13], child->model.m[14]);
	bullets->createBullet(player_pos, Vector3(0, 0, 0), 100, 100, child);*/

	cout << bulletxml->first_node("mesh")->value() << endl;

	cout << "OK" << endl;
}