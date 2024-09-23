#pragma once
#include "GameObject/Stage/Maptip/MapTip.h"
#include "OtobeLib/Texture/Animation/Animation.h"
class Wall:public MapTip, OtobeLib::Animation
{
public:
	//�R���X�g���N�^
	Wall(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~Wall()override {};

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void Render()override;

	//�I��
	void Finalize()override;

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

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
};

