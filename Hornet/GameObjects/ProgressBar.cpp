#include "ProgressBar.h"
#include "../Engine/HtCamera.h"




ProgressBar::ProgressBar(ObjectType objType) : GameObject(objType)
{
	SetDrawDepth(3);
}

void ProgressBar::Initialise(Vector2D position, int width, int height, ProgressBarType type)
{
	m_position = position;
	m_width = width;
	m_height = height;
	m_type = type;
	m_minimumLevel = 0;
	m_maximumLevel = 100;
	m_fillProgress = m_maximumLevel;
	m_bgColour = Colour(255, 0, 0, 0);
	m_borderColour = Colour(255, 255, 255, 255);
	m_fillColour = Colour(255, 0, 255, 0);
}

void ProgressBar::Update(double frametime)
{
	if (IsActive())
	{
		Vector2D bottomLeft = Vector2D(m_position.XValue - m_width / 2, m_position.YValue - m_height / 2);
		Vector2D topRight = Vector2D(m_position.XValue + m_width / 2, m_position.YValue + m_height / 2);
		m_bgRectangle.PlaceAt(bottomLeft, topRight);
		m_borderRectangle.PlaceAt(bottomLeft, topRight);


		bottomLeft = Vector2D(m_position.XValue - m_width / 2, m_position.YValue - m_height / 2);
		int fillProgress = static_cast<int>((m_fillProgress / static_cast<float>(m_maximumLevel)) * (m_width));
		topRight = Vector2D((m_position.XValue - m_width / 2) + fillProgress, m_position.YValue + m_height / 2);
		m_fillRectangle.PlaceAt(bottomLeft, topRight);
	}
}

IShape2D& ProgressBar::GetCollisionShape()
{
	return m_collisionShape;
}

void ProgressBar::Render()
{
	HtCamera::instance.UseCamera(false);

	if (m_imageNumber >= 0)
	{
		Vector2D labelPosition = Vector2D((m_position.XValue - m_width / 2) - (m_labelWidth / 2) - PADDING, m_position.YValue);
		HtGraphics::instance.DrawAt(labelPosition, m_images[m_imageNumber], m_scale);
	}
	HtGraphics::instance.FillRect(m_bgRectangle, m_bgColour);
	HtGraphics::instance.FillRect(m_fillRectangle, m_fillColour);
	HtGraphics::instance.DrawRect(m_borderRectangle, m_borderColour);

	HtCamera::instance.UseCamera(true);
}

void ProgressBar::SetRange(int minValue, int maxValue, int initialValue)
{
	m_minimumLevel = minValue;
	m_maximumLevel = maxValue;
	m_fillProgress = initialValue;
}

void ProgressBar::SetBackgroundColour(Colour backgroundColour)
{
	m_bgColour = backgroundColour;
}

void ProgressBar::SetBorderColour(Colour borderColor)
{
	m_borderColour = borderColor;
}

void ProgressBar::SetFillColour(Colour fillColour)
{
	m_fillColour = fillColour;
}

void ProgressBar::SetProgress(int progress)
{
	if (progress < m_minimumLevel)
	{
		m_fillProgress = m_minimumLevel;
	}
	else if (progress > m_maximumLevel)
	{
		m_fillProgress = m_maximumLevel;
	}
	else
	{
		m_fillProgress = progress;
	}
}

void ProgressBar::ChangeProgress(int amount)
{
	if ((m_fillProgress + amount) < m_minimumLevel)
	{
		m_fillProgress = m_minimumLevel;
	}
	else if ((m_fillProgress + amount) > m_maximumLevel)
	{
		m_fillProgress = m_maximumLevel;
	}
	else
	{
		m_fillProgress += amount;
	}
}

void ProgressBar::SetLabelImage(const std::string& fileName, double scale)
{
	m_scale = scale;
	LoadImage(fileName.c_str());
	m_labelWidth = static_cast<int>(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]) * m_scale);
	m_labelHeight = static_cast<int>(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]) * m_scale);
}

ProgressBarType ProgressBar::GetType()
{
	return m_type;
}


