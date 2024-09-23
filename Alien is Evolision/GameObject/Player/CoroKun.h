#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "GameObject/MoveState/MoveState.h"
#include <list>
#include <vector>
#include <functional>
#include "GameObject/MoveState/DamageState/DamageState.h"
#include "GameObject/MoveState/WalkState/WalkState.h"
#include "GameObject/MoveState/AccelState/AccelState.h"
#include "GameObject/Technique/BodyBlow/BodyBlow.h"
#include "GameObject/Technique/VirusBeam/VirusBeam.h"
#include "OtobeLib/Texture/Animation/Animation.h"
#include "GameObject/Technique/NeedleOverall/NeedleOverall.h"
#include "GameObject/Technique/Resurrection/Resurrection.h"
#include "GameObject/Player/NormalTechnique/NormalTechnique.h"
#include "OtobeLib/Status/StatusBase/StatusBase.h"
#include "GameObject/Gage/HPGage/HPGage.h"
#include "GameObject/Gage/ExperienceGage/ExperienceGage.h"
#include "GameObject/Gage/TechniqueGage/TechniqueGage.h"
#include "GameObject/Gage/DeathblowGage/DeathblowGage.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"
#include "PLAYERANIME.h"
#include "OtobeLib/Texture/TextureBase/ColorChange.h"
#include "OtobeLib/Texture/FadeTexture/FadeTexture.h"

