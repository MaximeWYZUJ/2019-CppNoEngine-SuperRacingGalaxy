#include "pch.h"

#include <fstream>
#include "EntryPoint.h"
#include "BitmapToMeshAdapter.h"
#include "BitmapReader.h"
#include "MeshToObjConverter.h"
#include "SceneManager.h"
#include <MeshRenderer.h>
#include <DeviceD3D11.h>
#include <variant>
#include "Material.h"

// Todo: Change this for a standard console main
int APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	using namespace SmallEngine;
	using namespace std;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

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
		MeshRenderer* renderer = new MeshRenderer(*mesh, Material(device), device);

		SceneNode* root = smgr->GetRoot();
		SceneNode* map = smgr->CreateSceneNode();
		map->AddComponent(renderer);
		root->AddChild(map);

		// We should set the camera here

		while (engine->Run())
		{
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