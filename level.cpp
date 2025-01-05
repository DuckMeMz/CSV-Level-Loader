#include "level.h"
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
#include <algorithm> 
#include <sstream> 
#include <fstream> 

//If the header name TILESET_HEIGTH is spelt correctly as TILESET_HEIGHT an error will be thrown: "DEBUG: Failed to access m_headerIndex map key in ParseLayer. Possibly a misspelt header/header index!". If this occurs please change the header name HEADER_TILESET_HEIGHT to the correct spelling as this program is made to run on the LevelOne_conf.csv with the mispelt header. Thank you :D

//For each of the object loading functions: ParseBoundary, ParseBackground, ParseEnemy etc. It could be more efficient by making loading straight from the vector into the object creation. I looked into this methdod but chose to not do it this way as loading it into seperate variables then into the function improves readability greatly. For such a system as loading, not being called every game loop, I belive this is acceptable and won't hinder performance much.

//An example of what it would look like if not loaded into seperate variables: 
//TileLayer layer = TileLayer(Dimension2D{ std::stoi(inputRow[m_headerIndex[HEADER_TILE_WIDTH]]), std::stoi(inputRow[m_headerIndex[HEADER_TILE_HEIGHT]]) }, Dimension2D{ std::stoi(inputRow[m_headerIndex[HEADER_WIDTH]]), std::stoi(inputRow[m_headerIndex[HEADER_HEIGHT]]); }, std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);, inputRow[m_headerIndex[HEADER_FILE]];, StringToBool(inputRow[m_headerIndex[HEADER_HAS_COLLISION]]););


Level::Level(int inLevelNum, const std::string& inFileName) : m_levelNum(inLevelNum), FILE_NAME(inFileName){}

Level::~Level() {}

//Intakes the configFile's name which is set in the constructor, loads the CSV file into a 2D vector called m_allLines. Returns true on completion or false for error state.
bool Level::ParseConfigFile(const std::string& fileName)
{
	//Opens file stream
	std::ifstream confFile;
	confFile.open(fileName);
	//If confFile fails to open writes to error logger and returns false as error state
	if (!confFile.is_open())
	{
		ErrorLogger::Write("Config File Failed To Open!");
		return false;
	}
	else if (confFile.is_open())
	{
		std::string extractedRow = "";
		std::string extractedCell = "";
		
		//Extracts a single row from CSV
		for (int i = 0; std::getline(confFile, extractedRow); i++)
		{
			//Open stringstream of the extracted row to split it into induvidual cells for loading into the column vector
			std::stringstream stringStreamLine(extractedRow);
			//Push an emprty string vector to hold the columns of this row
			m_allLines.push_back(std::vector<std::string>());
			//Loop through each cell/column in the row seperated by the chosen M_DELIMITER (Default = Comma (,) )
			for (int j = 0; std::getline(stringStreamLine, extractedCell, M_DELIMITER); j++)
			{
				//Changing empty cells to null makes for easier readability & handling in the future
				if (extractedCell == "") { extractedCell = "Null"; }
				//Push the extracted cells into the vector at the current iterations row number
				m_allLines[i].push_back(extractedCell);
			}
		}
	}
	//Check for confFile I/O fails
	else if (confFile.fail()) 
	{ 
		ErrorLogger::Write("Config File Fail!");
		return false;  
	}

	//End Read
	confFile.close();
	return true;
}
//Returns true if the given index M_DIGIT_VARIABLE_INDEX 's first character is equal to a digit or a "-". This means that it's a header. M_DIGIT_VARIABLE_INDEX needs to be set to a digit only variables like xPosition NOT something like FilePath.

bool Level::IsHeader(const std::vector<std::string>& inputRow)
{
	//Literal value here is used for checking 1 character into the string. 
	return !std::any_of(inputRow[M_DIGIT_VARIABLE_INDEX].begin(), inputRow[M_DIGIT_VARIABLE_INDEX].begin() + 1, ::isdigit) != (inputRow[M_DIGIT_VARIABLE_INDEX].find("-") + 1);
}

//This is called when current line is a header, to load the headers into the m_headerIndex map 
//This function does not check the input data. 
void Level::ParseHeaders(const std::vector<std::string>& inputRow)
{
	//Resets iteration to make sure the header index is accurate
	int i = 0;
	for (auto& cell : inputRow)
	{
		//Skips the cell if it's Null
		if (cell == "Null")
		{ 
			continue;
		}
		//Loads the headers in configeration below:
		//		  Key : Value
		//Header Name : Iteration (Index)
		m_headerIndex[cell] = i;
		i++;
	}
}

