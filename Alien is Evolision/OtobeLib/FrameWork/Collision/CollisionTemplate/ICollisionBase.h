#pragma once
#include <list>
#include <map>
#include <vector>
#include <functional>
namespace OtobeLib
{
	template<class T>
	class ICollisionBase//�C���^�[�t�F�[�X�N���X
	{
	public:
		//�o�[�`�����f�X�g���N�^(�h���N���X�̃f�X�g���N�^���ĂԂ���)
		virtual ~ICollisionBase(){}

		//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
		virtual void OnCollisionEnter(T* obj) = 0;

		//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
		virtual void NonCollisionEnter(T* obj) = 0;

		//------------�Z�b�^�[------------------------------------------//
		//�����������ǂ����̃t���O��ݒ肷��
		virtual void SetCheck(const bool& check) = 0;

		//���������I�u�W�F�N�g��ID�����X�g�ɒǉ�����
		virtual void Add(const int& id) = 0;

		//���������I�u�W�F�N�g�����X�g�ɒǉ�����
		virtual void Add(T* object) = 0;

		//�����蔻��̊֐���ݒ肷��
		virtual void AddFunc(const std::string& name, std::function<void(T*)> func) = 0;

		//������������ID��ݒ肷��
		virtual void ReMove(const int& id) = 0;

		//������������ID��S�ď���
		virtual void ALLMove() = 0;

		//�����蔻��̒��S��ݒ肷��
		virtual void SetCenter(const DirectX::SimpleMath::Vector3& center) = 0;

		//�����蔻��̔��a��ݒ肷��
		virtual void SetRadius(const DirectX::SimpleMath::Vector3& radius) = 0;

		//---------------------------------------------------------------//

		//------------�Q�b�^�[-------------------------------------------//
		//�����������ǂ����̃t���O���擾����
		virtual const bool& GetCheck()const = 0;

		//���g�̓����蔻����擾����
		virtual ICollisionBase* GetThisCollision() = 0;

		//���X�g�̒�����w�肵��ID���擾����i�������A�擾�ł��Ȃ������Ƃ��́|1���Ԃ�j
		virtual const int Find(const int& id) = 0;

		//�ݒ肵���֐����ĂԂ��ǂ���
		virtual const bool RequestFunc(const std::string& name) = 0;

		//ID���擾����
		virtual const int& GetID()const = 0;

		//�����蔻��̒��S�_���擾����
		virtual const DirectX::SimpleMath::Vector3& GetCenter()const = 0;

		//�����蔻��̔��a���擾����
		virtual const DirectX::SimpleMath::Vector3& GetRadius()const = 0;

		//���������I�u�W�F�N�g�̃��X�g���擾����
		virtual std::vector<T*>& GetObjectList() = 0;

		//-----------------------------------------------------------------//
	};
}

