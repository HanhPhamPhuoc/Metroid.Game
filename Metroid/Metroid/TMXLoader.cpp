#include "TMXLoader.h"


TMXLoader::TMXLoader()
{
}


TMXLoader::~TMXLoader()
{
	for (std::unordered_map<std::string, TMXMap*>::iterator it = _MapContainer.begin(); it != _MapContainer.end();it++)
	{
		delete it->second;
	}
}


void TMXLoader::AddMap(std::string mapName, std::string filePath)
{
	// String to hold file contents
	std::string fileContents = "";

	// Attempt to load file using provided file path
	if (LoadFile(filePath, fileContents) == true)
	{
		// Create new RapidXML document instance to use to parse map data
		rapidxml::xml_document<char> currentMap;

		//parse the file
		currentMap.parse<0>((char*)fileContents.c_str());

		//get first node
		rapidxml::xml_node<> *parentNode = currentMap.first_node("map");

		// Add new TMXMap to m_mapContainer
		_MapContainer[mapName] = new TMXMap();

		// Load the map settings, tilesets and layers
		LoadMapSettings(_MapContainer[mapName], parentNode);
		LoadTileSets(_MapContainer[mapName], parentNode);
		LoadLayers(_MapContainer[mapName], parentNode);
	}
}


TMXMap* TMXLoader::GetMap(std::string mapName)
{
	// Attempt to find and return a map using provided name, else return nullptr
	std::unordered_map<std::string, TMXMap*>::iterator it = _MapContainer.find(mapName);

	if (it == _MapContainer.end())
	{
		return NULL;
	}
	else
	{
		//first means key, which is mapName
		//second means value, which is TMXMap
		return it->second;
	}
}

void TMXLoader::LoadMapSettings(TMXMap* map, rapidxml::xml_node<> *parentNode)
{
	unsigned int width = atoi(parentNode->first_attribute("width")->value());
	unsigned int height = atoi(parentNode->first_attribute("height")->value());
	unsigned int tileWidth = atoi(parentNode->first_attribute("tilewidth")->value());
	unsigned int tileHeight = atoi(parentNode->first_attribute("tileheight")->value());
	
	map->SetAttributes(width, height, tileWidth, tileHeight);
}


void TMXLoader::LoadTileSets(TMXMap* map, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	//get tileset node
	currentNode = currentNode->first_node("tileset");

	// Check if there is a tileset node
	if (currentNode != nullptr)
	{
		unsigned int firstGid = atoi(currentNode->first_attribute("firstgid")->value());
		std::string name = currentNode->first_attribute("name")->value();
		unsigned int tileWidth = atoi(currentNode->first_attribute("tilewidth")->value());
		unsigned int tileHeight = atoi(currentNode->first_attribute("tileheight")->value());
		unsigned int tileCount = atoi(currentNode->first_attribute("tilecount")->value());
		unsigned int columns = atoi(currentNode->first_attribute("columns")->value());

		currentNode = currentNode->first_node("image");
		std::string sourcePath;
		unsigned int imageWidth;
		unsigned int imageHeight;
		if (currentNode != nullptr)
		{
			sourcePath = currentNode->first_attribute("source")->value();
			imageWidth = atoi(currentNode->first_attribute("width")->value());
			imageHeight = atoi(currentNode->first_attribute("height")->value());
		}

		TMXTileSet tileSet;
		tileSet.SetAttributes(name, firstGid, tileWidth, tileHeight, tileCount, columns, sourcePath, imageWidth, imageHeight);

		map->SetTileSet(tileSet);

	}
}


void TMXLoader::LoadLayers(TMXMap* map, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	// Move to first layer node
	currentNode = currentNode->first_node("layer");

	while (currentNode != nullptr)
	{
		TMXTileLayer layer;

		//get layer attributes
		std::string layerName = currentNode->first_attribute("name")->value();
		unsigned int layerWidth = atoi(currentNode->first_attribute("width")->value());
		unsigned int layerHeight = atoi(currentNode->first_attribute("height")->value());

		//set layer attributes
		layer.SetAttributes(layerName, layerWidth, layerHeight);

		//get layer data
		char* data = currentNode->first_node("data")->value();
		
		//set layer data
		layer.SetData(data);

		//Add layer to map
		map->AddLayer(layer);

		// Move to the next layer
		currentNode = currentNode->parent()->next_sibling("layer");
	}
}

bool TMXLoader::LoadFile(std::string filePath, std::string &fileContents)
{
	std::ifstream file(filePath, std::ios::in | std::ios::binary);

	if (file)
	{
		// move the file pointer to the end of the file
		file.seekg(0, std::ios::end);
		
		//file.tellg is used to get the position of the file pointer, which also means the size of the file in this case
		fileContents.resize(file.tellg());

		//move the file pointer to the beginning of the file
		file.seekg(0, std::ios::beg);

		//read this file into string
		file.read(&fileContents[0], fileContents.size());
		file.close();

		return true;
	}
	return false;
}
