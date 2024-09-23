#pragma once
namespace OtobeLib
{
	template <class T>
	class Singleton
	{
		friend T;//�C���X�^���X�𐶐�����ۂɁA�t�����h�N���X�ɖ������A�N�Z�X���Ă���i��{�I�Ƀ����o�ɃA�N�Z�X�͂��Ȃ��j
	private:
		static T* m_Singleton_class;//�V���O���g���N���X�̃e���v���[�g
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
				m_Singleton_class = new T();//Instance��new����Ă��Ȃ���΁Anew���A�����łȂ���΁AInstance�̃|�C���^��Ԃ�
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
