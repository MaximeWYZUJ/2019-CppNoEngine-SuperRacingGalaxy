#include "pch.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include "EntryPoint.h"
#include "BitmapToMeshAdapter.h"
#include "SceneManager.h"
#include <DeviceD3D11.h>
#include <variant>
#include "Material.h"
#include "InputManager.h"

// Todo: Change this for a standard console main
int APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	using namespace std;
	using namespace Cookie;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// scale, rotate, translate

	/*ifstream f("heightmap128.obj");
	if (!f.good())
	{
		Bitmap bitmap = BitmapReader::Read("heightmap128.ppm");
		Mesh loadedMesh = BitmapToMeshAdapter::Convert(bitmap);
		string objAsStr = MeshToObjConverter::Convert(loadedMesh);
		ofstream ofs("heightmap128.obj");
		ofs << objAsStr;
		ofs.close();
	}*/

	try
	{
		unique_ptr<Engine> engine = EntryPoint::CreateStandaloneEngine();
		engine->Initialisations();

		Device* device = engine->GetDevice();
		SceneManager* smgr = engine->GetSceneManager();

		Mesh* mesh = smgr->GetMesh("cube.obj");

		SceneNode* root = smgr->GetRoot();
		SceneNode* firstNode = smgr->AddSceneNode(root);
		firstNode->localTransform.SetPosition({ 0.0f, 0.0f, 2.0f });
		auto const mat = new Material(device);
		smgr->AddMeshRenderer(mesh, mat, firstNode);

		SceneNode* secondNode = smgr->AddSceneNode(firstNode);
		secondNode->localTransform.SetPosition({ 0.0f, 0.0f, 5.0f });
		smgr->AddMeshRenderer(mesh, mat, secondNode);

		// We should set the camera here
		// Bind Input Actions for first "scene" (main menu)
		// Bind lambda on Update Hook for game logic

		float x = 0.0f;
		while (engine->Run())
		{
			x += 0.01f;
			firstNode->localTransform.SetRotation(Quaternion<>::FromDirection(x, Vector3<>(0.0f, 1.0f, 0.0f)));
			secondNode->localTransform.SetRotation(Quaternion<>::FromDirection(x * 2.0f, Vector3<>(0.0f, 1.0f, 0.0f)));
			engine->Update();
		}

		return (int)1;
	}

	catch (const std::exception & E)
	{
		const int BufferSize = 128;
		wchar_t message[BufferSize];

		size_t numCharacterConverted;
		mbstowcs_s(&numCharacterConverted, message, E.what(), BufferSize - 1);
		::MessageBox(nullptr, message, L"Erreur", MB_ICONWARNING);

		return (int)99;
	}

	catch (int codeErreur)
	{
		wchar_t szErrMsg[MAX_LOADSTRING];	// Un message d'erreur selon le code

		::LoadString(hInstance, codeErreur, szErrMsg, MAX_LOADSTRING);
		::MessageBox(nullptr, szErrMsg, L"Erreur", MB_ICONWARNING);

		return (int)99; // POURQUOI 99???
	}
}