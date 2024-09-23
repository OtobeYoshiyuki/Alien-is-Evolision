#pragma once
#include "OtobeLib/FrameWork/Collision/Collision.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/ICollisionBase.h"
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
namespace OtobeLib
{
	class Collision;
	class CollisionManager
	{
	private:
		//�A��������ID��ݒ肷��
		std::multimap<std::string, std::string>		m_IDGroup;

		//��������̂�ݒ肷��
		std::map<std::string, std::vector<std::string>>		m_CollisionGrop;

		//�v���C���[���̓����蔻��
		std::vector<ITextureBase*>					m_HeroCollision;	

		//�e�N�X�`���̃��X�g
		std::vector<ITextureBase*>					m_TextureList;		

		//�����蔻��̃��X�g
		std::map<std::string, std::vector<ITextureBase*>> m_CollisionList;

		//�v���C���[���̎g�������蔻��
		std::vector<ITextureBase*>					m_UseHero;

		//���̎g�������蔻��
		std::map<std::string, std::vector<ITextureBase*>> m_UseOther;

		//�Ăяo���֐��I�u�W�F�N�g
		std::function<bool(ITextureBase*, ITextureBase*)>	m_Collision;

	public:
		//�R���X�g���N�^
		CollisionManager();	

		//�f�X�g���N�^
		~CollisionManager() {}	

		//������
		void Initialize();

		//�X�V����
		void Update();			

		//�e�N�X�`�����X�g�ɒǉ�
		void Add(ITextureBase* obj) { m_TextureList.push_back(obj);}	

		//�g�����̂�ǉ�����
		void AddUseHero(ITextureBase* obj)
		{ 
			if (obj->GetState() == ITextureBase::STATE::DRAW)
			{
				m_UseHero.push_back(obj);
			}
		}

		void AddUseOther(ITextureBase* obj, const std::string& heroName) 
		{
			if (obj->GetState() == ITextureBase::STATE::DRAW)
			{
				m_UseOther[heroName].push_back(obj);
			}
		}
		void AddUseOther(ITextureBase* other, ITextureBase* hero, const float& length);

		//ID�̔���������̂�ǉ�����
		void SetIDGroup(const std::string& A, const std::string& B)			
		{	
			m_IDGroup.insert(std::pair<std::string, std::string>(A, B));
			m_IDGroup.insert(std::pair<std::string, std::string>(B, A));
		}

		//��������̂�ݒ肷��
		void SetCollisionGrop(const std::string& Hero, const std::string& Other)
		{
			m_CollisionGrop[Hero].push_back(Other);
		}

		//�����蔻��̎�������Z�b�g����
		void ResetUseHero() { m_UseHero.clear(); }

	private:

		//2D�I�u�W�F�N�g�̓����蔻��̍X�V
		void Object2D();												
	};
}

