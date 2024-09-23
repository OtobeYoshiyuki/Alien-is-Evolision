#pragma once
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/ICollisionBase.h"
#include "OtobeLib/Status/Interface/IStatusBase.h"
#include "OtobeLib/Texture/Animation/Animation.h"

namespace OtobeLib
{
	class ITextureBase
	{
	public:
		enum class STATE :int
		{
			NONE,//�`�悵�Ȃ�
			DRAW,//�`�悷��
		};

	public:
		//�o�[�`�����f�X�g���N�^�i�p����̃N���X�̃C���X�^���X�̃f�X�g���N�^���Ă΂��ۂɕK�v�j
		virtual ~ITextureBase() {};

		//������
		virtual void Initialize() = 0;

		//�X�V
		virtual void Update() = 0;

		//�`��
		virtual void Render() = 0;

		//�I��
		virtual void Finalize() = 0;

		//----------------------�Z�b�^�[------------------------------------------------------------//

		//�e�N�X�`����ݒ肷��
		virtual void SetTexture(ITextureBase* texture) = 0;
		
		//�n���h����ݒ肷��
		virtual void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle) = 0;

		//���W��ݒ肷��
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) = 0;

		//�x�N�g����ݒ肷��
		virtual void SetVelocity(const DirectX::SimpleMath::Vector3& vel) = 0;

		//�؂�o���摜�̏���ݒ肷��
		virtual void SetRect(const RECT& rect) = 0;

		//�摜�̐F����ݒ肷��
		virtual void SetColor(const DirectX::SimpleMath::Color& color) = 0;
		virtual void SetColor(const DirectX::XMVECTORF32& color) = 0;

		//�摜�̉�]�p�x��ݒ肷��
		virtual void SetAngle(const float& angle) = 0;

		//�摜�̊g��{����ݒ肷��
		virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;

		//�摜�̏�Ԃ�ݒ肷��
		virtual void SetState(const STATE& state) = 0;

		//�摜�̖��O��ݒ肷��
		virtual void SetName(const std::string& name) = 0;

		//�摜�̕`�揇��ݒ肷��
		virtual void SetDrawOrder(const int& order) = 0;

		//�摜�̃p�X��ݒ肷��
		virtual void SetPath(const std::wstring& path) = 0;

		//�폜�t���O��ݒ肷��
		virtual void SetIsVal(const bool& val) = 0;

		//�摜�̒��S�ʒu��ݒ肷��
		virtual void SetOrigin(const DirectX::SimpleMath::Vector2& origin) = 0;

		//�X�V�֐���ݒ肷��
		virtual void SetActive(const bool& active) = 0;

		//----------------�Z�b�^�[-----------------------------------------------------------//

		//----------------�Q�b�^�[------------------------------------------------------------//

		//�n���h����ݒ肷��
		virtual const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const = 0;

		//���W��ݒ肷��
		virtual const DirectX::SimpleMath::Vector3& GetPosition()const = 0;

		//�x�N�g����ݒ肷��
		virtual const DirectX::SimpleMath::Vector3& GetVelocity()const = 0;

		//�؂�o���摜�̏���ݒ肷��
		virtual const RECT& GetRect()const = 0;

		//�摜�̐F����ݒ肷��
		virtual const DirectX::SimpleMath::Color& GetColor()const = 0;

		//�摜�̉�]�p�x��ݒ肷��
		virtual const float& GetAngle()const = 0;

		//�摜�̊g��{����ݒ肷��
		virtual const DirectX::SimpleMath::Vector3& GetScale()const = 0;

		//�摜�̏�Ԃ�ݒ肷��
		virtual const STATE& GetState()const = 0;

		//�摜�̖��O��ݒ肷��
		virtual const std::string& GetName()const = 0;

		//�摜�̕`�揇���擾����
		virtual const int& GetDrawOrder()const = 0;

		//�摜�̃p�X��ݒ肷��
		virtual const std::wstring& GetPath()const = 0;

		//�폜�t���O��ݒ肷��
		virtual const bool& GetIsVal()const = 0;

		//�摜�̒��S�ʒu���擾����
		virtual const DirectX::SimpleMath::Vector2& GetOrigin()const = 0;

		//�X�V�֐����擾����
		virtual const bool& GetActive()const = 0;

		//----------------�Q�b�^�[-------------------------------------------------------------//


		//----------------�Z�b�^�[--------------------------------------------------------------//

		//���݂̏�Ԃ�ݒ肷��
		virtual void SetSpriteEffect(const DirectX::SpriteEffects& effect) = 0;

		//�`�揇��ύX����(�������́A���X�g�Ɋ֌W�Ȃ����ёւ������)
		virtual void SetLayerDenpth(const float& denpth) = 0;

		//----------------�Z�b�^�[--------------------------------------------------------------//

		//----------------�Q�b�^�[--------------------------------------------------------------//

		//���݂̏�Ԃ��擾����
		virtual const DirectX::SpriteEffects& GetSpriteEffect()const = 0;

		//�`�揇���擾����(�������́A���X�g�Ɋ֌W�Ȃ����ёւ������)
		virtual const float& GetLayerDenpth() = 0;

		//----------------�Q�b�^�[--------------------------------------------------------------//
		
		//�摜�̐؂�o���ʒu����摜�̒��S�_�����߂�
		virtual const DirectX::SimpleMath::Vector2 AskOrigin()const = 0;

		//�摜�������o��
		virtual void Extrude(ITextureBase* texture) = 0;
		virtual void ExtrudeX(ITextureBase* texture) = 0;
		virtual void ExtrudeY(ITextureBase* texture) = 0;

		//���W�̏C��
		virtual void PositionCorrection(ITextureBase* texture) = 0;

		//���g�̓����蔻����擾����
		virtual ICollisionBase<ITextureBase>* GetThisCollision() = 0;

		//���g�̃X�e�[�^�X���擾����
		virtual IStatusBase* GetStatus() = 0;

		//�E�������A�����������擾����
		virtual const int DirectX() = 0;

		//�A�j���[�V�������擾����
		virtual Animation* GetThisAnimation() = 0;
	};
	
}

