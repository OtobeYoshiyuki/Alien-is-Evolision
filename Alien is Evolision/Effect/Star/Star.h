#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "GameObject/MoveState/MoveState.h"
#include "GameObject/MoveState/AutoMoveState/AutoMoveState.h"
#include "GameObject/MoveState/JumpState/JumpState.h"
#include <list>
class Star:public OtobeLib::TextureBase
{
private:
	//AutoMoveState
	std::unique_ptr<AutoMoveState>	m_AutoMoveState;

	//JumpState
	std::unique_ptr<JumpState>		m_JumpState;

	//MoveState�̃��X�g
	std::list<MoveState*>			m_MoveStateList;

	//1�t���[���ɉ�]���鑬�x
	float m_AngleSpeed;
public:
	//�R���X�g���N�^
	Star(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~Star()override {};

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
	//----------------�Q�b�^�[-------------------------------------------------------------//

public:
	//1�t���[���ɉ�]���鑬�x��ݒ肷��
	void SetAngleSpeed(const float& speed) { m_AngleSpeed = speed; }

	//1�t���[���ɉ�]���鑬�x���擾����
	const float& GetAngleSpeed()const { return m_AngleSpeed; }

	//AutoMoveState���擾����
	std::unique_ptr<AutoMoveState>& GetAutoMoveState() { return m_AutoMoveState; }

	//JumpState���擾����
	std::unique_ptr<JumpState>& GetJumpState() { return m_JumpState; }

private:
	//�X�e�[�g�̍X�V���s��
	void UpdateState();

	//�p�x�̍X�V���s��
	void UpdateAngle();
};

