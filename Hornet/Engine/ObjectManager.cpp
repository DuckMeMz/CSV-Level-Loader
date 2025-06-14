#include "HtKeyboard.h"
#include "ObjectManager.h"
#include <algorithm>
#include "HtCamera.h"
#include "../src/GameManager.h"
#include "../GameObjects/GameObject.h"

ObjectManager ObjectManager::instance;

ObjectManager::ObjectManager()
{
	m_currentScene = 0;
	m_slowDownActive = false;
	m_debugActive = false;
	m_frametime = 0;
	m_debugTarget = nullptr;
}

ObjectManager::~ObjectManager()
{
	DeleteAllObjects();
}

void ObjectManager::AddItem(GameObject* pNewItem)
{
	if (pNewItem)
	{
		// Can't change certain things any longer, such as draw depth
		pNewItem->Lock();

		// Use current scene
		pNewItem->SetSceneNumber(m_currentScene);

		// Find position of first item with a higher or equal Draw Depth

		auto it = m_allObjectList.begin();

		while (it != m_allObjectList.end() && (*it)->GetDrawDepth() < pNewItem->GetDrawDepth())
		{
			++it;
		}
		m_allObjectList.insert(it, pNewItem);

		if (pNewItem->IsCollidable())
		{
			m_colliderList.push_back(pNewItem);
		}
		if (pNewItem->CanHandleEvents())
		{
			m_eventHandlerList.push_back(pNewItem);
		}
	}
}

// Returns a pointer to the closest active object of a given type to 
// the specified location in the currentScene
GameObject* ObjectManager::FindClosestObject(Vector2D location, ObjectType ot)
{
	GameObject* pClosest = nullptr;
	double distance2 = 9e9f;	// A very long way away.
	for (GameObject* pNext : m_allObjectList)
	{
		if (pNext &&
			pNext->GetType() == ot &&           // Is the correct type of object
			pNext->GetSceneNumber() == m_currentScene &&  // Is in the current active scene
			pNext->IsActive() &&                          // Is active
			(pNext->GetPosition() - location).magnitudeSquared() < distance2)     // Is closer than the previous closest
		{
			distance2 = (pNext->GetPosition() - location).magnitudeSquared();
			pClosest = pNext;
		}
	}
	return pClosest;
}

std::vector<GameObject*> ObjectManager::GetAllObjectsOfType(ObjectType ot)
{
	std::vector<GameObject*> answer;
	for (GameObject* pNext : m_allObjectList)
	{
		if (pNext &&
			pNext->GetType() == ot &&           // Is the correct type of object
			pNext->GetSceneNumber() == m_currentScene)  // Is in the current active scene
		{
			answer.push_back(pNext);
		}
	}
	return answer;
}

void ObjectManager::DeleteInactiveItems()
{

	// Remove all inactive objects from collider list
	auto itc = std::remove_if(m_colliderList.begin(), m_colliderList.end(), [](GameObject* pGO) { return !pGO->IsActive(); });
	m_colliderList.erase(itc, m_colliderList.end());

	// Remove all inactive objects from event handler list
	auto ite = std::remove_if(m_eventHandlerList.begin(), m_eventHandlerList.end(), [](GameObject* pGO) { return !pGO->IsActive(); });
	m_colliderList.erase(ite, m_eventHandlerList.end());

	// Delete all inactive objects
	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		if (!(*it)->IsActive())
		{
#ifdef _DEBUG
			if(*it == m_debugTarget)
				m_debugTarget = nullptr;
#endif // DEBUG
			delete* it;
			*it = nullptr;
		}
	}

	// Remove all inactive objects from master list
	auto ita = std::remove(m_allObjectList.begin(), m_allObjectList.end(), nullptr);
	m_allObjectList.erase(ita, m_allObjectList.end());
}

void ObjectManager::UpdateAll(double frametime)
{

#ifdef _DEBUG
	if (m_slowDownActive)
		frametime /= 50;
#endif // _DEBUG

	m_frametime = frametime;

	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		if ((*it)->IsActive() && (*it)->GetSceneNumber() == m_currentScene)
		{
			(*it)->Update(float(frametime));
		}
	}
}

void ObjectManager::RenderAll()
{
	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		if ((*it)->IsActive() && (*it)->GetSceneNumber() == m_currentScene)
		{
			(*it)->Render();
		}
	}

}

void ObjectManager::ProcessCollisions()
{
	//  collisions
	auto it1 = m_colliderList.begin();

	for (; it1 != m_colliderList.end(); ++it1)
	{
		auto it2 = it1;
		it2++;

		for (; it2 != m_colliderList.end(); ++it2)
		{
			if ((*it1)->IsActive() && (*it2)->IsActive()
				&& (*it1)->GetSceneNumber() == m_currentScene
				&& (*it2)->GetSceneNumber() == m_currentScene)
			{
				if ((*it1)->HasCollided(**it2))
				{
					(*it1)->ProcessCollision(**it2);
					(*it2)->ProcessCollision(**it1);
				}
			}
		}
	}
}


int ObjectManager::GetNumObjects() const
{
	return (int)m_allObjectList.size();
}

