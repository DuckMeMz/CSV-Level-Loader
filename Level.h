#pragma once
#include "level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include "../Layers/Layer.h"
#include "../Engine/ErrorLogger.h"
#include "../Engine/ObjectManager.h"
#include "../GameObjects/objecttypes.h"
#include "../GameObjects/BoundaryObject.h"
#include "../Layers/TileLayer.h"
#include "../GameObjects/Scene.h"
#include "../GameObjects/PickUp.h"
#include "../GameObjects/FuelPump.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Player.h"



class Level
{
	int levelNum;
	const std::string fileName;

	std::vector<std::vector<std::string>> allLines;
	std::map<std::string, int> headerIndex;

	const char delimiter = ',';

	/*
	Justification:
	While this is a lot of variables to be created all together, with 31 strings (Keeping sensible string lengths) should only amount to ~2kb, which isn't too bad for how much versatility this provides.
	Usage:
	Strings: If you want to change the CSV file headers you are able to change them here
	*/

	//Header Naming
	const std::string HEADER_TYPE = "LAYER_TYPE";
	const std::string HEADER_X_POSITION = "X_POSITION";
	const std::string HEADER_Y_POSITION = "Y_POSITION";
	const std::string HEADER_WIDTH = "WIDTH";
	const std::string HEADER_HEIGHT = "HEIGHT";
	const std::string HEADER_SCALE = "SCALE";
	const std::string HEADER_FILE = "FILE";
	const std::string HEADER_TILE_WIDTH = "TILE_WIDTH";
	const std::string HEADER_TILE_HEIGHT = "TILE_HEIGHT";
	const std::string HEADER_TILESET = "TILESET";
	const std::string HEADER_TILESET_WIDTH = "TILESET_WIDTH";
	const std::string HEADER_TILESET_HEIGHT = "TILESET_HEIGTH"; //Height mispell as default, correct to correct spelling if nessessary
	const std::string HEADER_HAS_COLLISION = "HAS_COLLISION";
	const std::string HEADER_OBJECT_TYPE = "OBJECT_TYPE";
	const std::string HEADER_IMAGE = "IMAGE";
	const std::string HEADER_ANGLE = "ANGLE";
	const std::string HEADER_IS_FLIPPED_H = "IS_FLIPPED_H";
	const std::string HEADER_IS_FLIPPED_V = "IS_FLIPPED_V";
	const std::string HEADER_HIT_DAMAGE = "HIT_DAMAGE";
	const std::string HEADER_VALUE = "VALUE";
	const std::string HEADER_IS_ANIMATED = "IS_ANIMATED";
	const std::string HEADER_FRAME_COUNT = "FRAME_COUNT";
	const std::string HEADER_ANIM_SPEED = "ANIM_SPEED";
	const std::string HEADER_PROJECTILE_SCALE = "PROJECTILE_SCALE";
	const std::string HEADER_PROJECTILE_BEARING = "PROJECTILE_BEARING";
	const std::string HEADER_PROJECTILE_OFFSET_X = "PROJECTILE_OFFSET_X";
	const std::string HEADER_PROJECTILE_OFFSET_Y = "PROJECTILE_OFFSET_Y";
	const std::string HEADER_PROJECTILE_IMAGE = "PROJECTILE_IMAGE";
	const std::string HEADER_PROJECTILE_LIFETIME = "PROJECTILE_LIFETIME";
	const std::string HEADER_PROJECTILE_DELAY = "PROJECTILE_DELAY";
	const std::string HEADER_PROJECTILE_SPEED = "PROJECTILE_SPEED";

public:

	Level(int inLevelNum, const std::string& inFileName);

	~Level();

	bool parseConfigFile(const std::string& fileName);
	bool isHeader(const std::vector<std::string>& inputVector);
	void parseHeaders(const std::vector<std::string>& inputRow);

	void ParseLayer(const std::vector<std::string>& inputRow);
	void ParseBoundary(const std::vector<std::string>& inputRow);
	void ParseFinish(const std::vector<std::string>& inputRow);
	void ParseBackground(const std::vector<std::string>& inputRow);
	void ParsePickup(const std::vector<std::string>& inputRow);
	void ParseFuelPump(const std::vector<std::string>& inputRow);
	void ParseEnemy(const std::vector<std::string>& inputRow);
	void ParsePlayer(const std::vector<std::string>& inputRow);
	std::string GetValue(const std::string& header, const std::vector<std::string>& inputRow);

	//std::vector<std::string> split(const std::string& string, char delimiter);
	bool StringToBool(const std::string& inpString);

	bool LoadLevel();
};