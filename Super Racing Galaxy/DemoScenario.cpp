#include "pch.h"
#include "DemoScenario.h"
#include <corecrt_math_defines.h>

using namespace Cookie;

Scenario ScenarioCreator::CreateDemoScenario()
{
	Scenario scenario;

	scenario.objects.push_back(Transform<>(
		{ 0.0f, 0.0f, 10.0f },
		{ 5.0f, 0.01f, 15.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }));

	scenario.objects.push_back(Transform<>(
		{ 0.0f, 0.0f, 40.0f },
		{ 5.0f, 0.01f, 15.0f },
		Quaternion<>::FromDirection(M_PI / 12, { 0.0f, 0.0f, 1.0f })));

	return scenario;
}
