#include "Cargo.h"

using namespace std;
using namespace Cookie;

Cargo::Cargo(Transform<> transform, float gravityValue, bool isUpVectorDynamic, string meshPath, std::wstring texture1Path)
	: gravityCenter(transform.GetPosition()), gravityValue(gravityValue), isUpVectorDynamic(isUpVectorDynamic)
{
	initialTransform = transform;
	type_ = Type::CARGO;
	meshPath_ = meshPath;
	texturePath_ = texture1Path;
	mesh = nullptr;
	texture = nullptr;
}

void Cargo::anime()
{
	cout << "Ca tourne !" << endl;
}
