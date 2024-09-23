#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
class Particle :public OtobeLib::TextureBase, OtobeLib::CollisionBase<OtobeLib::ITextureBase>
{
public:
	//�R���X�g���N�^
	Particle(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~Particle()override {};

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
	virtual void SetTexture(ITextureBase* texture)override { TextureBase::SetTexture(texture); };

	//�n���h����ݒ肷��
	virtual void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { TextureBase::SetHandle(handle); };

	//���W��ݒ肷��
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos); };

	//�x�N�g����ݒ肷��
	virtual void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { TextureBase::SetVelocity(vel); };

	//�؂�o���摜�̏���ݒ肷��
	virtual void SetRect(const RECT& rect)override { TextureBase::SetRect(rect); };

	//�摜�̐F����ݒ肷��
	virtual void SetColor(const DirectX::SimpleMath::Color& color)override { TextureBase::SetColor(color); };
	virtual void SetColor(const DirectX::XMVECTORF32& color)override { TextureBase::SetColor(color); };

	//�摜�̉�]�p�x��ݒ肷��
	virtual void SetAngle(const float& angle)override { TextureBase::SetAngle(angle); };

	//�摜�̊g��{����ݒ肷��
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale)override { TextureBase::SetScale(scale); };

	//�摜�̏�Ԃ�ݒ肷��
	virtual void SetState(const ITextureBase::STATE& state)override { TextureBase::SetState(state); };

	//�摜�̖��O��ݒ肷��
	virtual void SetName(const std::string& name)override { TextureBase::SetName(name); };

	//�摜�̕`�揇��ݒ肷��
	virtual void SetDrawOrder(const int& order)override { TextureBase::SetDrawOrder(order); };

	//�摜�̃p�X��ݒ肷��
	virtual void SetPath(const std::wstring& path)override { TextureBase::SetPath(path); };

	//----------------�Z�b�^�[-----------------------------------------------------------//

	//----------------�Q�b�^�[------------------------------------------------------------//

	//�n���h�����擾����
	virtual const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return TextureBase::GetHandle(); };

	//���W���擾����
	virtual const DirectX::SimpleMath::Vector3& GetPosition()const override { return TextureBase::GetPosition(); };

	//�x�N�g�����擾����
	virtual const DirectX::SimpleMath::Vector3& GetVelocity()const override { return TextureBase::GetVelocity(); };

	//�؂�o���摜�̏����擾����
	virtual const RECT& GetRect()const override { return TextureBase::GetRect(); };

	//�摜�̐F�����擾����
	virtual const DirectX::SimpleMath::Color& GetColor()const override { return TextureBase::GetColor(); };

	//�摜�̉�]�p�x���擾����
	virtual const float& GetAngle()const override { return TextureBase::GetAngle(); };

	//�摜�̊g��{�����擾����
	virtual const DirectX::SimpleMath::Vector3& GetScale()const override { return TextureBase::GetScale(); };

	//�摜�̏�Ԃ��擾����
	virtual const ITextureBase::STATE& GetState()const override { return TextureBase::GetState(); };

	//�摜�̖��O���擾����
	virtual const std::string& GetName()const override { return TextureBase::GetName(); };

	//�摜�̕`�揇���擾����
	virtual const int& GetDrawOrder()const override { return TextureBase::GetDrawOrder(); };

	//�摜�̃p�X��ݒ肷��
	virtual const std::wstring& GetPath()const override { return TextureBase::GetPath(); };
	//----------------�Q�b�^�[-------------------------------------------------------------//

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	virtual void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	virtual void NonCollisionEnter(ITextureBase* obj)override;

	//���g�̓����蔻����擾����
	ICollisionBase<ITextureBase>* GetThisCollision()override { return CollisionBase::GetThisCollision(); };

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

};

