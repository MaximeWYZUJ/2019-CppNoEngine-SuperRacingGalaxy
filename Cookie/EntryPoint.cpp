#include "pch.h"
#include <memory>
#include "EntryPoint.h"
#include "DeviceD3D11.h"
#include "ActionManager.h"

namespace Cookie
{
	using namespace std;

	unique_ptr<Engine> EntryPoint::CreateStandaloneEngine()
	{
#ifdef WIN32
		// Let's just create a default instance with DirectX for windows
		auto device = make_unique<DeviceD3D11>();
		auto inputManager = make_unique<InputManager>(device.get());
		auto actionManager = make_unique<ActionManager>(inputManager.get());
		auto physics = unique_ptr<PhysicsEngine>(&PhysicsEngine::getInstance());
		auto sceneManager = make_unique<SceneManager>(device.get());
		auto textureManager = make_unique<TextureManager>();
		auto materialManager = make_unique<MaterialManager>();
		auto synchronizer = make_unique<Synchronizer>();
		
		return make_unique<Engine>(
			move(device),
			move(inputManager),
			move(actionManager),
			move(physics),
			move(sceneManager),
			move(textureManager),
			move(materialManager),
			move(synchronizer));
#else
		throw exception("The standalone engine only supports windows platform with DirectX API");
#endif
	}
}