void ObjectManager::DeleteAllObjects()
{
	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		delete* it;
	}

	m_allObjectList.clear();
	m_colliderList.clear();
	m_eventHandlerList.clear();
#ifdef _DEBUG
	m_debugTarget = nullptr;
#endif // DEBUG


}

void ObjectManager::HandleEvent(Event evt)
{
	auto it = m_eventHandlerList.begin();

	for (; it != m_eventHandlerList.end(); ++it)
	{
		(*it)->HandleEvent(evt);
	}

	GameManager::instance.HandleEvent(evt);
}

// Sets the current scene used by the object system
// Only objects in the current scene will be drawn or updated.
void ObjectManager::SetCurrentScene(int sceneNumber)
{
	m_currentScene = sceneNumber;
}

// Returns the number of the current scene.
int ObjectManager::GetCurrentScene()
{
	return m_currentScene;
}

// Sets ALL objects to inactive.
void ObjectManager::DeactivateAll()
{
	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		(*it)->Deactivate();
	}
}

// Deactivates all objects with the corresponding type
void ObjectManager::DeactivateType(ObjectType type)
{
	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		if ((*it) && (*it)->GetSceneNumber() == m_currentScene &&
			(*it)->GetType() == type)
			(*it)->Deactivate();
	}
}

// Deactivates all objects with the specified scene
void ObjectManager::DeactivateScene(int sceneNumber)
{
	auto it = m_allObjectList.begin();

	for (; it != m_allObjectList.end(); ++it)
	{
		if ((*it)->GetSceneNumber() == sceneNumber)
			(*it)->Deactivate();
	}
}


void ObjectManager::SetDebug(bool debugOn)
{
	m_debugActive = debugOn;
}

void ObjectManager::ToggleDebug()
{
	SetDebug(!m_debugActive);
}

void ObjectManager::CycleDebugObject()
{
	if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_HOME))
	{
		SetDebug(true);
		if (m_allObjectList.size() > 0)
			m_debugTarget = *m_allObjectList.begin();
		else
			m_debugTarget = nullptr;
	}
	if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_END))
	{
		SetDebug(false);
			m_debugTarget = nullptr;
	}
	if (HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_INSERT))
	{
		m_slowDownActive = !m_slowDownActive;
	}
	if (m_debugActive && HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_PAGEUP))
	{
		if (m_allObjectList.size() > 0)
		{
			auto it = std::find(m_allObjectList.begin(), m_allObjectList.end(), m_debugTarget);
			if (it == m_allObjectList.end())
			{
				it = m_allObjectList.begin();
				m_debugTarget = *it;
			}
			else
			{
				++it;
				if (it != m_allObjectList.end())
					m_debugTarget = *it;
				else
				{
					m_debugTarget = nullptr;
				}
			}
		}
		else
		{
			m_debugTarget = nullptr;
		}
	}
	if (m_debugActive && HtKeyboard::instance.NewKeyPressed(SDL_SCANCODE_PAGEDOWN))
	{
		if (m_allObjectList.size() > 0)
		{
			auto it = std::find(m_allObjectList.begin(), m_allObjectList.end(), m_debugTarget);
			if (it == m_allObjectList.begin()) // If current target is first, cyle to end
			{
				it = m_allObjectList.end();
			}
			it--;
			m_debugTarget = *it;
		}
		else
		{
			m_debugTarget = nullptr;
		}
	}
}

void ObjectManager::RenderDebug()
{
	if (m_debugActive)
	{
		bool cameraPreviouslyActive = HtCamera::instance.IsCameraActive();
		HtCamera::instance.UseCamera(false);

		// Write general information

		// Number of objects
		HtGraphics::instance.WriteTextAligned(-1400, 990, "Num objects: ", HtGraphics::LIGHTGREEN, 2);
		HtGraphics::instance.WriteIntAligned(-800, 990, (int)m_allObjectList.size(), HtGraphics::LIGHTGREEN, 2);

		// Frame time
		HtGraphics::instance.WriteTextAligned(-1400, 950, "Frame Time: ", HtGraphics::LIGHTGREEN, 2);
		HtGraphics::instance.WriteFloatAligned(-800, 950, m_frametime, HtGraphics::LIGHTGREEN, 2);
		// Frame rate
		HtGraphics::instance.WriteTextAligned(-1400, 910, "Frame Rate: ", HtGraphics::LIGHTGREEN, 2);
		HtGraphics::instance.WriteFloatAligned(-800, 910, 1 / m_frametime, HtGraphics::LIGHTGREEN, 2);
		if (m_slowDownActive)
			HtGraphics::instance.WriteTextAligned(-1400, 870, "Slowdown engaged ", HtGraphics::RED, 2);

		

		// Render the debug target
		RenderDebugObject();

		HtCamera::instance.UseCamera(cameraPreviouslyActive);

		if (m_debugTarget)
		{
			m_debugTarget->RenderDebugShape();
		}
	}
}

void ObjectManager::RenderDebugObject()
{
	// Display information for the current target
	if (m_debugActive && m_debugTarget)
	{
		m_debugTarget->RenderDebug();
	}
}

