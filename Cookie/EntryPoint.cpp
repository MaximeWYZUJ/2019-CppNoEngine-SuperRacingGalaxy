#include "pch.h"
#include <memory>
#include "EntryPoint.h"
#include "DeviceD3D11.h"

namespace Cookie
{
	using namespace std;

	unique_ptr<Engine> EntryPoint::CreateStandaloneEngine()
	{
#ifdef WIN32
		// Let's just create a default instance with DirectX for windows
		auto device = make_unique<DeviceD3D11>();
		auto inputManager = make_unique<InputManager>(device.get());
		auto physics = unique_ptr<PhysicsEngine>(&PhysicsEngine::getInstance());
		auto sceneManager = make_unique<SceneManager>();
		auto textureManager = make_unique<TextureManager>();
		auto materialManager = make_unique<MaterialManager>();
		return make_unique<Engine>(move(device), move(inputManager), move(physics), move(sceneManager), move(textureManager), move(materialManager));
#else
		throw exception("The standalone engine only supports windows platform with DirectX API");
#endif
	}
}
