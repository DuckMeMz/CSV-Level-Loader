#pragma once
#include <unordered_map> 
#include <string>
#include <vector>



class Level
{
	int m_levelNum;
	//Keeping inline with engine naming conventions consts are exempt from m_ naming scheme
	const std::string FILE_NAME;
	/*
	Justification for 2D vector: A 2D vector is optimal in this case, compared to a linked list or 2D array. Unknown CSV size at runtime rules out an array unless you set a huge literal number array size being extremly memory inefficient. A linked list while it's size can be altered during runtime, it's not optimal in this situation as element access is in linear time while vectors are constant. Deletion isn't a factor in this case as nothing is being deleted in this process. A 2D vector while slower compared to a single vector, increases readability tremendously and especially while loading times are still sub 2 second this isn't a problem currently. 
	*/
	std::vector<std::vector<std::string>> m_allLines;

	/*
	Justification for unordered map: Compared to map, an unordered map has a better time complexity. Unordered O(n) vs Ordered O(Log(N)), which is especially vital when a lot of element insertion, deletion and search is taking place in this unordered_map. While space might be worse than a traditional map I belive the speed increase should account for this. 
	*/
	std::unordered_map<std::string, int> m_headerIndex;

	//Making these literal values into single variables allows easy altering in the future.
	//This int should be set to a digit cell such as Xpos. NOT something like File location. If it's set incorrectly IsHeader function will return incorrect values.
	const int M_DIGIT_VARIABLE_INDEX = 1; //Default 1 | Xpos variable

	const char M_DELIMITER = ','; // Default "," | Default for a CSV
	

	/*
	These variables are used to find the index of the variable in the row vector. Other methods were considered but I belive this is the most scalable and simple way.
	While this is a lot of variables to be created all together, with 31 strings (Keeping sensible string lengths) should only amount to ~2kb, which I belive is acceptable for how much versatility this provides. This amount of variable are also found in the player header file and justified there with "Player constants there's only one player so this is fine", while this is strings compared to doubles and ints, there is only one of these classes created so I belive this is acceptable.

	m_ naming conventions not found on the const variables in player.h so it's not done here 

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
	const std::string HEADER_TILESET_HEIGHT = "TILESET_HEIGTH"; //Height mispell as default
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


	//Helper Functions - These helper functions are here for simplicity within the other functions, keeping repeatable code seperate keeps everything readable and easy development in future.
	bool ParseConfigFile(const std::string& fileName);
	bool IsHeader(const std::vector<std::string>& inputVector);
	void ParseHeaders(const std::vector<std::string>& inputRow);
	bool StringToBool(const std::string& inpString);


	//Object Parsing - Induvidual functions instead of functions like ParseGameObject or ParseLayerObject makes it a lot easier to scale, maintain and read. Scaling for the rest of the game object types as found in objecttypes.h like reactor is extremly simple and shouldn't affect anything else in the process aas they are all kept seperate.
	void ParseLayer(const std::vector<std::string>& inputRow);
	void ParseBoundary(const std::vector<std::string>& inputRow);
	void ParseFinish(const std::vector<std::string>& inputRow);
	void ParseBackground(const std::vector<std::string>& inputRow);
	void ParsePickup(const std::vector<std::string>& inputRow);
	void ParseFuelPump(const std::vector<std::string>& inputRow);
	void ParseEnemy(const std::vector<std::string>& inputRow);
	void ParsePlayer(const std::vector<std::string>& inputRow);

	//Main Function
	bool LoadLevel();
};