#pragma once

#include <algorithm>
#include <vector>

namespace Cookie
{
	template<class T>
	class ObjectPool
	{
	public:
		using Type = typename std::remove_cv<T>::type;

		ObjectPool(size_t capacity);

		Type& Rent();
		void Return(Type* obj);

	private:
		Type* data;
		std::vector<int> freeIndices;
	};

	template<class T>
	ObjectPool<T>::ObjectPool(size_t capacity)
	{
		data = new Type[capacity];
		freeIndices.resize(capacity);

		std::generate(
			begin(freeIndices),
			end(freeIndices),
			[n = capacity]() mutable { return --n; });
	}

	template<class T>
	auto ObjectPool<T>::Rent() -> Type&
	{
		assert(freeIndices.size() > 0);

		int index = freeIndices.back();
		freeIndices.pop_back();

		Type* d = new (&data[index]) Type;

		return *d;
	}

	template<class T>
	void ObjectPool<T>::Return(Type* obj)
	{
		assert(
			&obj > &data &&
			&obj < &(data + freeIndices.size()));

		int const index = data + (obj - data);

		assert(std::find(begin(freeIndices), end(freeIndices), index) == end(freeIndices));

		delete obj;
		freeIndices.push_back(index);
	}
}
