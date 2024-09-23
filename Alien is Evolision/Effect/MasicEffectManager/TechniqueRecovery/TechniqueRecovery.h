#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "TemplateState.h"
#include "GameObject/TargetScrollCamera/TARGET_STATE.h"
#include "GameObject/MoveState/TargetState/TrackingState.h"
#include "GameObject/MoveState/HomingState/HomingState.h"
#include "OtobeLib/FrameWork/Owner/Owner.h"
#include <vector>

class TechniqueRecovery :public OtobeLib::TextureBase, OtobeLib::CollisionBase<OtobeLib::ITextureBase>,
	State<TARGET_STATE>, OtobeLib::Owner<OtobeLib::ITextureBase>
{
private:
	//�X�e�[�g�̃R���e�i
	std::vector<MoveState*> m_stateList;

	//�ǐ՗p�̃X�e�[�g
	std::unique_ptr<TrackingState> m_tracking;

	//�z�[�~���O�p�̃X�e�[�g
	std::unique_ptr<HomingState>	m_homing;

	//�o�ߎ���
	float					m_timer;

	//�G�����ł��Ă���A�����o���܂ł̑҂�����
	float					m_waitTimer;

	//�񕜂�����l
	int					m_masicPower;
public:
	//�R���X�g���N�^
	TechniqueRecovery(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~TechniqueRecovery()override {};

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

	//���L�҂̃|�C���^��ݒ肷��
	void SetOwner(ITextureBase* ownew)override { Owner::SetOwner(ownew); }

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

	//���L�҂̃|�C���^���擾����
	ITextureBase* GetOwner() { return Owner::GetOwner(); }

	//----------------�Q�b�^�[-------------------------------------------------------------//

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

	//���g�̓����蔻����擾����
	ICollisionBase<ITextureBase>* GetThisCollision() { return CollisionBase::GetThisCollision(); };

	//�����������ǂ����̃t���O���擾����
	const bool& GetCheck()const override { return CollisionBase::GetCheck(); };

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const TARGET_STATE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const TARGET_STATE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const TARGET_STATE& GetPreState()const override { return State::GetPreState(); };

public:
	//�ǐ՗p�̃X�e�[�g���擾����
	TrackingState* GetTrackingState() { return m_tracking.get(); }

	//�z�[�~���O�p�̃X�e�[�g���擾����
	HomingState* GetHomingState() { return m_homing.get(); }

	//�҂����Ԃ�ݒ肷��
	void SetWaitTimer(const float& wait) { m_waitTimer = wait; }

	//�񕜗ʂ�ݒ肷��
	void SetHeal(const int& heal) { m_masicPower = heal; }

private:
	//State�̍X�V����
	void UpdateState();

	//State_Wait�̏���
	void State_Wait();

	//State_Tracking�̏���
	void State_Tracking();

	//�v���C���[�Ɠ����������̏���
	void OnCollision_Player(OtobeLib::ITextureBase* player);
};


