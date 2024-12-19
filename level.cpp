#include "Level.h"

Level::Level(int inLevelNum, const std::string& inFileName) : levelNum(inLevelNum), fileName(inFileName){}

Level::~Level() {}

bool Level::parseConfigFile(const std::string& fileName)
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
			allLines.push_back(std::vector<std::string>());
			//Loop through each cell/column in the row seperated by the chosen delimiter (Default = Comma (,) )
			for (int j = 0; std::getline(stringStreamLine, extractedCell, delimiter); j++)
			{
				//Changing empty cells to null makes for easier readability & handling in the future
				if (extractedCell == "") { extractedCell = "Null"; }
				//Push the extracted cells into the vector at the current iterations row number
				allLines[i].push_back(extractedCell);
			}
		}
	}
	//Check for confFile I/O fails
	else if (confFile.fail()) 
	{ 
		ErrorLogger::Write("Config File Fail!");
		//Returns false as error state
		return false;  
	}

	//End Read
	confFile.close();
	return true;
}

bool Level::isHeader(const std::vector<std::string>& inputRow)
{
	//If it finds the first char to be a digit return 0, if not returns 1, if first char is minus returns 0
	return !std::any_of(inputRow[1].begin(), inputRow[1].begin() + 1, ::isdigit) != (inputRow[1].find("-") + 1);
}

//This is called when current line is a header, to load the headers into the headerIndex map 
void Level::parseHeaders(const std::vector<std::string>& inputRow)
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
		//Loads the header value into header map in the order it is found with header name as the key and iteration as value
		headerIndex[cell] = i;
		i++;
	}
}

void Level::ParseLayer(const std::vector<std::string>& inputRow)
{
	try {
		//Tile
		int width = std::stoi(inputRow[headerIndex[HEADER_WIDTH]]);
		int height = std::stoi(inputRow[headerIndex[HEADER_HEIGHT]]);
		int tileWidth = std::stoi(inputRow[headerIndex[HEADER_TILE_WIDTH]]);
		int tileHeight = std::stoi(inputRow[headerIndex[HEADER_TILE_HEIGHT]]);
		double scale = std::stod(inputRow[headerIndex[HEADER_SCALE]]);
		std::string fileName = inputRow[headerIndex[HEADER_FILE]];
		bool hasCollision = StringToBool(inputRow[headerIndex[HEADER_HAS_COLLISION]]);

		//Tileset
		std::string tileSet = inputRow[headerIndex[HEADER_TILESET]];
		int tilesetWidth = std::stoi(inputRow[headerIndex[HEADER_TILESET_WIDTH]]);
		int tilesetHeight = std::stoi(inputRow[headerIndex[HEADER_TILESET_HEIGHT]]);


		TileLayer layer = TileLayer(Dimension2D{ tileWidth, tileHeight }, Dimension2D{ width, height }, scale, fileName, hasCollision);
		layer.LoadTileSet(tileSet, { tilesetWidth, tilesetHeight });
		layer.Load();
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParseLayer. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParseBoundary(const std::vector<std::string>& inputRow )
{
	try {
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		int width = std::stoi(inputRow[headerIndex[HEADER_WIDTH]]);
		int height = std::stoi(inputRow[headerIndex[HEADER_HEIGHT]]);

		BoundaryObject* boundery = new BoundaryObject(ObjectType::BOUNDARY);
		boundery->Initialise(Vector2D{ xPos, yPos }, width, height);
		ObjectManager::instance.AddItem(boundery);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParseBoundery. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParseBackground(const std::vector<std::string>& inputRow)
{
	try {
		std::string image = inputRow[headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[headerIndex[HEADER_SCALE]]);

		Scene* background = new Scene(ObjectType::BACKGROUND);
		background->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);
		ObjectManager::instance.AddItem(background);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParseBackground. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParsePickup(const std::vector<std::string>& inputRow)
{
	try {
		std::string image = inputRow[headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[headerIndex[HEADER_SCALE]]);

		PickUp* pickUp = new PickUp(ObjectType::PICK_UP);
		pickUp->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);
		ObjectManager::instance.AddItem(pickUp);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParsePickup. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParseFuelPump(const std::vector<std::string>& inputRow)
{
	try {
		std::string image = inputRow[headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[headerIndex[HEADER_SCALE]]);

		FuelPump* fuelPump = new FuelPump(ObjectType::FUEL_PUMP);
		fuelPump->Initialise(image.c_str(), Vector2D{ xPos,yPos }, angle, scale);
		ObjectManager::instance.AddItem(fuelPump);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParseFuelPump. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParseFinish(const std::vector<std::string>& inputRow)
{
	try {
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		int width = std::stoi(inputRow[headerIndex[HEADER_WIDTH]]);
		int height = std::stoi(inputRow[headerIndex[HEADER_HEIGHT]]);

		BoundaryObject* boundary_finish = new BoundaryObject(ObjectType::BOUNDARY_FINISH);
		boundary_finish->Initialise(Vector2D{ xPos, yPos }, width, height);
		ObjectManager::instance.AddItem(boundary_finish);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParseFinish. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParseEnemy(const std::vector<std::string>& inputRow)
{
	try {
		std::string image = inputRow[headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[headerIndex[HEADER_SCALE]]);
		bool hFlip = StringToBool(inputRow[headerIndex[HEADER_IS_FLIPPED_H]]);
		bool vFlip = StringToBool(inputRow[headerIndex[HEADER_IS_FLIPPED_V]]);

		Enemy* enemy = new Enemy(ObjectType::ENEMY);
		enemy->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);
		enemy->FlipHorizontal(hFlip);
		enemy->FlipVertical(vFlip);

		//Enemy Projectile
		double projectileLifeTime = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_LIFETIME]]);
		double projectileOffsetX = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_OFFSET_X]]);
		double projectileOffsetY = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_OFFSET_Y]]);
		double shootDelay = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_DELAY]]);
		std::string projectileImage = inputRow[headerIndex[HEADER_PROJECTILE_IMAGE]];
		double projectileScale = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_SCALE]]);
		double projectileSpeed = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_SPEED]]);
		double projectileBearing = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_BEARING]]);

		enemy->LoadProjectiles(projectileLifeTime, Vector2D{ projectileOffsetX, projectileOffsetY }, projectileBearing, shootDelay, projectileImage, projectileScale, projectileSpeed);
		ObjectManager::instance.AddItem(enemy);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParseEnemy. Likely a misspelt header/header index!");
		return;
	}
}

