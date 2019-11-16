#include "pch.h"
#include "DemoScenario.h"
#include <corecrt_math_defines.h>

using namespace Cookie;

Scenario ScenarioCreator::CreateDemoScenario()
{
	Scenario scenario;

	scenario.objects.push_back(Transform<>(
		{ 0.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }));

	scenario.objects.push_back(Transform<>(
		{ 3000.0f, 0.0f, 3000.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }));

	scenario.objects.push_back(Transform<>(
		{ 2000.0f, 1320.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }));

	scenario.objects.push_back(Transform<>(
		{ 1200.0f, 1200.0f, 0.0f },
		{ 0.1f, 0.1f, 0.1f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }));

	scenario.objects.push_back(Transform<>(
		{ 2000.0f, 0.0f, 7000.0f },
		{ 5.0f, 5.0f, 5.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }));

	return scenario;
}
