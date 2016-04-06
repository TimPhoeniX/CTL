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
		using AS = ArcSingleton<T>;

		T* instance;

		static size_t Count(size_t c)
		{
			static size_t count = 0;
			count += c;
			return count;
		}

		static T* Instance()
		{
			static T* instance = nullptr;
			if(Count(0) == 0) instance = new T();
			return instance;
		}

		void GetInstance()
		{
			this->instance = Instance();
			Count(1);
		}

		void FreeInstance()
		{
			size_t uses = Count(-1);
			if(uses == 0) delete this->instance;
		}

	public:
		ArcSingleton()
		{
			this->GetInstance();
		}

		~ArcSingleton()
		{
			this->FreeInstance();
		}

		size_t GetCount() const
		{
			return Count(0);
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
 