void Level::ParsePlayer(const std::vector<std::string>& inputRow)
{
	try {
		//Player
		std::string image = inputRow[headerIndex[HEADER_IMAGE]];
		double xPos = std::stod(inputRow[headerIndex[HEADER_X_POSITION]]);
		double yPos = std::stod(inputRow[headerIndex[HEADER_Y_POSITION]]);
		double angle = std::stod(inputRow[headerIndex[HEADER_ANGLE]]);
		double scale = std::stod(inputRow[headerIndex[HEADER_SCALE]]);

		Player* player = new Player(ObjectType::PLAYER);
		player->Initialise(image.c_str(), Vector2D{ xPos, yPos }, angle, scale);

		//Projectile
		double projectileLifeTime = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_LIFETIME]]);
		double projectileOffsetX = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_OFFSET_X]]);
		double projectileOffsetY = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_OFFSET_Y]]);
		double shootDelay = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_DELAY]]);
		std::string projectileImage = inputRow[headerIndex[HEADER_PROJECTILE_IMAGE]];
		double projectileScale = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_SCALE]]);
		double projectileSpeed = std::stod(inputRow[headerIndex[HEADER_PROJECTILE_SPEED]]);

		player->LoadProjectiles(projectileLifeTime, Vector2D{ projectileOffsetX, projectileOffsetY }, shootDelay, projectileImage, projectileScale, projectileSpeed);
		ObjectManager::instance.AddItem(player);
	}
	catch (std::exception e) {
		ErrorLogger::Write("Failed to access headerIndex map key in ParsePlayer. Likely a misspelt header/header index!");
		return;
	}
}

bool Level::StringToBool(const std::string& inpString)
{
	//If a more versatile StoB is needed you can change the whole input into lower case then compare, this is unnecessary for this application
	if (inpString == "True" || inpString == "true" || inpString == "TRUE") { return true; }
	//If it's anything else then it's false
	else { return false; }
}

bool Level::LoadLevel()
{
	if (parseConfigFile(fileName) == true)
	{
		for (auto& row : allLines) 
		{
			//If the current row is a header parses the headers into the headerIndex map
			if (isHeader(row))
			{
				parseHeaders(row);
			}
			if (!isHeader(row))
			{
				try {
					//Easily possible to add more game objects
					if (row[headerIndex[HEADER_TYPE]] == "TILE") ParseLayer(row);
					else if (row[headerIndex[HEADER_TYPE]] == "BOUNDARY") ParseBoundary(row);
					else if (row[headerIndex[HEADER_TYPE]] == "BOUNDARY_FINISH") ParseFinish(row);
					else if (row[headerIndex[HEADER_TYPE]] == "BACKGROUND") ParseBackground(row);
					else if (row[headerIndex[HEADER_TYPE]] == "PICKUP") ParsePickup(row);
					else if (row[headerIndex[HEADER_TYPE]] == "FUEL_PUMP") ParseFuelPump(row);
					else if (row[headerIndex[HEADER_TYPE]] == "ENEMY") ParseEnemy(row);
					else if (row[headerIndex[HEADER_TYPE]] == "PLAYER") ParsePlayer(row);
					else
					{
						//If unexpected type is encountered logs error but doesn't fail.
						ErrorLogger::Write("Unexpected Header Type Encountered!");
						return true;
					}
				}
				catch (std::exception e) {
					ErrorLogger::Write("Failed to access headerIndex map key in LoadLevel. Likely a misspelt object/layer type");
					return false;
				}
			}
		}
	}
	else 
	{ 
		//Doesn't load the level if configFile parsing fails
		return false; 
	}
	//Load level if everything completes properly
	return true;
}