class CoroKun:public OtobeLib::TextureBase,OtobeLib::CollisionBase<OtobeLib::ITextureBase>,
	OtobeLib::Animation,OtobeLib::StatusBase,
	StateArray<FADE_STATE,PLAYERMOVE,COLOR_CHANGE>
{
public:
	//�s����͈͂̍ő�l
	static const DirectX::SimpleMath::Vector3 MAX_LIMIT;

	//�s����͈͂̍ŏ��l
	static const DirectX::SimpleMath::Vector3 MIN_LIMIT;

	//1�t���[���ɉ��Z����X�s�[�h
	static const float SPEED;

	//Easing�֐��Ɏg�p����g�[�^���^�C��
	static const float EASING_TOTALTIME;

	//Easing�֐��Ɏg�p����ő�l
	static const float EASING_MAX;

	//Easing�֐��Ɏg�p����ŏ��l
	static const float EASING_MIN;

	//�������E
	static const int MAX_LEVEL = 20;

	//�������x��
	static const int MIN_LEVEL = 1;

	//���o�[�X�A�r���e�B�̎�������
	static const float DEATHBLOW_ABILITY_SUSTAIN_TIME;

	//�w�ʂ̓_�ł̊J�n����
	static const float BACKTEXTURE_BLINK_START;

private:
	//HP�Q�[�W
	HPGage* m_HPGage;

	//MP�Q�[�W
	TechniqueGage* m_mpGage;

	//Special�Q�[�W
	DeathblowGage* m_specialGage;

	//�o���l�Q�[�W
	ExperienceGage* m_expGage;

	//�w�ʂ̉摜
	TextureBase* m_backTexture;

	//���̉摜
	TextureBase* m_sweatTexture;

	//�C�[�W���O�֐��̃J�E���g�p�̕ϐ�
	float m_easingTime;

	//�o�ߎ���
	float m_timer;

	//���Ԃ̍X�V
	bool  m_timerUp;

	//�t���[�����̃J�E���g
	int m_frameCount;

	//�v���C���[�̃X�e�[�g�̃��X�g
	std::vector<MoveState*>		m_StateList;

	//BodyBlow
	BodyBlow*					m_BodyBlow;

	//VirusBeam
	VirusBeam*					m_VirusBeam;

	//NormalTechnique
	std::unique_ptr<NormalTechnique>	m_Normal;

	//NeedleOverAll
	std::unique_ptr<NeedleOverall>				m_NeedleOverAll;

	//Resurrection
	std::unique_ptr<Resurrection>				m_Resurrection;

	//WalkState
	std::unique_ptr<WalkState>		m_WalkState;

	//DamageState
	std::unique_ptr<DamageState>	m_DamageState;

	//AccelState
	std::unique_ptr<AccelState>		m_AccelState;

	//���V��̍��W
	DirectX::SimpleMath::Vector3	m_warpPos;

	//�_���[�W�̃`�F�b�N�p�̃t���O
	bool							m_damageCheck;

	//�̓�����̃R�}���h���̃G�t�F�N�g
	ITextureBase*					m_fadeBodyBlow;

	//�r�[���̃R�}���h���̃G�t�F�N�g
	ITextureBase*					m_fadeBeam;

	//�j�[�h���C���N�W�F�N�V�����̃R�}���h���̃G�t�F�N�g
	ITextureBase*					m_fadeNeedleOverAll;

	//���U���N�V�����̃R�}���h���̃G�t�F�N�g
	ITextureBase*					m_fadeResurection;

	//�K�E�Z��UI�̃G�t�F�N�g
	OtobeLib::FadeTexture*			m_fadeSpecial;

public:
	//�R���X�g���N�^
	CoroKun(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~CoroKun()override {};

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
	void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); }

	//�n���h����ݒ肷��
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); }

	//���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); }

	//�x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); }

	//�؂�o���摜�̏���ݒ肷��
	void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); }

	//�摜�̐F����ݒ肷��
	void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); }
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); }

	//�摜�̉�]�p�x��ݒ肷��
	void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); }

	//�摜�̊g��{����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); }

	//�摜�̏�Ԃ�ݒ肷��
	void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); }

	//�摜�̖��O��ݒ肷��
	void SetName(const std::string& name)override { TextureBase::SetName(name); }

	//�摜�̕`�揇��ݒ肷��
	void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); }

	//�摜�̃p�X��ݒ肷��
	void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); }

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

	//�X�e�[�^�X��ݒ肷��
	void SetStatus(const int& hp, const int& mp, const int& atack, const int& defence,
		const int& special, const int& level, const int& next)override
		{	StatusBase::SetStatus(hp,mp,atack,defence,special,level,next);	}

	//�X�e�[�^�X�̍ő�l��ݒ肷��
	virtual void SetMaxStatus(const int& maxhp, const int& maxmp, const int& maxatack,
		const int& maxdefence, const int& maxspecial, const int& maxlevel,const int& maxnext)override
	{
		StatusBase::SetMaxStatus(maxhp, maxmp, maxatack, maxdefence, maxspecial, maxlevel,maxnext);
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

	//HP�̍ő�l��ݒ肷��
	virtual void SetMaxHP(const int& max)override { StatusBase::SetMaxHP(max); }

	//���̃��x���܂ł̌o���l��ݒ肷��
	virtual void SetNextLevelPoint(const int& point)override { StatusBase::SetNextLevelPoint(point); };

	//----------------�Z�b�^�[-----------------------------------------------------------//

	//----------------�Q�b�^�[------------------------------------------------------------//

	//�n���h�����擾����
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); }

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

	//���g�̃A�j���[�V�������擾����
	Animation* GetThisAnimation()override { return Animation::GetThisAnimation(); }

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

	//���x�����擾����
	virtual const int& GetLevel()const override { return StatusBase::GetLevel(); };

	//�������E���擾����
	virtual const int& GetMaxLevel()const override { return StatusBase::GetMaxLevel(); };

	//HP�̍ő�l���擾����
	virtual const int& GetMaxHP()const override { return StatusBase::GetMaxHP(); }

	//MP�̍ő�l���擾����
	virtual const int& GetMaxMP()const override { return StatusBase::GetMaxMP(); }

	//Special�̍ő�l���擾����
	virtual const int& GetMaxSpecialPoint()const override { return StatusBase::GetMaxSpecialPoint(); }

	//���̃��x���܂ł̌o���l��ݒ肷��
	virtual const int& GetNextLevelPoint()const { return StatusBase::GetNextLevelPoint(); };

	//�o���l�̍ő�l��ݒ肷��
	virtual const int& GetMaxNextLevelPoint()const { return StatusBase::GetMaxNextLevelPoint(); }

	//���g�̃X�e�[�^�X���擾����
	virtual IStatusBase* GetStatus()override { return StatusBase::GetStatus(); };


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
	void AddData1(const FADE_STATE& state)override { StateArray::AddData1(state); };
	void AddData2(const PLAYERMOVE& state)override { StateArray::AddData2(state); };
	void AddData3(const COLOR_CHANGE& state)override { StateArray::AddData3(state); };

	//���݂̏�Ԃ��擾����
	const FADE_STATE& GetNowData1()const override { return StateArray::GetNowData1(); };
	const PLAYERMOVE& GetNowData2()const override { return StateArray::GetNowData2(); };
	const COLOR_CHANGE& GetNowData3()const override { return StateArray::GetNowData3(); };

	//�ЂƂO�̏�Ԃ��擾����
	const FADE_STATE& GetPreData1()const override { return StateArray::GetPreData1(); };
	const PLAYERMOVE& GetPreData2()const override { return StateArray::GetPreData2(); };
	const COLOR_CHANGE& GetPreData3()const override { return StateArray::GetPreData3(); };

