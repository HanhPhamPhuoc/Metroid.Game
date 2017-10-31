#pragma once


#include <unordered_map>
#include "rapidxml.hpp"
#include "TMXMap.h"
#include "TMXTileLayer.h"
#include "TMXTileSet.h"

class TMXLoader
{
private:

	//use unordered_map for fast access purpose
	std::unordered_map<std::string, TMXMap*> _MapContainer;


	//private functions
	void LoadMapSettings(TMXMap* map, rapidxml::xml_node<> *parentNode);
	void LoadTileSets(TMXMap* map, rapidxml::xml_node<> *parentNode, const std::string& folderPath);
	void LoadLayers(TMXMap* map, rapidxml::xml_node<> *parentNode);
	void LoadObjectGroups(TMXMap* map, rapidxml::xml_node<> *parentNode);

public:
	TMXLoader();
	~TMXLoader();

	void AddMap(const std::string& mapName, const std::string& filePath);
	TMXMap* GetMap(const std::string& mapName) const;

};
