#include<memory>
#include<mutex>


namespace dxrt {
template<typename T>
class SingleTon
{
public:
	static T& GetInstance()
	{
		std::call_once(_flag, Create);
		return *_obj;
	}

private:
	static void Create()
	{
		_obj = std::unique_ptr<T>(new T);
	}
	static std::once_flag _flag;
	static std::unique_ptr<T> _obj;
}

}  // namespace dxrt
