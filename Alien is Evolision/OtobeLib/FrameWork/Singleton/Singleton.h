#pragma once
namespace OtobeLib
{
	template <class T>
	class Singleton
	{
		friend T;//インスタンスを生成する際に、フレンドクラスに無理やりアクセスしている（基本的にメンバにアクセスはしない）
	private:
		static T* m_Singleton_class;//シングルトンクラスのテンプレート
	private:
		Singleton::Singleton()
		{
		}
	public:
		virtual Singleton::~Singleton()
		{
		}

		static T* Singleton::GetInstance()
		{
			if (m_Singleton_class == nullptr)
			{
				m_Singleton_class = new T();//Instanceがnewされていなければ、newし、そうでなければ、Instanceのポインタを返す
			}
			return m_Singleton_class;
		}

		static void Singleton::Release()
		{
			if (m_Singleton_class != nullptr)
			{
				delete m_Singleton_class;
				m_Singleton_class = nullptr;
			}
		}
	};
	template<class T>
	T* Singleton<T>::m_Singleton_class = nullptr;
}
