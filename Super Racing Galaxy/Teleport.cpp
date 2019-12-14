#include "Teleport.h"
#include <assert.h>

using namespace std::chrono;

Teleport::Teleport(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, float cooldown, std::vector<Cookie::Transform<>> controlPoints_)
	: cooldown{ cooldown }
{
	initialTransform = transform;
	type_ = Type::TELEPORT;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	mesh = nullptr;
	texture = nullptr;

	controlPoints = controlPoints_;
	linkedTeleport = nullptr;
}

void Teleport::linkTo(Teleport* teleport)
{
#ifdef _DEBUG
	assert(teleport);
#endif // DEBUG

	linkedTeleport = teleport;
}

void Teleport::useNow()
{
	lastUse = system_clock::now();
}

bool Teleport::mayUse() const
{
	auto now = system_clock::now();
	auto duration = duration_cast<seconds>(now - lastUse).count();
	return duration > cooldown;
}
