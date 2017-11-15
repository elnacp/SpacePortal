
#pragma once

#include "mesh.h"
#include 



class ResourceManager{
private:	
	std::unordered_map<std::string, Mesh*> m_meshes;
	std::unordered_map<std::string, Texture*> m_meshes;
	std::unordered_map<std::string, Shader*> m_meshes;

};

