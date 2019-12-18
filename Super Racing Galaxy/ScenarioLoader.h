#pragma once

namespace Cookie {
	class SceneNode;
	class Device;
	class TextureManager;
	class MaterialManager;
	class SceneManager;
	class Engine;
}

class Scenario;
class Planet;
class Scenery;
class Vehicle;
class Teleport;
class Goal;
class Prefab;
class Skybox;

class ScenarioLoader
{
public:
	static void LoadScenario(Cookie::Engine* engine, Scenario const& scenario);

private:
	static void CreateObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Cookie::TextureManager* textureManager, Cookie::Device* device, Cookie::SceneNode* root, Prefab* obj);
	static void InitPlanetObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Planet* obj);
	static void InitVehicleObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Vehicle* obj);
	static void InitSceneryObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Scenery* obj);	
	static void InitSkyboxObject(Cookie::SceneManager* smgr, Cookie::MaterialManager *materialManager, Skybox* obj);
	static void InitTeleportObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Teleport* obj);
	static void InitGoalObject(Cookie::SceneManager* smgr, Cookie::MaterialManager* materialManager, Goal* obj);
};
