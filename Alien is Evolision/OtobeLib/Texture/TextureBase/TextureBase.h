#pragma once
#include "OtobeLib/Texture/Interface/ITextureBase.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
#include <functional>
#include <vector>
namespace OtobeLib
{
	class TextureBase:public ITextureBase,Blinking
	{
		//----------------�ϐ�-------------------------------------------------------------------//
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_Handle;	//�O���t�B�b�N�n���h��

		DirectX::SimpleMath::Vector3						m_Position;	//���W

		DirectX::SimpleMath::Vector3						m_Velocity;	//�x�N�g��

		RECT												m_Rect;		//�n�_�y�яI�_

		DirectX::SimpleMath::Color							m_Color;	//�F

		float												m_Angle;	//��]�i�f�O���[�j

		DirectX::SimpleMath::Vector3						m_Scale;	//�g��{��

		std::string											m_Name;		//���O

		STATE												m_State;	//���

		int													m_DrawOrder;//�`�揇

		std::wstring										m_Path;		//�摜�̃p�X

		bool												m_IsVal;	//�폜�t���O

		DirectX::SimpleMath::Vector2						m_Origin;	//�摜�̒��S�ʒu

		DirectX::SpriteEffects								m_Effect;	//�摜�p�̃G�t�F�N�g

		bool												m_Active;	//�X�V�֐�

		float												m_LayerDenpth;//�`�揇�i���X�g�ɉe������Ȃ��j
		//----------------�ϐ�--------------------------------------------------------------------//
	public:
		//�R���X�g���N�^
		TextureBase();

		//�R���X�g���N�^
		TextureBase(ITextureBase* texture);

		//�f�X�g���N�^
		virtual ~TextureBase();

		//������
		virtual void Initialize()override;

		//�X�V
		virtual void Update()override;

		//�`��
		virtual void Render()override;

		//�I��
		virtual void Finalize()override;

		//----------------------�Z�b�^�[------------------------------------------------------------//

		//�e�N�X�`����ݒ肷��
		virtual void SetTexture(ITextureBase* texture)override;

		//�n���h����ݒ肷��
		virtual void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { m_Handle = handle; }

		//���W��ݒ肷��
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { m_Position = pos; };

		//�x�N�g����ݒ肷��
		virtual void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { m_Velocity = vel; };

		//�؂�o���摜�̏���ݒ肷��
		virtual void SetRect(const RECT& rect)override { m_Rect = rect; };

		//�摜�̐F����ݒ肷��
		virtual void SetColor(const DirectX::SimpleMath::Color& color)override { m_Color = color; };
		virtual void SetColor(const DirectX::XMVECTORF32& color)override { m_Color = color; };

		
		//�摜�̉�]�p�x��ݒ肷��
		virtual void SetAngle(const float& angle)override { m_Angle = angle; };

		//�摜�̊g��{����ݒ肷��
		virtual void SetScale(const DirectX::SimpleMath::Vector3& scale)override { m_Scale = scale; };

		//�摜�̏�Ԃ�ݒ肷��
		virtual void SetState(const ITextureBase::STATE& state)override { m_State = state; };

		//�摜�̖��O��ݒ肷��
		virtual void SetName(const std::string& name)override { m_Name = name; };

		//�摜�̕`�揇��ݒ肷��
		virtual void SetDrawOrder(const int& order)override { m_DrawOrder = order; };

		//�摜�̃p�X��ݒ肷��
		virtual void SetPath(const std::wstring& path)override { m_Path = path; };

		//�폜�t���O��ݒ肷��
		virtual void SetIsVal(const bool& val)override { m_IsVal = val; }

		//�摜�̒��S�ʒu��ݒ肷��
		virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { m_Origin = origin; };

		//���݂̏�Ԃ�ݒ肷��
		virtual void SetSpriteEffect(const DirectX::SpriteEffects& effect)override { m_Effect = effect; };

		//�`�揇��ύX����(�������́A���X�g�Ɋ֌W�Ȃ����ёւ������)
		virtual void SetLayerDenpth(const float& denpth) { m_LayerDenpth = denpth; };

		//�X�V�֐���ݒ肷��
		virtual void SetActive(const bool& active)override { m_Active = active; };

		//�_�ł̐ݒ������
		virtual void SetBlink(const bool& blink)override { Blinking::SetBlink(blink); }

		//�_�ł���Ԋu��ݒ肷��
		virtual void SetTarget(const float& target) { Blinking::SetTarget(target); }

