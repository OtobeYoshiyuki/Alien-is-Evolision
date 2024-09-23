#pragma once
#include "Effect/Particle/Particle.h"
#include "TemplateState.h"
#include "ENERGYCHAGE.h"
class NormalBullet:public Particle,State<ENERGY_CHAGE>
{
public:
	//�擾�X�R�A
	static const int BASE_SCORE = 30;
private:
	//����ւ��p�̃x�N�g��
	DirectX::SimpleMath::Vector3 m_Temp;

	//�v���C���[�̒e�𔭎˂������_�̍��W
	DirectX::SimpleMath::Vector3 m_PlayerTarget;
public:
	//�R���X�g���N�^
	NormalBullet(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~NormalBullet()override {};

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
	void SetTexture(ITextureBase* texture)override { Particle::SetTexture(texture); };

	//�n���h����ݒ肷��
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { Particle::SetHandle(handle); };

	//���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { Particle::SetPosition(pos); };

	//�x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { Particle::SetVelocity(vel); };

	//�؂�o���摜�̏���ݒ肷��
	void SetRect(const RECT& rect)override { Particle::SetRect(rect); };

	//�摜�̐F����ݒ肷��
	void SetColor(const DirectX::SimpleMath::Color& color)override { Particle::SetColor(color); };
	void SetColor(const DirectX::XMVECTORF32& color)override { Particle::SetColor(color); }

	//�摜�̉�]�p�x��ݒ肷��
	void SetAngle(const float& angle)override { Particle::SetAngle(angle); };

	//�摜�̊g��{����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { Particle::SetScale(scale); };

	//�摜�̏�Ԃ�ݒ肷��
	void SetState(const ITextureBase::STATE& state)override { Particle::SetState(state); };

	//�摜�̖��O��ݒ肷��
	void SetName(const std::string& name)override { Particle::SetName(name); };

	//�摜�̕`�揇��ݒ肷��
	void SetDrawOrder(const int& order)override { Particle::SetDrawOrder(order); };

	//�摜�̃p�X��ݒ肷��
	void SetPath(const std::wstring& path)override { Particle::SetPath(path); };

	//�摜�̌��_��ݒ肷��
	void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { Particle::SetOrigin(origin); }

	//�摜�̃G�t�F�N�g��ݒ肷��
	void SetSpriteEffect(const DirectX::SpriteEffects& effect)override { Particle::SetSpriteEffect(effect); }

	//�摜�̍X�V�֐���ݒ肷��
	void SetActive(const bool& active)override { Particle::SetActive(active); }

	//----------------------�Z�b�^�[------------------------------------------------------------//


	//----------------�Q�b�^�[------------------------------------------------------------//

	//�n���h�����擾����
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return Particle::GetHandle(); };

	//���W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition()const override { return Particle::GetPosition(); };

	//�x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetVelocity()const override { return Particle::GetVelocity(); };

	//�؂�o���摜�̏����擾����
	const RECT& GetRect()const override { return Particle::GetRect(); };

	//�摜�̐F�����擾����
	const DirectX::SimpleMath::Color& GetColor()const override { return Particle::GetColor(); };

	//�摜�̉�]�p�x���擾����
	const float& GetAngle()const override { return Particle::GetAngle(); };

	//�摜�̊g��{�����擾����
	const DirectX::SimpleMath::Vector3& GetScale()const override { return Particle::GetScale(); };

	//�摜�̏�Ԃ��擾����
	const ITextureBase::STATE& GetState()const override { return Particle::GetState(); };

	//�摜�̖��O���擾����
	const std::string& GetName()const override { return Particle::GetName(); };

	//�摜�̕`�揇���擾����
	const int& GetDrawOrder()const override { return Particle::GetDrawOrder(); };

	//�摜�̃p�X��ݒ肷��
	const std::wstring& GetPath()const override { return Particle::GetPath(); };

	//�摜�̌��_��ݒ肷��
	const DirectX::SimpleMath::Vector2& GetOrigin()const override { return Particle::GetOrigin(); }

	//�摜�̃G�t�F�N�g��ݒ肷��
	const DirectX::SpriteEffects& GetSpriteEffect()const override { return Particle::GetSpriteEffect(); }

	//�摜�̍X�V�֐���ݒ肷��
	const bool& GetActive()const override { return Particle::GetActive(); }

	//----------------�Q�b�^�[------------------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

	//���g�̓����蔻����擾����
	ICollisionBase<ITextureBase>* GetThisCollision()override { return Particle::GetThisCollision(); }

public:

	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const ENERGY_CHAGE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const ENERGY_CHAGE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const ENERGY_CHAGE& GetPreState()const override { return State::GetPreState(); };

public:

	//����ւ��p�̃x�N�g����ݒ肷��
	void SetTempVel(const DirectX::SimpleMath::Vector3& vel) { m_Temp = vel; }

	//���˂������_�̍��W��ݒ肷��
	void SetPlayerTarget(const DirectX::SimpleMath::Vector3& target) { m_PlayerTarget = target; }

	//����ւ��p�̃x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetTempVel()const { return m_Temp; }

	//���˂������_�̍��W��ݒ肷��
	const DirectX::SimpleMath::Vector3& GetPlayerTarget()const { return m_PlayerTarget; }
};

