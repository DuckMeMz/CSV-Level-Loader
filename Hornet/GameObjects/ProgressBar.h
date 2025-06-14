#pragma once
#include "GameObject.h"
//#include "../Events/EventBus.h"
//#include "../Events/PlayerDamageEvent.h"

class ProgressBar : public GameObject
{
public:
	ProgressBar(ObjectType objType);

	void Initialise(Vector2D position, int width, int height, ProgressBarType type);
	void Update(double frametime) override;
	IShape2D& GetCollisionShape() override;

	void Render() override;

	void SetRange(int minValue, int maxValue, int initialValue);
	void SetBackgroundColour(Colour backgroundColour);
	void SetBorderColour(Colour borderColor);
	void SetFillColour(Colour fillColour);
	void SetProgress(int progress);
	void ChangeProgress(int amount);
	void SetLabelImage(const std::string& fileName, double scale);

	ProgressBarType GetType();


private:
	int m_height;
	int m_width;
	ProgressBarType m_type;
	Rectangle2D m_bgRectangle;
	Rectangle2D m_fillRectangle;
	Rectangle2D m_borderRectangle;
	int m_minimumLevel;
	int m_maximumLevel;
	int m_fillProgress;
	Colour m_bgColour;
	Colour m_borderColour;
	Colour m_fillColour;
	int m_labelWidth;
	int m_labelHeight;
	Rectangle2D m_collisionShape;
	const int PADDING = 10;

};