		//�_�ł̏I�����Ԃ̐ݒ������
		virtual void SetFinal(const int& Final) { Blinking::SetFinal(Final); }
		
		//�_�ł̏I�����Ԃ�K�����邩
		virtual void SetFinalOption(const bool& finish) { Blinking::SetFinalOption(finish); }
		//----------------�Z�b�^�[-----------------------------------------------------------//

		//----------------�Q�b�^�[------------------------------------------------------------//

		//�n���h�����擾����
		virtual const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return m_Handle; }

		//���W���擾����
		virtual const DirectX::SimpleMath::Vector3& GetPosition()const override { return m_Position; };

		//�x�N�g�����擾����
		virtual const DirectX::SimpleMath::Vector3& GetVelocity()const override { return m_Velocity; };

		//�؂�o���摜�̏����擾����
		virtual const RECT& GetRect()const override { return m_Rect; };

		//�摜�̐F�����擾����
		virtual const DirectX::SimpleMath::Color& GetColor()const override { return m_Color; };

		//�摜�̉�]�p�x���擾����
		virtual const float& GetAngle()const override { return m_Angle; };

		//�摜�̊g��{�����擾����
		virtual const DirectX::SimpleMath::Vector3& GetScale()const override { return m_Scale; };

		//�摜�̏�Ԃ��擾����
		virtual const ITextureBase::STATE& GetState()const override { return m_State; };

		//�摜�̖��O���擾����
		virtual const std::string& GetName()const override { return m_Name; };

		//�摜�̕`�揇���擾����
		virtual const int& GetDrawOrder()const override { return m_DrawOrder; };

		//�摜�̃p�X��ݒ肷��
		virtual const std::wstring& GetPath()const override { return m_Path; };

		//�폜�t���O��ݒ肷��
		virtual const bool& GetIsVal()const override { return m_IsVal; };

		//�摜�̒��S�ʒu���擾����
		virtual const DirectX::SimpleMath::Vector2& GetOrigin()const override { return m_Origin; };

		//���݂̏�Ԃ��擾����
		virtual const DirectX::SpriteEffects& GetSpriteEffect()const override { return m_Effect; };

		//�`�揇���擾����(�������́A���X�g�Ɋ֌W�Ȃ����ёւ������)
		virtual const float& GetLayerDenpth() { return m_LayerDenpth; }

		//�X�V�֐����擾����
		virtual const bool& GetActive()const { return m_Active; };

		//�_�ł̎擾
		virtual bool NowBlinking()override { return Blinking::NowBlinking(); };

		//�_�Œ����ǂ���
		virtual const bool& GetBlink()const override{ return Blinking::GetBlink(); }

		//�t���[�������擾����
		virtual const float& GetFrameCount()const override { return Blinking::GetFrameCount(); }

		//�_�ł̏I�����Ԃ��擾����
		virtual const int& GetFinal()const override { return Blinking::GetFinal(); }

		//�_�ł̊Ԋu���擾����
		virtual const float& GetTarget()const override { return Blinking::GetTarget(); }

		//�_�ł̃t���[�������擾����
		virtual const float& GetTargetCount()const { return Blinking::GetTargetCount(); }

		//----------------�Q�b�^�[-------------------------------------------------------------//

		//�摜�̐؂�o���ʒu����摜�̒��S�_�����߂�
		const DirectX::SimpleMath::Vector2 AskOrigin()const override;

		//�摜�������o��
		void Extrude(ITextureBase* texture)override;

		void ExtrudeX(ITextureBase* texture)override;

		void ExtrudeY(ITextureBase* texture)override;

		//�`��n�𔽓]������
		void ChangeSpriteEffect();

		//���W�̏C��
		void PositionCorrection(ITextureBase* texture)override;

		//���g�̓����蔻����擾����
		virtual ICollisionBase<ITextureBase>* GetThisCollision()override { return nullptr; }

		//�A�j���[�V�������擾����
		virtual Animation* GetThisAnimation()override { return nullptr; }

		//���g�̃X�e�[�^�X���擾����
		virtual IStatusBase* GetStatus()override { return nullptr; };

		//�E�������A�����������擾����
		const int DirectX()override;

	public:
	//�e�N�X�`���̕`��
		void DrawTexture();

	public:
		//�ϊ�
		static const std::vector<TextureBase> Cast(const std::vector<std::vector<std::string>>& input);
		//std::string����std::wstring�ւ̕ϊ�
		static const std::wstring Convert(const std::string byteString);
	};
}

