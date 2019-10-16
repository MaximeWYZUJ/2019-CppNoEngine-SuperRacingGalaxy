#pragma once

namespace Cookie
{
	class Objet3D
	{
	public:
		virtual ~Objet3D() = default;
		virtual void Anime(float tempsEcoule) {};
		virtual void Draw() = 0;
	};
}