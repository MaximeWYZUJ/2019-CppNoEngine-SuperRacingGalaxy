#include "pch.h"

#include <fstream>
#include <memory>
#include "BitmapReader.h"
#include "Size.h"

namespace SmallEngine
{
	using namespace std;

	Bitmap BitmapReader::Read(std::string const& filePath)
	{
		ifstream ifs(filePath, std::ifstream::binary);

		if (!ifs)
		{
			// error
			return {};
		}

		ifs.seekg(0, ifs.end);
		int const length = ifs.tellg();
		ifs.seekg(0, ifs.beg);

		string param;
		if (!(ifs >> param))
		{
			return Bitmap();
		}

		if (param != "P6")
		{
			return Bitmap();
		}

		int width;
		if (!(ifs >> width))
		{
			return Bitmap();
		}

		int height;
		if (!(ifs >> height))
		{
			return Bitmap();
		}

		int maxVal;
		if (!(ifs >> maxVal))
		{
			return Bitmap();
		}

		int64_t const offset = ifs.tellg();

		ifs.seekg(offset + 1LL);

		int64_t const dataLength = length - offset - 1LL;
		vector<uint8_t> v;
		v.resize(dataLength);

		uint8_t* buf = v.data();
		ifs.read(reinterpret_cast<char*>(buf), dataLength);
		ifs.close();

		vector<uint8_t> res;
		res.reserve(dataLength / 3);

		for (int i = 0; i < dataLength; i += 3)
		{
			res.push_back(v[i]);
		}

		return Bitmap(Size<int>{ width, height }, move(res));
	}
}
