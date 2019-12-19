#pragma once
#include "Planet.h"

namespace Cookie
{
	class BillboardToggler
	{
	public:
		BillboardToggler(std::vector<Planet*> planets);
		void Update(Vector3<> const& camPos, float minDistance);

	private:
		static void PlanetAsBillboard(Planet* p);
		static void PlanetAsFullRendering(Planet* p);
		
		std::vector<Planet*> planets;
	};

}
