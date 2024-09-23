#pragma once
#include "Effect/Particle/Particle.h"
#include "LagState.h"
#include "TemplateState.h"
class MainParticle:public Particle,State<SCALE>
{
public:
	//1�t���[���ɃX�P�[���ɉ��Z���鑬�x
	static const DirectX::SimpleMath::Vector3 SCALESPEED;
private:
	//�`���[�W�����
	int m_ChageCount;

	//�`���[�W������E�l
	int m_LimitChage;

	//�`���[�W�����̃t���O
	bool m_ChageFinish;

	//�g��E�k�������
	int	m_ScaleCount;

	//�g��E�k��������E��
	int m_ScaleLimitCount;

	//�ǂꂾ���g�傷�邩�̌��E
	float m_ScaleLimit;

	//���݂̊g��E�k���x�N�g��
	DirectX::SimpleMath::Vector3 m_ScaleVel;
public:
	//�R���X�g���N�^
	MainParticle(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~MainParticle()override {};

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
	void SetTexture(OtobeLib::ITextureBase* texture)override { TextureBase::SetTexture(texture); };

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

	//�X�V�֐���ݒ肷��
	void SetActive(const bool& active)override { TextureBase::SetActive(active); };

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

	//�X�V�֐����擾����
	const bool& GetActive()const { return TextureBase::GetActive(); };

	//----------------�Q�b�^�[-------------------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;


public:
	//�`���[�W������E�l��ݒ肷��
	void SetChageLimit(const int& limit) { m_LimitChage = limit; }

	//����A�g��E�k�����邩
	void SetScaleLimitCount(const int& limit) { m_ScaleLimitCount = limit; }

	//�ǂꂾ���A�g��E�k�����邩
	void SetScaleLimit(const float& limit) { m_ScaleLimit = limit; }

	//�`���[�W������E�l���擾����
	const int& GetChageLimit()const { return m_LimitChage; }

	//�`���[�W�����̃t���O���擾����
	const bool& GetChageFinish()const { return m_ChageFinish; }

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const SCALE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const SCALE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const SCALE& GetPreState()const override { return State::GetPreState(); };

private:
	//���͂̃p�[�e�B�N�����ǔ����Ă���Ƃ��̏���
	void State_Tracking(const int& NoDrawNum);

	//���S�̃p�[�e�B�N�����傫���Ȃ鎞�̏���
	void State_PowerUp(const int& NoDrawNum);

	//�G�l���M�[��MAX�̎��̏���
	void State_Max();
};