//This function is for parsing the layer line. Called when the objectType is equal to "TILE". Takes input from the current row and loads the Tile aswell as the TileSet.
void Level::ParseLayer(const std::vector<std::string>& inputRow)
{
	try {
		//Tile
		int width = std::stoi(inputRow[m_headerIndex[HEADER_WIDTH]]);
		int height = std::stoi(inputRow[m_headerIndex[HEADER_HEIGHT]]);
		int tileWidth = std::stoi(inputRow[m_headerIndex[HEADER_TILE_WIDTH]]);
		int tileHeight = std::stoi(inputRow[m_headerIndex[HEADER_TILE_HEIGHT]]);
		double scale = std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);
		std::string fileName = inputRow[m_headerIndex[HEADER_FILE]];
		bool hasCollision = StringToBool(inputRow[m_headerIndex[HEADER_HAS_COLLISION]]);

		//Tileset
		std::string tileSet = inputRow[m_headerIndex[HEADER_TILESET]];
		int tilesetWidth = std::stoi(inputRow[m_headerIndex[HEADER_TILESET_WIDTH]]);
		int tilesetHeight = std::stoi(inputRow[m_headerIndex[HEADER_TILESET_HEIGHT]]);


		TileLayer layer = TileLayer(Dimension2D{ tileWidth, tileHeight }, Dimension2D{ width, height }, scale, fileName, hasCollision);
		layer.LoadTileSet(tileSet, { tilesetWidth, tilesetHeight });
		layer.Load();
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParseLayer. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the Boundary objects. Called when objectType is equal to "BOUNDARY". Only handles BOUNDARY not BOUNDARY_FINISH. Intakes current vector row and loads a boundary object pointer into the objectManager.
void Level::ParseBoundary(const std::vector<std::string>& inputRow )
{
	try {
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		int width = std::stoi(inputRow[m_headerIndex[HEADER_WIDTH]]);
		int height = std::stoi(inputRow[m_headerIndex[HEADER_HEIGHT]]);

		BoundaryObject* boundery = new BoundaryObject(ObjectType::BOUNDARY);
		boundery->Initialise(Vector2D{ xPos, yPos }, width, height);
		ObjectManager::instance.AddItem(boundery);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParseBoundery. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the background of the level. Called when objectType is equal to "BACKGROUND". Intakes current vector row and loads a background object pointser into the objectManager.
void Level::ParseBackground(const std::vector<std::string>& inputRow)
{
	try {
		std::string image = inputRow[m_headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[m_headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);

		Scene* background = new Scene(ObjectType::BACKGROUND);
		background->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);
		ObjectManager::instance.AddItem(background);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParseBackground. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the pickup object. Called when objectType is equal to "PICKUP". Intakes current vector row and loads a pickup object pointer into the objectManager.
void Level::ParsePickup(const std::vector<std::string>& inputRow)
{
	try {
		std::string image = inputRow[m_headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[m_headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);

		PickUp* pickUp = new PickUp(ObjectType::PICK_UP);
		pickUp->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);


		ObjectManager::instance.AddItem(pickUp);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParsePickup. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the fuel pump object. Called when objectType is equal to "FUELPUMP". Intakes current vector row and loads a fuelpump object along with its animation as a pointer into the objectManager.
void Level::ParseFuelPump(const std::vector<std::string>& inputRow)
{
	try {
		//Object
		std::string image = inputRow[m_headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[m_headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);

		FuelPump* fuelPump = new FuelPump(ObjectType::FUEL_PUMP);
		fuelPump->Initialise(image.c_str(), Vector2D{ xPos,yPos }, angle, scale);

		//Animation
		fuelPump->SetFuelValue(std::stoi(inputRow[m_headerIndex[HEADER_VALUE]]));
		fuelPump->SetFrameCount(std::stoi(inputRow[m_headerIndex[HEADER_FRAME_COUNT]]));
		fuelPump->SetAnimationSpeed(std::stod(inputRow[m_headerIndex[HEADER_ANIM_SPEED]]));

		ObjectManager::instance.AddItem(fuelPump);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParseFuelPump. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the boundary finish object. Called when object type is equal to "BOUNDARY_FINISH". Intakes current vector row and loads a boundary_finish object into the objectManager.
void Level::ParseFinish(const std::vector<std::string>& inputRow)
{
	try {
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		int width = std::stoi(inputRow[m_headerIndex[HEADER_WIDTH]]);
		int height = std::stoi(inputRow[m_headerIndex[HEADER_HEIGHT]]);

		BoundaryObject* boundary_finish = new BoundaryObject(ObjectType::BOUNDARY_FINISH);
		boundary_finish->Initialise(Vector2D{ xPos, yPos }, width, height);
		ObjectManager::instance.AddItem(boundary_finish);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParseFinish. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the enemy object. Called when objectType is equal to "ENEMY". Loads the enemy object and projectiles. Loads an enemy object pointer into the objectManager.
void Level::ParseEnemy(const std::vector<std::string>& inputRow)
{
	try {
		//Enemy object variables
		std::string image = inputRow[m_headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[m_headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);
		bool hFlip = StringToBool(inputRow[m_headerIndex[HEADER_IS_FLIPPED_H]]);
		bool vFlip = StringToBool(inputRow[m_headerIndex[HEADER_IS_FLIPPED_V]]);

		//Created enemy object
		Enemy* enemy = new Enemy(ObjectType::ENEMY);
		enemy->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);
		//Checks for H or V flip
		enemy->FlipHorizontal(hFlip);
		enemy->FlipVertical(true);

		//Enemy Projectile variables
		double projectileLifeTime = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_LIFETIME]]);
		double projectileOffsetX = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_OFFSET_X]]);
		double projectileOffsetY = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_OFFSET_Y]]);
		double shootDelay = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_DELAY]]);
		std::string projectileImage = inputRow[m_headerIndex[HEADER_PROJECTILE_IMAGE]];
		double projectileScale = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_SCALE]]);
		double projectileSpeed = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_SPEED]]);
		double projectileBearing = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_BEARING]]);

		//Loads enemy projectile variables
		enemy->LoadProjectiles(projectileLifeTime, Vector2D{ projectileOffsetX, projectileOffsetY }, projectileBearing, shootDelay, projectileImage, projectileScale, projectileSpeed);
		ObjectManager::instance.AddItem(enemy);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParseEnemy. Possibly a misspelt header/header index!");
		return;
	}
}
//Parses the player object. Called when objectType is equal to "PLAYER". Loads the player object and projectile pointer into the objectManager. 
void Level::ParsePlayer(const std::vector<std::string>& inputRow)
{
	try {
		//Player object variables
		std::string image = inputRow[m_headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[m_headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[m_headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[m_headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[m_headerIndex[HEADER_SCALE]]);

		Player* player = new Player(ObjectType::PLAYER);
		player->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);

		//Projectile object variables
		double projectileLifeTime = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_LIFETIME]]);
		double projectileOffsetX = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_OFFSET_X]]);
		double projectileOffsetY = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_OFFSET_Y]]);
		double shootDelay = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_DELAY]]);
		std::string projectileImage = inputRow[m_headerIndex[HEADER_PROJECTILE_IMAGE]];
		double projectileScale = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_SCALE]]);
		double projectileSpeed = std::stod(inputRow[m_headerIndex[HEADER_PROJECTILE_SPEED]]);

		player->LoadProjectiles(projectileLifeTime, Vector2D{ projectileOffsetX, projectileOffsetY }, shootDelay, projectileImage, projectileScale, projectileSpeed);
		ObjectManager::instance.AddItem(player);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access m_headerIndex map key in ParsePlayer. Possibly a misspelt header/header index!");
		return;
	}
}

