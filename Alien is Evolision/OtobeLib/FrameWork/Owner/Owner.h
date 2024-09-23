#pragma once
namespace OtobeLib
{
	template<class T>
	class Owner
	{
	private:
		T* m_Owner;
	public:
		//コンストラクタ
		Owner():m_Owner(nullptr){}

		//デストラクタ
		~Owner(){}

		virtual void SetOwner(T* ownew) { m_Owner = ownew; }
		virtual T* GetOwner() { return m_Owner; }
	};
}

