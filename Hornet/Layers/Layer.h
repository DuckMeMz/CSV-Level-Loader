#pragma once
#include <string>
#include "../Engine/Vector2D.h"
#include "../Engine/Result.h"

//Create a struct that contains 2 ints for tile sizes and level dimensions
struct Dimension2D {
	int width = 0;
	int height = 0;
};

class Layer
{
public:
	Layer();
	virtual ~Layer() {};
	Layer(Dimension2D dimensions, const std::string fileName);
	virtual Result Load() = 0;
	void SetWidth(int width) { m_dimensions.width = width; }
	void SetHeight(int height) { m_dimensions.height = height; }

protected:
	Dimension2D m_dimensions;
	Vector2D m_position;
	double m_scale;
	std::string FILE_NAME;


};

