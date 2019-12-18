#include "Teleport.h"
#include <assert.h>
#include <algorithm>

#include "SceneNode.h"
#include "PhysicsComponent.h"
#include "Landing.h"

using namespace std;
using namespace chrono;

Teleport::Teleport(Cookie::Transform<> transform, std::string meshPath, std::wstring texturePath, std::string triggerMeshpath, double timeTravel, float cooldown)
	: cooldown{ cooldown }, timeTravel{ timeTravel }
{
	initialTransform = transform;
	type_ = Type::TELEPORT;
	meshPath_ = meshPath;
	texturePath_ = texturePath;
	triggerPath_ = triggerMeshpath;
	mesh = nullptr;
	texture = nullptr;

	controlPoints = { transform.GetPosition() };
}

void Teleport::linkTo(Landing* landing, vector<Cookie::Vector3<>> contP)
{
#ifdef _DEBUG
	assert(landing);
#endif // DEBUG

	controlPoints = { root->localTransform.GetPosition() };
	for_each(contP.begin(), contP.end(), [&](const Cookie::Vector3<>& t) {
		controlPoints.push_back(t);
	});
	controlPoints.push_back(landing->root->localTransform.GetPosition());
}

void Teleport::resetCooldown()
{
	lastUse = system_clock::now();
}

bool Teleport::mayUse() const
{
	auto now = system_clock::now();
	auto duration = duration_cast<seconds>(now - lastUse).count();
	return duration > cooldown;
}

void Teleport::run()
{
	if (isActive) {
		realTimeTravel += 1.0 / 60.0; // 60 fps
		if (abs(realTimeTravel - timeTravel) < 1.0 / 60.0) {
			// Stop animation
			isActive = false;
			root->physics->resetAcceleration = true;
			objToTeleport = nullptr;
			realTimeTravel = 0.0;
		}
		else {
			// Keep on animating
			objToTeleport->root->localTransform.SetPosition(animateTeleport(realTimeTravel / timeTravel));
			objToTeleport->root->physics->isDirty = true;
		}
	}
}

void Teleport::printControlPoints(ostream& out)
{
	out << "Control points : " << endl;
	std::for_each(controlPoints.begin(), controlPoints.end(), [&out](const Cookie::Vector3<>& p) {
		out << p.x << " ; " << p.y << " ; " << p.z << endl;
	});
}

Cookie::Vector3<> hermite(Cookie::Vector3<> P0, Cookie::Vector3<> m0, Cookie::Vector3<> P1, Cookie::Vector3<> m1, double t) {
	double h00 = 2 * pow(t, 3) - 3 * pow(t, 2) + 1;
	double h10 = pow(t, 3) - 2 * pow(t, 2) + t;
	double h01 = -2 * pow(t, 3) + 3 * pow(t, 2);
	double h11 = pow(t, 3) - pow(t, 2);

	return h00 * P0 + h10 * m0 + h01 * P1 + h11 * m1;
}

Cookie::Vector3<> Teleport::animateTeleport(double t)
{
	int N = controlPoints.size() - 1; // indice de fin [P0, PN]
	int index = floor(t / (1.0/N));
	if (index == N)
		return objToTeleport->root->localTransform.GetPosition();

	if (index == 0 || index == N - 1) {
		// Interpolation linéaire
		double ratio = N * t - index;
		return controlPoints[index] * (1 - ratio) + controlPoints[index + 1] * ratio;
	}
	else {
		// Interpolation par hermite
		auto P0 = controlPoints[index];
		auto P1 = controlPoints[index + 1];
		auto m0 = index == 0 ? P1 - P0 : 0.5*(P1 - controlPoints[index - 1]);
		auto m1 = index == N-1 ? P1 - P0 : 0.5*(controlPoints[index + 2] - P0);
	
		return hermite(P0, m0, P1, m1, N * t - index);
	}
}
