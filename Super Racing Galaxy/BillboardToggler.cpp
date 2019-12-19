#include "BillboardToggler.h"
#include "SceneNode.h"
#include "MeshRenderer.h"
#include "Scenery.h"
#include "Teleport.h"
#include "Goal.h"

namespace Cookie
{
	using namespace std;
	
	BillboardToggler::BillboardToggler(std::vector<Planet*> planets)
		: planets(planets)
	{
	}

	void BillboardToggler::Update(Vector3<> const& camPos, float minDistance)
	{
		for (Planet* p : planets)
		{
			float d = Vector3<>::Distance(camPos, p->root->localTransform.GetPosition());
			
			if (d >= 500.0f)
			{
				PlanetAsBillboard(p);
			}
			else
			{
				PlanetAsFullRendering(p);
			}
		}
	}
	
	void BillboardToggler::PlanetAsBillboard(Planet* p)
	{
		p->billboard->meshRenderer->Enable();
		p->root->meshRenderer->Disable();
		for (Scenery* s : p->setElements)
		{
			s->root->meshRenderer->Disable();
		}
		for (Teleport* t : p->teleportElements)
		{
			t->root->meshRenderer->Disable();
		}
		if (p->goal)
		{
			p->goal->root->meshRenderer->Disable();
		}
	}
	
	void BillboardToggler::PlanetAsFullRendering(Planet* p)
	{
		p->billboard->meshRenderer->Disable();
		p->root->meshRenderer->Enable();
		for (Scenery* s : p->setElements)
		{
			s->root->meshRenderer->Enable();
		}
		for (Teleport* t : p->teleportElements)
		{
			t->root->meshRenderer->Enable();
		}
		if (p->goal)
		{
			p->goal->root->meshRenderer->Enable();
		}
	}
}