//Intakes a string and outputs true or false based on what the string is. Anything other than True, true or TRUE will return false.
bool Level::StringToBool(const std::string& inpString)
{
	//If a more versatile StoB is needed you can change the whole input into lower case then compare, this is unnecessary for this application if CSV file standards are kept.
	if (inpString == "True" || inpString == "true" || inpString == "TRUE") { return true; }
	//If it's anything other returns false
	else { return false; }
}

//Main function that is called in GameManger.cpp. First parses the header with the inputted FILE_NAME set by the constructor. 
//Iterates through each row checking for header with IsHeader(), Parses headers into m_headerIndex with ParseHeaders()if true, checks the object types if it's not a header.
//Returns true if completed sucessfully 
bool Level::LoadLevel()
{
	//Make sure the config file parsed correctly before continuing
	if (ParseConfigFile(FILE_NAME) == true)
	{
		for (auto& row : m_allLines) 
		{
			//If the current row is a header, clears the current headerIndex, then parses the headers into the m_headerIndex map
			if (IsHeader(row))
			{
				m_headerIndex.clear();
				ParseHeaders(row);
			}
			else
			{
				try {
					//Easily scalable to add more game objects
					if (row[m_headerIndex[HEADER_TYPE]] == "TILE") ParseLayer(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "BOUNDARY") ParseBoundary(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "BOUNDARY_FINISH") ParseFinish(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "BACKGROUND") ParseBackground(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "PICKUP") ParsePickup(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "FUEL_PUMP") ParseFuelPump(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "ENEMY") ParseEnemy(row);
					else if (row[m_headerIndex[HEADER_TYPE]] == "PLAYER") ParsePlayer(row);
					else
					{
						//If an unexpected type is encountered logs error but doesn't enter fail state. 
						ErrorLogger::Write("Unexpected Header Type Encountered!");
						return true;
					}
				}
				//If a headerType is invalid, logs error and enters fail state.
				catch (std::exception e) {
					ErrorLogger::Write("Failed to access m_headerIndex map key in LoadLevel. Possibly a misspelt object/layer type");
					return false;
				}
			}
		}
	}
	else 
	{ 
		//Doesn't load the level if configFile parsing fails
		//No error message as it's sent to the ErrorLogger in the ParseConfigFile function.
		return false; 
	}
	//Load level if everything completes properly
	return true;
}