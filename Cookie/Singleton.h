#pragma once

namespace SmallEngine
{
	template <class T> class Singleton
	{
	public:
        static T& GetInstance()
        {
			static T instance;
            return instance;
        }

	protected :
		Singleton(void){}
		~Singleton(void){}

	private :
		Singleton(Singleton&);
        void operator =(Singleton&) = delete;
	};
}