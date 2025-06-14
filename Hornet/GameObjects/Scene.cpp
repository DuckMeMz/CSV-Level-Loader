#include "Scene.h"

Scene::Scene(ObjectType objType) : GameObject(objType)
{
	m_width = 0;
	m_height = 0;
}

void Scene::Initialise(const char* imagefile, Vector2D position, double angle, double scale)
{
	GameObject::Initialise(imagefile, position, angle, scale);
	m_width = static_cast<int>(HtGraphics::instance.GetWidthOfPicture(m_images[m_imageNumber]));
	m_height = static_cast<int>(HtGraphics::instance.GetHeightOfPicture(m_images[m_imageNumber]));
}

void Scene::Update(double frametime)
{
	//No need to do anything here
}

IShape2D& Scene::GetCollisionShape()
{
	//We need to implement this function but this does nothing as collision is disabled
	return m_collision;
}
