#pragma once

namespace Cookie {
	class Engine;
}

class Scenario;
class Prefab;
class Planet;
class Scenery;
class Vehicle;

class ScenarioLoader
{
public:
	static void LoadScenario(Cookie::Engine* engine, Scenario const& scenario);

private:
	static void CreateObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Cookie::TextureManager* textureManager, Cookie::Device* device, Cookie::SceneNode* root, Prefab* obj);
	static void InitPlanetObject(Cookie::SceneManager* smgr, Planet* obj);
	static void InitVehicleObject(Cookie::SceneManager* smgr, Vehicle* obj);
	static void InitSceneryObject(Cookie::SceneManager* smgr, Scenery* obj);
};
