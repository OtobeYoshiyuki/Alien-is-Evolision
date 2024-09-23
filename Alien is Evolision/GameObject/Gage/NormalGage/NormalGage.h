#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "TemplateState.h"
#include "NormalState.h"
class NormalGage:public OtobeLib::TextureBase,State<NORMALSTATE>
{
public:
	//�Q�[�W�̍ő�l
	static const int MAX_GAGE = 100;

	//�Q�[�W�̍ŏ��l
	static const int MIN_GAGE = 0;

private:
	//�v�Z�̕␳�l
	int m_Correction;

public:
	//�R���X�g���N�^
	NormalGage(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~NormalGage()override {};

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

	//�摜�̌��_��ݒ肷��
	void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { TextureBase::SetOrigin(origin); }

	//�摜�̃G�t�F�N�g��ݒ肷��
	void SetSpriteEffect(const DirectX::SpriteEffects& effect)override { TextureBase::SetSpriteEffect(effect); }

	//�摜�̍X�V�֐���ݒ肷��
	void SetActive(const bool& active)override { TextureBase::SetActive(active); }

	//----------------------�Z�b�^�[------------------------------------------------------------//


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

	//�摜�̌��_��ݒ肷��
	const DirectX::SimpleMath::Vector2& GetOrigin()const override { return TextureBase::GetOrigin(); }

	//�摜�̃G�t�F�N�g��ݒ肷��
	const DirectX::SpriteEffects& GetSpriteEffect()const override { return TextureBase::GetSpriteEffect(); }

	//�摜�̍X�V�֐���ݒ肷��
	const bool& GetActive()const override { return TextureBase::GetActive(); }

	//----------------�Q�b�^�[------------------------------------------------------------//

	public:

	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const NORMALSTATE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const NORMALSTATE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const NORMALSTATE& GetPreState()const override { return State::GetPreState(); };


public:

	//�Q�[�W�̕ύX���s��
	void ChangeGage();

	//�␳�l���擾����
	const int& GetCorrection()const { return m_Correction; }

};

