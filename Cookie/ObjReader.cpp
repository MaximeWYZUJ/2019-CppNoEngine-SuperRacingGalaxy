#include "pch.h"

#include <fstream>
#include <sstream>
#include <charconv>
#include <iterator>
#include "ObjReader.h"

namespace Cookie
{
	using namespace std;

	ObjData ObjReader::Read(std::string const& filePath)
	{
		ifstream ifs(filePath, ios::binary);

		if (!ifs)
		{
			// error
			return {};
		}

		ObjData res;

		string lineType;
		string v1;
		string v2;
		string v3;
		string v4;

		unique_ptr<char[]> buf = make_unique<char[]>(1024);
		string_view bufSv{ buf.get(), 1024 };

		vector<string_view> segments;
		segments.reserve(8);

		vector<string_view> subSegments;
		subSegments.reserve(3);

		while (ifs.getline(buf.get(), 1024))
		{
			bufSv = string_view{ buf.get(), static_cast<size_t>(ifs.gcount() - 1) };
			Split(bufSv, ' ', segments);

			if (segments.size() == 0)
			{
				continue;
			}

			if (segments[0] == "v")
			{
				if (segments.size() < 4 || segments.size() > 5)
				{
					return {};
				}

				Vector4<>& point = res.vertices.emplace_back(Vector4<>{});
				point.w = 1.0f;

				for (int i = 1; i < segments.size(); ++i)
				{
					auto val = reinterpret_cast<float*>(&point) + i - 1;
					string_view const& sv = segments[i];
					if (auto [p, ec] = from_chars(sv.data(), sv.data() + sv.length(), *val);
						ec == std::errc::invalid_argument)
					{
						return {};
					}
				}
			}
			else if (segments[0] == "vt")
			{
				// Not supported
			}
			else if (segments[0] == "vn")
			{
				if (segments.size() != 4)
				{
					return {};
				}

				Vector3<>& normal = res.normals.emplace_back(Vector3<>{});

				for (int i = 1; i < segments.size(); ++i)
				{
					auto val = reinterpret_cast<float*>(&normal) + i - 1;
					string_view const& sv = segments[i];
					if (auto [p, ec] = from_chars(sv.data(), sv.data() + sv.length(), *val);
						ec == std::errc::invalid_argument)
					{
						return {};
					}
				}
			}
			else if (segments[0] == "vp")
			{
				// Not supported
			}
			else if (segments[0] == "f")
			{
				if (segments.size() > 4)
				{
					// Does not support complex faces, only triangles
					return {};
				}

				ObjTriangle& t = res.triangles.emplace_back(ObjTriangle{});
				int offset = 0;
				for (auto it = next(begin(segments)); it != end(segments); ++it)
				{
					Split(*it, '/', subSegments);

					auto val = (&t.vertexIndices.x) + offset;
					if (subSegments.size() == 0) return {};
					if (auto [p, ec] = from_chars(subSegments[0].data(), subSegments[0].data() + subSegments[0].length(), *val);
						ec != std::errc::invalid_argument)
					{
						--*val;
					}

					if (subSegments.size() > 1)
					{
						auto val = (&t.textureIndices.x) + offset;
						if (auto [p, ec] = from_chars(subSegments[1].data(), subSegments[1].data() + subSegments[1].length(), *val);
							ec != std::errc::invalid_argument)
						{
							--*val;
						}
					}

					if (subSegments.size() > 2)
					{
						auto val = (&t.normalIndices.x) + offset;
						if (auto [p, ec] = from_chars(subSegments[2].data(), subSegments[2].data() + subSegments[2].length(), *val);
							ec != std::errc::invalid_argument)
						{
							--*val;
						}
					}

					++offset;
				}
			}
		}

		if (!ifs.eof())
		{
			return {};
		}

		return res;
	}
}