private:
	//�v���C���[�̃X�e�[�g�̏���
	void PlayerState();

	//����
	void Move();

	//���S���̏���
	void SweatAnime();

	//��]�Ək���̏���
	void RotScale();

	//���S�A�j���[�V�����J�n�̏���
	void DeadAnimeStart();

	//���S�A�j���[�V�����I���̏���
	void DeadAnimeResult();

	//�X�e�[�g�̍X�V
	void UpdateState();

	//�Z�̍X�V����
	void UpdateTechnique();

	//�C�[�W���O�֐��̍X�V����
	void UpdateEasing();

	//�w�ʂ̃t�F�[�h�̍X�V����
	void UpdateBackFade();

	//Hp�Q�[�W�̍X�V����
	void UpdateHpGage();

	//�R�}���h�G�t�F�N�g�̍X�V����
	void UpdateCommandEffect();

	//�w�肵���R�}���h�G�t�F�N�g���Đ�
	void PlayCommandEffect(ITextureBase* command, const int& cost);

	//�K�E�Z�G�t�F�N�g���Đ�
	void PlaySpecialEffect(OtobeLib::FadeTexture* special, const int& cost,const DirectX::XMVECTORF32 color);

	//���ɓ���������
	void OnCollisionFloor(ITextureBase* floor);

	//�ǂɓ���������
	void OnCollisionWall(ITextureBase* wall);

	//�A�ɓ��������Ƃ�
	void OnCollisionBabul(ITextureBase* babul);

	//���]��ɓ��������Ƃ�
	void OnCollisionRebound(ITextureBase* rebound);

	//�N���X�C�b�`�ɓ��������Ƃ�
	void OnCollisionSwitch(ITextureBase* switchi);

	//���Ƃ����ɓ��������Ƃ�
	void OnCollisionFoleFloor(ITextureBase* fole);

	//HyperEnemy�ɓ��������Ƃ�
	void OnCollisionHyperEnemy(ITextureBase* hyene);

	//SuperEnemy�ɓ��������Ƃ�
	void OnCollision_SuperEnemy(ITextureBase* super);

	//NormalEnemy�ɓ��������Ƃ�
	void OnCollision_NormalEnemy(ITextureBase* normal);

	//UI�ɓ���������
	void OnCollisionWhiteBack(ITextureBase* whiteback);

	//HyperEnemy�̖A�ɓ���������
	void OnCollisionHyperBabble(ITextureBase* highbabble);

	//SuperEnemy�̖A�ɓ��������Ƃ�
	void OnCollision_SuperBabble(ITextureBase* superbabble);

	//�S�[���ɓ��������Ƃ�
	void OnCollision_Goal(ITextureBase* goal);

	//�p�x��ύX����
	void InputAngle();

	//�Z�𐶐�����
	void CreateTechnique();

public:
	//�_���[�W���󂯂�
	void OnDamage(const int& atack,const int& defence,const float correction = 1.0f);

	//�R�}���h�Z���g�p����
	void OnCommand(const int& cost);

	//�K�E�Z�Q�[�W���g�p����
	void OnSpesial(const int& cost);

	//�R�}���h�Q�[�W���񕜂�����
	void OnHealCommand(int& heal);

	//�K�E�Z�Q�[�W���񕜂�����
	void OnHealSpecial(int& heal);

	//HP�Q�[�W���񕜂�����
	void OnHealHp(int& heal);

	//�o���l�Q�[�W���񕜂�����
	void OnHealExp(int& heal);

	//�I�����̏���
	void Finish(const PLAYERMOVE& state, const int& sound);

	//�_���[�W�t���O��ݒ肷��
	void SetDamageCheck(const bool& check) { m_damageCheck = check; }

private:
	//�K�E�Z�����s
	void OnExeDeathBlowHyperEnemy();
	void OnExeDeathBlowSuperEnemy();
	void OnExeDeathBlowNormalEnemy();
public:
	//�S�Ă̋Z���g�p����Ă��邩�ǂ������ׂ�
	const bool ALLTechniqueExe();

public:
	//WalkState���擾����
	std::unique_ptr<WalkState>& GetWalkState() { return m_WalkState; }

	//BodyBlow���擾����
	BodyBlow* GetBodyBlow() { return m_BodyBlow; }

	//ExperienceGage���擾����
	ExperienceGage* GetExperienceGage() { return m_expGage; }

	//HPGage���擾����
	HPGage* GetHpGage() { return m_HPGage; }

	//TechniqueGage���擾����
	TechniqueGage* GetTechniqueGage() { return m_mpGage; }

	//DeathblowGage���擾����
	DeathblowGage* GetDeathblowGage() { return m_specialGage; }

	//�K�E�Z��UI���擾����
	OtobeLib::FadeTexture* GetSpecialUI() { return m_fadeSpecial; }
};
