#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "TemplateState.h"
#include "GameObject/DamageUI/FadeState.h"

namespace OtobeLib
{
	class FadeTexture:public TextureBase,State<FADE_STATE>
	{
	public:
		//�t�F�[�h�̏����l
		static const float DEFALT_FADE_SPEED;

		//�����x�̍ő�l
		static const float DEFALT_MAX_ALFA;

		//�����x�̍ŏ��l
		static const float DEFALT_MIN_ALFA;
	private:
		//�t�F�[�h�̉��Z���x
		float	m_fadeSpeed;

		//�t�F�[�h�̃A���t�@�l�̍ő�l
		float	m_maxAlfa;

		//�t�F�[�h�̃A���t�@�l�̍ŏ��l
		float	m_minAlfa;
	public:
		//�R���X�g���N�^
		FadeTexture(OtobeLib::ITextureBase* texture);

		//�f�X�g���N�^
		~FadeTexture()override {};

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

	public:
		//���݂̏�Ԃ�ݒ肷��
		void SetNowState(const FADE_STATE& now)override { State::SetNowState(now); };

		//���݂̏�Ԃ��擾����
		const FADE_STATE& GetNowState()const override { return State::GetNowState(); };

		//�ЂƂO�̏�Ԃ��擾����
		const FADE_STATE& GetPreState()const override { return State::GetPreState(); };

	public:
		//�t�F�[�h�̉��Z���x��ݒ肷��
		void SetFadeSpeed(const float& speed) { m_fadeSpeed = speed; }

		//�t�F�[�h�̃A���t�@�l�̍ŏ��l��ݒ肷��
		void SetMinAlfa(const float& min) { m_minAlfa = min; }

		//�t�F�[�h�̃A���t�@�l�̍ő�l��ݒ肷��
		void SetMaxAlfa(const float& max) { m_maxAlfa = max; }

		//�t�F�[�h�̉��Z���x���擾����
		const float& GetFadeSpeed()const { return m_fadeSpeed; }

		//�t�F�[�h�̃A���t�@�l�̍ŏ��l���擾����
		const float& GetMinAlfa()const { return m_minAlfa; }

		//�t�F�[�h�̃A���t�@�l�̍ő�l���擾����
		const float& GetMaxAlfa()const { return m_maxAlfa; }
	};
}

