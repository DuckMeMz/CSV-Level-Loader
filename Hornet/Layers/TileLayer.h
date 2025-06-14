#pragma once
#include <list>
#include <string>
#include "Layer.h"
#include "../GameObjects/Tile.h"

//ToDo: Need to add some defensive code here

/// <summary>
/// The TileLayer class: this class is derived from Layer and represents the layers of scenary in the game.
/// </summary>
class TileLayer : public Layer
{
public:
    /// <summary>
    /// This constructor creates a tile layer from a csv file representing where tiles should be placed.
    /// Once constructed use the LoadTileSet(...) method to load the file that represents the tiles.
    /// Once the tile set is loaded use the Load(...) method to draw the tiles.
    /// </summary>
    /// <param name="tileDimensions">The dimensions of each tile: Dimension2D</param>
    /// <param name="rowsColumns">The number of tiles per row and column: Dimension2D</param>
    /// <param name="scale">The scale of the tiles: double</param>
    /// <param name="FILE_NAME">The name of the csv file that determines the tile positions: C++ string</param>
    /// <param name="isCollisionLayer">Do the tiles have a collision set on them: bool</param>
    TileLayer(Dimension2D tileDimensions, Dimension2D rowsColumns, double scale, const std::string fileName, bool isCollisionLayer);
    
    /// <summary>
    /// Loads the tileset, this as an image where each section represents an individual tile
    /// </summary>
    /// <param name="FILE_NAME">The tileset image: string</param>
    /// <param name="tileSetDimensions">The dimensions of each tile in the image: Dimension2D</param>
    void LoadTileSet(const std::string fileName, Dimension2D tileSetDimensions);

    /// <summary>
    /// This uses the .csv file and the tileSet to draw the tiles on the screen
    /// </summary>
    /// <returns>Success or not: Result</returns>
    virtual Result Load() override;
    
   
private:
    Dimension2D m_tileDimensions;
    double m_scale;
    Dimension2D m_rowsColumns;
    Dimension2D m_layerDimensions;
    Dimension2D m_tileSetDimensions;
    bool m_isCollisionLayer;
    std::string m_tileSet;
};

