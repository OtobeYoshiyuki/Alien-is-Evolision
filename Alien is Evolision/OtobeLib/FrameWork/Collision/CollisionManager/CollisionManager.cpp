#include "pch.h"
#include "CollisionManager.h"
#include <map>
#include <utility>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CollisionManager::CollisionManager():m_Collision(nullptr)
{
}

void CollisionManager::Initialize()
{
	for (std::map<std::string,std::vector<std::string>>::const_iterator iter = m_CollisionGrop.begin();
		iter != m_CollisionGrop.end(); iter++)
	{
		//�����蔻��̎�����擾����
		const std::string& first = iter->first;

		//�����蔻��̎�l����ݒ肷��
		for (ITextureBase* tex : m_TextureList)
		{
			if (first == tex->GetName())
			{
				m_HeroCollision.push_back(tex);
			}
		}

		//��������̂�ݒ肷��
		std::vector<std::string> otherList = m_CollisionGrop.find(first)->second;

		for (const std::string& other:otherList)
		{
			//�����鑼�̂��̂�ݒ肷��
			for (ITextureBase* tex : m_TextureList)
			{
				if (other == tex->GetName())
				{
					m_CollisionList[first].push_back(tex);
				}
			}
		}
	}
}

void CollisionManager::Update()
{
	//2D�̓����蔻��̏���
	this->Object2D();
}

void CollisionManager::AddUseOther(ITextureBase* other, ITextureBase* hero, const float& length)
{
	//�n�_�I�_�̎擾
	RECT rect1 = hero->GetRect();
	RECT rect2 = other->GetRect();

	//�摜�̔��a�̎擾
	Vector3 Size1 = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f);
	Vector3 Size2 = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f);

	Size1 *= 0.5f;
	Size2 *= 0.5f;

	//���ꂼ��̍��W�����߂�i���W�͉摜�̒��S�œ��ꂷ��j
	Vector3 Pos1 = hero->GetPosition() + Size1 - hero->GetOrigin();
	Vector3 Pos2 = other->GetPosition() + Size2 - other->GetOrigin();

	//�������Z�o����
	Vector3 Length = Pos1 - Pos2;

	//��Βl�ɕϊ�����
	Length = Vector3(fabsf(Length.x), fabsf(Length.y), fabsf(Length.z));

	//�w�肵���������Z���ꍇ�́A�ǉ����Ȃ�
	if (Length.x < length && Length.y < length && Length.z < length)
	{
		if (other->GetState() == ITextureBase::STATE::DRAW &&
			hero->GetState() == ITextureBase::STATE::DRAW)
		{
			m_UseOther[hero->GetName()].push_back(other);
		}
	}
}


void CollisionManager::Object2D()
{
	for (ITextureBase* object1 : m_UseHero)
	{
		//�ݒ肵�Ă��铖����I�u�W�F�N�g�̃��X�g���擾����
		std::vector<ITextureBase*> objectList = m_UseOther[object1->GetName()];

		for (ITextureBase* object2 : objectList)
		{
			ICollisionBase<ITextureBase>* collision1 = object1->GetThisCollision();
			ICollisionBase<ITextureBase>* collision2 = object2->GetThisCollision();

			if (!collision1 || !collision2)continue;

			//�������Ă���I�u�W�F�N�g���������g����Ȃ��Ƃ�
			if (object1 != object2)
			{
				m_Collision = Collision::HitCheck_OBB;

				//�`�悳��Ă��Ȃ���΁A�����蔻������Ȃ�
				if (object1->GetState() != ITextureBase::STATE::DRAW)continue;
				if (object2->GetState() != ITextureBase::STATE::DRAW)continue;

				if (Collision::HitCheck_OBB(object1, object2))
				{
					//�����蔻��̎��s�֐�
					collision1->OnCollisionEnter(object2);

					//�����蔻��̎��s�֐�
					collision2->OnCollisionEnter(object1);

					if (m_IDGroup.find(object1->GetName()) != m_IDGroup.end() &&
						m_IDGroup.find(object2->GetName()) != m_IDGroup.end())
					{
						if (collision1->Find(collision2->GetID()) == -1)
						{
							collision1->Add(collision2->GetID());
						}
						if (collision2->Find(collision1->GetID()) == -1)
						{
							collision2->Add(collision1->GetID());
						}
							
					}
				}
				else
				{
					//�����蔻��̎��s�֐�
					collision1->NonCollisionEnter(object2);

					//�����蔻��̎��s�֐�
					collision2->NonCollisionEnter(object1);
				}
			}
		}

	}

	//���t���[���R���e�i�̒��g���N���A����
	m_UseHero.clear();
	m_UseOther.clear();
}

