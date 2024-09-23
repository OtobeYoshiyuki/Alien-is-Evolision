#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "OtobeLib/Texture/Animation/Animation.h"
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"
#include "GameObject/MoveState/MoveState.h"
#include "TemplateState.h"
#include "GameObject/Enemy/ENEMYSTATE.h"
#include "OtobeLib/Status/StatusBase/StatusBase.h"
#include "GameObject/Gage/HPGage/HPGage.h"
#include "GameObject/FixdTurret/FixedTurret.h"
#include <list>
#include <vector>
#include "Effect/Experience/Experience.h"
#include "Effect/MasicEffectManager/MasicEffectManager.h"
#include "GameObject/Item/Heal/Heal.h"
#include "OtobeLib/Texture/TextureBase/ColorChange.h"

class SuperEnemy :public OtobeLib::TextureBase, OtobeLib::CollisionBase<OtobeLib::ITextureBase>,
	OtobeLib::Animation,StateArray<ENEMYSTATE,int,COLOR_CHANGE>, OtobeLib::StatusBase
{
public:
	//�Œ�C��̐�
	static const int TURRET_NUM = 2;

	//�v�l����
	static const float THINK_TIME;

	//�������E
	static const int MAX_LEVEL = 20;

	//�G�̋���(�o���l�ɉe������)
	static const float STRENGTH;

private:
	//HPGage
	HPGage* m_HpGage;

	//HPGage�̗���
	OtobeLib::ITextureBase* m_HpBack;

	//�Œ�C��̃��X�g
	std::vector<std::unique_ptr<FixedTurret>> m_TurrentList;

	//�v�l����
	float					  m_ThinkTime;

	//�o���l(�ǔ�)
	Experience* m_experience;

	//MasicPower�p�̃G�t�F�N�g
	std::unique_ptr<MasicEffectManager>	m_masicEffectManager;

	//�񕜃A�C�e��
	Heal* m_healItem;

	//�t���[�����̃J�E���g
	int m_frameCount;

	//�_���[�W�̃`�F�b�N�p�̃t���O
	bool							m_damageCheck;

public:
	//�R���X�g���N�^
	SuperEnemy(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~SuperEnemy()override {};

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Render()override;

	//�I��
	void Finalize()override;

	//----------------------�Z�b�^�[------------------------------------------------------------//

	//�e�N�X�`����ݒ肷��
	void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); };

	//�n���h����ݒ肷��
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); };

	//���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); };

	//�x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); };

	//�؂�o���摜�̏���ݒ肷��
	void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); };

	//�摜�̐F����ݒ肷��
	void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); };
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); }

	//�摜�̉�]�p�x��ݒ肷��
	void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); };

	//�摜�̊g��{����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); };

	//�摜�̏�Ԃ�ݒ肷��
	void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); };

	//�摜�̖��O��ݒ肷��
	void SetName(const std::string& name)override { TextureBase::SetName(name); };

	//�摜�̕`�揇��ݒ肷��
	void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); };

	//�摜�̃p�X��ݒ肷��
	void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); };

	//�A�j���[�V�����̏���ݒ肷��
	void SetAnimation(const std::string& name, const RECT& rect)override { Animation::SetAnimation(name, rect); }

	//���ɍĐ�����A�j���[�V�����̏���ݒ肷��
	void SetNextAnimation(const std::string& now, const std::string& next)override { Animation::SetNextAnimation(now, next); }

	//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
	void SetEndTime(const std::string& name, const int& time)override { Animation::SetEndTime(name, time); }

	//���݂̃A�j���[�V������ݒ肷��
	void SetNowAnimation(const RECT& rect)override { Animation::SetNowAnimation(rect); }

	//���݂̃A�j���[�V�����̏���ݒ肷��
	void SetNowInfo(const std::string& info)override { Animation::SetNowInfo(info); }

	//�A�j���[�V�����̃��[�v����ݒ肷��
	void SetLoopInfo(const std::string& loop)override { Animation::SetLoopInfo(loop); }

	//�A�j���[�V�����̍X�V�t���O��ݒ肷��
	void SetAniFlag(const bool& flag)override { Animation::SetAniFlag(flag); }

	//�A�j���[�V�����̃��[�v�t���O��ݒ肷��
	void SetLoopFlag(const bool& flag)override { Animation::SetLoopFlag(flag); }

	//�A�j���[�V�����̃p�����[�^�[�������p��
	void SetNewAnimation(Animation* ani)override { Animation::SetNewAnimation(ani); }

	//�A�j���[�V�����̃^�C�}�[������������
	void SetInitTimer(const int& timer)override { Animation::SetInitTimer(timer); }

	//�X�e�[�^�X��ݒ肷��
	void SetStatus(const int& hp, const int& mp, const int& atack, const int& defence,
		const int& special, const int& level, const int& next)override
	{
		StatusBase::SetStatus(hp, mp, atack, defence, special, level, next);
	}

	//�X�e�[�^�X�̍ő�l��ݒ肷��
	virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
		const int& maxdefence, const int& maxspecial, const int& maxlevel, const int& maxnext)override
	{
		StatusBase::SetMaxStatus(maxhp, maxmp, maxatack, maxdefence, maxspecial, maxlevel, maxnext);
	}

	//HP��ݒ肷��
	virtual void SetHP(const int& hp)override { StatusBase::SetHP(hp); }

	//MP��ݒ肷��
	virtual void SetMP(const int& mp)override { StatusBase::SetMP(mp); }

	//�U���͂�ݒ肷��
	virtual void SetAtack(const int& atack)override { StatusBase::SetAtack(atack); }

	//�h��͂�ݒ肷��
	virtual void SetDefence(const int& defence)override { StatusBase::SetDefence(defence); }

	//�X�y�V�����|�C���g��ݒ肷��
	virtual void SetSpecial(const int& special)override { StatusBase::SetSpecial(special); }

	//���x����ݒ肷��
	virtual void SetLevel(const int& level) override { StatusBase::SetLevel(level); };

	//�������E��ݒ肷��
	virtual void SetMaxLevel(const int& max)override { StatusBase::SetMaxLevel(max); };

	//���̃��x���܂ł̌o���l��ݒ肷��
	virtual void SetNextLevelPoint(const int& point)override { StatusBase::SetNextLevelPoint(point); };

	//----------------�Z�b�^�[-----------------------------------------------------------//

	//----------------�Q�b�^�[------------------------------------------------------------//

	//�n���h�����擾����
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); };

	//���W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition()const override { return TextureBase::GetPosition(); };

	//�x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetVelocity()const override { return TextureBase::GetVelocity(); };

	//�؂�o���摜�̏����擾����
	const RECT& GetRect()const override { return TextureBase::GetRect(); };

	//�摜�̐F�����擾����
	const DirectX::SimpleMath::Color& GetColor()const override { return TextureBase::GetColor(); };

	//�摜�̉�]�p�x���擾����
	const float& GetAngle()const override { return TextureBase::GetAngle(); };

	//�摜�̊g��{�����擾����
	const DirectX::SimpleMath::Vector3& GetScale()const override { return TextureBase::GetScale(); };

	//�摜�̏�Ԃ��擾����
	const ITextureBase::STATE& GetState()const override { return TextureBase::GetState(); };

	//�摜�̖��O���擾����
	const std::string& GetName()const override { return TextureBase::GetName(); };

	//�摜�̕`�揇���擾����
	const int& GetDrawOrder()const override { return TextureBase::GetDrawOrder(); };

	//�摜�̃p�X��ݒ肷��
	const std::wstring& GetPath()const override { return TextureBase::GetPath(); };

	//�A�j���[�V�����̏����擾����
	const RECT& GetAnimation(const std::string& name)override { return Animation::GetAnimation(name); }

	//���ɍĐ�����A�j���[�V�����̏����擾����
	const std::string& GetNextAnimation(const std::string& now)override { return Animation::GetNextAnimation(now); }

	//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
	const int& GetEndTime(const std::string& name)override { return Animation::GetEndTime(name); }

	//���݂̃A�j���[�V�������擾����
	const RECT& GetNowAnimation()const override { return Animation::GetNowAnimation(); }

	//���݂̃A�j���[�V�����̏���ݒ肷��
	const std::string& GetNowInfo()const override { return Animation::GetNowInfo(); }

	//�A�j���[�V�����̃��[�v����ݒ肷��
	const std::string& GetLoopInfo()const override { return Animation::GetLoopInfo(); }

	//�A�j���[�V�����̍X�V�t���O���擾����
	const bool& GetAniFlag()const override { return Animation::GetAniFlag(); }

	//�A�j���[�V�����̃��[�v�t���O���擾����
	const bool& GetLoopFlag()const override { return Animation::GetLoopFlag(); }

	//�A�j���[�V�����J�E���g���擾����
	const int& GetAniCount()const override { return Animation::GetAniCount(); }

	//HP���擾����
	const int& GetHP()const override { return StatusBase::GetHP(); }

	//MP���擾����
	virtual const int& GetMP()const override { return StatusBase::GetMP(); }

	//�U���͂��擾����
	virtual const int& GetAtack()const override { return StatusBase::GetAtack(); }

	//�h��͂��擾����
	virtual const int& GetDefence()const override { return StatusBase::GetDefence(); }

	//�X�y�V�����|�C���g���擾����
	virtual const int& GetSpecial()const override { return StatusBase::GetSpecial(); }

	//���x����ݒ肷��
	virtual const int& GetLevel()const override { return StatusBase::GetLevel(); };

	//�������E��ݒ肷��
	virtual const int& GetMaxLevel()const override { return StatusBase::GetMaxLevel(); }

	//���̃��x���܂ł̌o���l��ݒ肷��
	virtual const int& GetNextLevelPoint()const { return StatusBase::GetNextLevelPoint(); }

	//���g�̃X�e�[�^�X���擾����
	virtual IStatusBase* GetStatus()override { return StatusBase::GetStatus(); }

	//----------------�Q�b�^�[-------------------------------------------------------------//

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

	//���g�̓����蔻����擾����
	ICollisionBase<ITextureBase>* GetThisCollision() { return CollisionBase::GetThisCollision(); };

	//�����������ǂ����̃t���O���擾����
	const bool& GetCheck()const override { return CollisionBase::GetCheck(); };

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

