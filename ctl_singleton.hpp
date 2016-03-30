/**
 * Conceptual Template Library by Piotr Grudzien
 * Automatic Singleton Adapters
 */

#ifndef CTL_AUTOSINGLETON
#define CTL_AUTOSINGLETON

namespace CTL
{
	template<typename T>
	class AutoSingleton
	{
	private:
		T* instance;
		
		static T* getInstance()
		{
			static T singleton;
			return &singleton;
		}

	public:
		AutoSingleton()
		{
			this->instance = AutoSingleton<T>::getInstance();
		}

		~AutoSingleton()
		{
		}

		T* operator->()
		{
			return this->instance;
		}

		T& operator*()
		{
			return *this->instance;
		}

	}; // !AutoSingleton

	template<typename T>
	class ArcSingleton
	{
	private:
		T* instance;

		static T* Manage(bool free)
		{
			static T* singleton = nullptr;;
			static size_t count = 0;
			if(free)
			{
				--count;
				if (count == 0)
				{
					delete singleton;
				}
			}
			else
			{
				if (count == 0)
				{
					singleton = new T();
				}
				++count;
			}
			return singleton;
		}

		static T* getInstance()
		{
			return ArcSingleton<T>::Manage(false);
		}

		static void freeInstance()
		{
			ArcSingleton<T>::Manage(true);
		}

	public:
		ArcSingleton()
		{
			this->instance = ArcSingleton<T>::getInstance();
		}

		~ArcSingleton()
		{
			ArcSingleton<T>::freeInstance();
		}

		T* operator->()
		{
			return this->instance;
		}

		T& operator*()
		{
			return *this->instance;
		}

	}; // !ArcSingleton

} // !CTL

#endif // !CTL_SINGLETON
 