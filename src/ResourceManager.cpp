#include "ResourceManager.h"
#include "shader.h"

//load a mesh into memory, and associate it with a handle
void ResourceManager::loadMesh(const std::string handle,
	const std::string path)
{
	Mesh *mesh = new Mesh();
	mesh->loadASE(path.c_str());
	m_meshes[handle] = mesh;
}//unload a mesh from memory, given its handle
void ResourceManager::unloadMesh(const std::string handle) {
	//try to find mesh in map
	std::unordered_map<std::string, Mesh*>::iterator itr = m_meshes.find(handle);
	//check it's there
	if (itr != m_meshes.end())
	{
		// found it - delete it and erase it
		delete itr->second;
		m_meshes.erase(itr);
	}
	else //else print error and quit
	{
		std::cout << "could not find asset with handle" << handle << std::endl;
		exit(0);
	}
}

//Return a pointer to a shader, given a handle
Mesh* ResourceManager::getMesh(const std::string name) {
	//...you should probably add some error checking here...!
	return m_meshes.at(name);
}

//load a mesh into memory, and associate it with a handle
boolean ResourceManager::loadShader(const std::string handle,
	const std::string path1, const std::string path2)
{
	Shader *shader = new Shader();
	shader->load(path1.c_str(), path2.c_str());
	m_shaders[handle] = shader;
	return true;
}//unload a mesh from memory, given its handle
void ResourceManager::unloadShader(const std::string handle) {
	//try to find mesh in map
	std::unordered_map<std::string, Shader*>::iterator itr = m_shaders.find(handle);
	//check it's there
	if (itr != m_shaders.end())
	{
		// found it - delete it and erase it
		delete itr->second;
		m_shaders.erase(itr);
	}
	else //else print error and quit
	{
		std::cout << "could not find asset with handle" << handle << std::endl;
		exit(0);
	}
}

//Return a pointer to a mesh object, given a handle
Shader* ResourceManager::getShader(const std::string name) {
	//...you should probably add some error checking here...!
	return m_shaders.at(name);
}

//load a mesh into memory, and associate it with a handle
void ResourceManager::loadTexture(const std::string handle,
	const std::string path)
{
	Texture *texture = new Texture();
	texture->load(path.c_str());
	m_textures[handle] = texture;
}//unload a mesh from memory, given its handle
void ResourceManager::unloadTexture(const std::string handle) {
	//try to find mesh in map
	std::unordered_map<std::string, Texture*>::iterator itr = m_textures.find(handle);
	//check it's there
	if (itr != m_textures.end())
	{
		// found it - delete it and erase it
		delete itr->second;
		m_textures.erase(itr);
	}
	else //else print error and quit
	{
		std::cout << "could not find asset with handle" << handle << std::endl;
		exit(0);
	}
}

//Return a pointer to a shader, given a handle
Texture* ResourceManager::getTexture(const std::string name) {
	//...you should probably add some error checking here...!
	return m_textures.at(name);
}