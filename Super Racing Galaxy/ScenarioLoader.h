#pragma once

namespace Cookie {
	class Engine;
}

class Scenario;
class Planet;
class Scenery;
class Vehicle;
class Prefab;

class ScenarioLoader
{
public:
	static void LoadScenario(Cookie::Engine* engine, Scenario const& scenario);

private:
	static void CreateObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Cookie::TextureManager* textureManager, Cookie::Device* device, Cookie::SceneNode* root, Prefab* obj);
	static void InitPlanetObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Planet* obj);
	static void InitVehicleObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Vehicle* obj);
	static void InitSceneryObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Scenery* obj);
};
