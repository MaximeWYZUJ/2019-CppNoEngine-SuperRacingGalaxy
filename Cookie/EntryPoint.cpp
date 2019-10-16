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
		return make_unique<Engine>(unique_ptr<DeviceD3D11>(new DeviceD3D11()), unique_ptr<SceneManager>(new SceneManager()));
#else
		throw exception("The standalone engine only supports windows platform with DirectX API");
#endif
	}
}
