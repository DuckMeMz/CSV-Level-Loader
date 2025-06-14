#include "TileLayer.h"
#include "../Engine/ObjectManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

TileLayer::TileLayer(Dimension2D tileDimensions, Dimension2D rowsColumns, double scale, const std::string fileName, bool isCollisionLayer)
	: m_tileDimensions(tileDimensions), m_rowsColumns(rowsColumns),
	m_scale(scale), m_isCollisionLayer(isCollisionLayer)
{
	//set the width and height of the layer based on the number of rows and columns,
	//the tile width and height and the scale
	m_layerDimensions.width = static_cast<int>(round(rowsColumns.width * tileDimensions.width * scale));
	m_layerDimensions.height = static_cast<int>(round(rowsColumns.height * tileDimensions.height * scale));
	FILE_NAME = fileName;
}

Result TileLayer::Load()
{
	std::fstream mapFile;
	mapFile.open(FILE_NAME);
	if (!mapFile.is_open())
	{
		std::cerr << "Error opening the map file: " << FILE_NAME << std::endl;
		return Result::FAILURE;
	}
	//loop through the mapfile creating tiles and adding them to the m_gameObjects list
	//Could change this to adding Layer objects
	std::string tileMapLine;

	for (int row = 0; std::getline(mapFile, tileMapLine); row++)
	{
		std::istringstream sstream(tileMapLine);
		std::string token;
		for (int col = 0; std::getline(sstream, token, ','); col++)
		{
			int number = std::stoi(token);
			if (number != -1)
			{
				//work out the source location of the image in the tileset
				int tileSetRow = number / m_tileSetDimensions.width;
				int tileSetColumn = number % m_tileSetDimensions.width;
				int centreX = (tileSetColumn * (m_tileDimensions.width)) + (m_tileDimensions.width / 2);
				int centreY = (tileSetRow * (m_tileDimensions.height)) + (m_tileDimensions.height / 2);

				//work out the destination of the tiles on the map 
				//These are 0,0 at top right
				int dstX = static_cast<int>(round(col * (m_tileDimensions.width - 2) * m_scale));
				int dstY = static_cast<int>(round(row * (m_tileDimensions.height - 2) * m_scale));
				
				//Number of tiles
				//int tilesHorizontal = m_layerDimensions.width / m_tileDimensions.width;

				//shift 0,0 to the middle
				dstX = dstX - (m_layerDimensions.width / 2);
				dstY = (m_layerDimensions.height / 2) - dstY;;

				//Create the tile
				ObjectType tileType = m_isCollisionLayer ? ObjectType::COLLISION_TILE : ObjectType::SCENE;
				Tile* tile = new Tile(tileType);
				tile->Initialise(m_tileSet.c_str(), Vector2D(dstX, dstY), 0.0, m_scale);
				tile->SetTileDimensions(m_tileDimensions.width, m_tileDimensions.height);
				tile->SetSourcePosition(Vector2D(centreX, centreY));
				
				//add it to the GameObjects list
				ObjectManager::instance.AddItem(tile);
			}
		}
	}
	mapFile.close();
	return Result::SUCCESS;
}

void TileLayer::LoadTileSet(const std::string fileName, Dimension2D tileSetDimensions)
{
	m_tileSetDimensions = tileSetDimensions;
	m_tileSet = fileName;
}

