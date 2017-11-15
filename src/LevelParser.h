#pragma once
#include <iostream>

#include "GameObject.h"

#include "extra\rapidxml.hpp"
#include "extra\rapidxml_utils.hpp"
#include "extra\rapidxml_iterators.hpp"

using namespace rapidxml;

namespace LevelParser {

	void loadObject(xml_node<> *mesh_node, GameObject* parent);
	void loadLevel(char* level_url);

}