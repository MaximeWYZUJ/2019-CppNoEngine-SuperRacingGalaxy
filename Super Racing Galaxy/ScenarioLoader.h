#pragma once

namespace Cookie {
	class Engine;
}

class Scenario;

class ScenarioLoader
{
public:
	static void LoadScenario(Cookie::Engine* engine, Scenario const& scenario);
};
