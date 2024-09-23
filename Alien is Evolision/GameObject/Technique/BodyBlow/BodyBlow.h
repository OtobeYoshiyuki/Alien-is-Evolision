#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/Texture/Animation/Animation.h"
class BodyBlow:public OtobeLib::TextureBase,OtobeLib::Animation
{
public:
	//�Q�[�W�̉񕜗�(�R�}���h�Z)
	static const int TECHNIQUE_HEAL_COMMAND = 30;

	//�Q�[�W�̉񕜗�(�K�E�Z)
	static const int TECHNIQUE_HEAL_SPECIAL = 40;

	//�擾�X�R�A
	static const int BASE_SCORE = 300;

	//�R�}���h�Z�̃Q�[�W�̏����
	static const int GAGE_COST = 50;
public:
	//�R���X�g���N�^
	BodyBlow(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~BodyBlow()override {};

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
	void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); };

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
	void SetAnimation(const std::string& name, const RECT& rect)override { Animation::SetAnimation(name,rect); }

	//���ɍĐ�����A�j���[�V�����̏���ݒ肷��
	void SetNextAnimation(const std::string& now, const std::string& next)override { Animation::SetNextAnimation(now,next); }

	//�A�j���[�V�����̏I�����Ԃ�ݒ肷��
	void SetEndTime(const std::string& name, const int& time)override { Animation::SetEndTime(name,time); }

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
	const std::string& GetNowInfo()const override{ return Animation::GetNowInfo(); }

	//�A�j���[�V�����̃��[�v����ݒ肷��
	const std::string& GetLoopInfo()const override{ return Animation::GetLoopInfo(); }

	//�A�j���[�V�����̍X�V�t���O���擾����
	const bool& GetAniFlag()const override{ return Animation::GetAniFlag(); }

	//�A�j���[�V�����̃��[�v�t���O���擾����
	const bool& GetLoopFlag()const override{ return Animation::GetLoopFlag(); }

	//�A�j���[�V�����J�E���g���擾����
	const int& GetAniCount()const override{ return Animation::GetAniCount(); }

	//----------------�Q�b�^�[-------------------------------------------------------------//

	//�v���C���[�Ɠ����s�����Ƃ�
	void WithPlayer(OtobeLib::ITextureBase* texture);
};

