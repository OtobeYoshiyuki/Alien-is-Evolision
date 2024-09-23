#pragma once
namespace OtobeLib
{
	template<class T>
	class Owner
	{
	private:
		T* m_Owner;
	public:
		//�R���X�g���N�^
		Owner():m_Owner(nullptr){}

		//�f�X�g���N�^
		~Owner(){}

		virtual void SetOwner(T* ownew) { m_Owner = ownew; }
		virtual T* GetOwner() { return m_Owner; }
	};
}