public:
	//���݂̏�Ԃ�ݒ肷��
	void AddData1(const ENEMYSTATE& state)override { StateArray::AddData1(state); };
	void AddData2(const int& state)override { StateArray::AddData2(state); };
	void AddData3(const COLOR_CHANGE& state)override { StateArray::AddData3(state); };

	//���݂̏�Ԃ��擾����
	const ENEMYSTATE& GetNowData1()const override { return StateArray::GetNowData1(); };
	const int& GetNowData2()const override { return StateArray::GetNowData2(); };
	const COLOR_CHANGE& GetNowData3()const override { return StateArray::GetNowData3(); };

	//�ЂƂO�̏�Ԃ��擾����
	const ENEMYSTATE& GetPreData1()const override { return StateArray::GetPreData1(); };
	const int& GetPreData2()const override { return StateArray::GetPreData2(); };
	const COLOR_CHANGE& GetPreData3()const override { return StateArray::GetPreData3(); };


	//�_���[�W���󂯂�
	void OnDamage(const int& atack, const int& defence, const float correction = 1.0f);
private:
	//�Q�[�W�̍X�V����
	void UpdateGage();

	//�X�e�[�g�̍X�V����
	void UpdateState();

	//State_None�̏���
	void State_None();

	//State_Start�̏���
	void State_Start();

	//State_Atacking�̏���
	void State_Atacking();

	//State_AtackResult�̏���
	void State_AtackResult();

	//State_Next�̏���
	void State_Next();

	//State_DeadStart�̏���
	void State_DeadStart();

	//State_Deadting�̏���
	void State_Deadting();

	//State_Dead�̏���
	void State_Dead();

	//�񕜃A�C�e���̃h���b�v�̏���
	void HealItemDrop();

	//Player�ɓ����������̏���
	void OnCollision_Player(ITextureBase* obj);

	//NeedleOverAll�ɓ��������Ƃ��̏���
	void OnCollision_NeedleOverAll(ITextureBase* obj);

	//Resurrection�ɓ��������Ƃ��̏���
	void OnCollision_Resurrection(ITextureBase* obj);

	//VirusBeam�ɓ��������Ƃ��̏���
	void OnCollision_VirusBeam(ITextureBase* obj);

	//NormalBullet�ɓ����������̏���
	void OnCollision_NormalBullet(ITextureBase* obj);

public:
	//�Œ�C��̃��X�g���擾����
	std::vector<std::unique_ptr<FixedTurret>>& GetTurretList() { return m_TurrentList; }

	//MasicEffectManager���擾����
	MasicEffectManager* GetMasicEffectManager() { return m_masicEffectManager.get(); }
};

