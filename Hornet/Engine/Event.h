#pragma once
#include "vector2D.h"
enum EventType { NONE, REFUEL_REQUEST, PICKUP_REQUEST, EXPLOSION, OBJECTCREATED, OBJECTDESTROYED, MISSIONCOMPLETE };

class GameObject;

struct Event
{
	GameObject* pSource;
	EventType type;
	Vector2D position;
	double data1;
	double data2;
};

