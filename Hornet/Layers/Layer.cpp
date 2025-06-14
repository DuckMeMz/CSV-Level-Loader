#include "Layer.h"

Layer::Layer()
{
	m_scale = 1.0;
}

Layer::Layer(Dimension2D dimensions, const std::string fileName)
{
	m_dimensions = dimensions;
	FILE_NAME = fileName;
}
