#ifndef
#define __SINGLETON__H

template <typename T>

class Singleton
{
public:
	static std::shared_ptr<T>& Instance();		
	
private:
	static T*  ptr;
	Singleton() = delete;
	Singleton(const Singleton &) = delete;
	~Singleton()= default;	
}


template <typename T>
static T& Singleton::Instance()
{
	if(ptr == nullptr)
	{	
		ptr = new T;
	}
	return *ptr;
}

template<typename T>
T* Singleton<T>::ptr = nullptr;

#endif
