#pragma once

namespace Cookie
{
	class Horloge
	{
	public:
		Horloge();

		int64_t GetTimeCount() const;
		double GetSecPerCount() const { return m_SecPerCount; }
		double GetTimeBetweenCounts(int64_t start, int64_t stop) const;
	private:
		double m_SecPerCount;
	};
}