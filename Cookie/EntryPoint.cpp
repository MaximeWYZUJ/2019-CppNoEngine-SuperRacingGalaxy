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
		return make_unique<Engine>(make_unique<DeviceD3D11>(), make_unique<InputManager>(), make_unique<SceneManager>());
#else
		throw exception("The standalone engine only supports windows platform with DirectX API");
#endif
	}
}
