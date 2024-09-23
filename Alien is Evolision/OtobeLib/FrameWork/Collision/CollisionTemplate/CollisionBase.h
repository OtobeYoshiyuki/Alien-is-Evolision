#pragma once
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/ICollisionBase.h"
namespace OtobeLib
{
	template<class T>
	class CollisionBase:public ICollisionBase<T>
	{
	private:
		bool mCheck;			//�����������ǂ����̔���

		int	 mID;				//�ʂ�ID

		static int sID;			//��������ID

		std::list<int>  mOtherID;//�������������ID�̃��X�g

		std::vector<T*>	m_OtherObject;//�����������̃I�u�W�F�N�g

		//�����蔻��̒��S�_
		DirectX::SimpleMath::Vector3 m_center;

		//�����蔻��̔��a
		DirectX::SimpleMath::Vector3 m_radius;

		//�Ăԓ����蔻��̊֐���ݒ肷��
		std::map<std::string, std::function<void(T*)>>m_callerFunc;

	public:
		//�R���X�g���N�^
		CollisionBase():mCheck(false)
		{
			mID = sID;
			sID++;
		}

		//�f�X�g���N�^
		virtual ~CollisionBase()override {};

		//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
		virtual void OnCollisionEnter(T* obj)override
		{
			//�����蔻��̃t���O�𗧂Ă�
			this->SetCheck(true);
		};

		//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
		virtual void NonCollisionEnter(T* obj)override
		{
			////�����蔻��̃t���O�𗧂Ă�
			//this->SetCheck(false);
		};

		//------------�Z�b�^�[------------------------------------------//
		//�����������ǂ����̃t���O��ݒ肷��
		void SetCheck(const bool& check)override { mCheck = check; };

		//���������I�u�W�F�N�g��ID�����X�g�ɒǉ�����
		void Add(const int& id)override { mOtherID.push_back(id); };

		//���������I�u�W�F�N�g��ǉ�����
		void Add(T* obj)override { m_OtherObject.push_back(obj); }

		//�����蔻��̒��S��ݒ肷��
		virtual void SetCenter(const DirectX::SimpleMath::Vector3& center)override { m_center = center; };

		//�����蔻��̔��a��ݒ肷��
		virtual void SetRadius(const DirectX::SimpleMath::Vector3& radius)override { m_radius = radius; };

		//�����蔻��̊֐���ݒ肷��
		void AddFunc(const std::string& name, std::function<void(T*)> func)override
		{
			m_callerFunc[name] = func;
		}


		//������������ID��ݒ肷��
		void ReMove(const int& id)override 
		{
			for (int& otherid : mOtherID)
			{
				if (otherid == id)
				{
					mOtherID.remove(id);
					return;
				}
			}
		};

		//������������ID��S�ď���
		void ALLMove()override { mOtherID.clear(); };
		//-----------------------------------------------------------------//

		//------------�Q�b�^�[-------------------------------------------//
		//�����������ǂ����̃t���O���擾����
		virtual const bool& GetCheck()const override { return mCheck; };

		//ID���擾����
		const int& GetID()const override { return mID; };

		//���g�̓����蔻����擾����
		virtual ICollisionBase* GetThisCollision()override { return this; };

		//���X�g�̒�����w�肵��ID���擾����i�������A�擾�ł��Ȃ������Ƃ��́|1���Ԃ�j
		const int Find(const int& id)override 
		{
			for (int& otherid : mOtherID)
			{
				if (otherid == id)return otherid;
			}
			return -1;
		};

		//�ݒ肵���֐����ĂԂ��ǂ���
		const bool RequestFunc(const std::string& name)override
		{
			//�C�e���[�^��錾����
			std::map<std::string, std::function<void(T*)>>::const_iterator iter;

			//�w�肵���֐���T��
			iter = m_callerFunc.find(name);

			//�w�肵���֐���������Ȃ�����(iter��end)
			if (iter == m_callerFunc.end())return false;

			return true;
		}

		//�����蔻��̒��S��ݒ肷��
		virtual const DirectX::SimpleMath::Vector3& GetCenter()const override { return m_center; }

		//�����蔻��̔��a��ݒ肷��
		virtual const DirectX::SimpleMath::Vector3& GetRadius()const override { return m_radius; }

		std::vector<T*>& GetObjectList()override { return m_OtherObject; }

		//-----------------------------------------------------------------------//

		//ID�𐶐�����
		static int CreateID() { return sID; };
	};

	template <class T>
	int CollisionBase<T>::sID = NULL;
}

