#pragma once
#include <vector>
#include "GameObject/Item/Item.h"
#include "TemplateState.h"
#include "GameObject/Item/BOUND.h"
#include "GameObject/MoveState/JumpState/JumpState.h"

class Heal :public Item,State<BOUND>
{
public:
	//1�t���[���ɉ��Z���鑬�x
	static const float	JUMP_SPEED;

	//�W�����v�̌��E�l(�ŏ�)
	static const float JUMP_LIMIT_FIRST;

	//�W�����v�̉�
	static const int JUMP_COUNT = 9;

	//�㏸����A�����܂ł̕␳�l
	static const float CORRECTION;

	//�o�E���h�W��
	static const float BOUND_POWER;

	//�����ʒu
	static const DirectX::SimpleMath::Vector3 INIT_POS;

	//�����{��
	static const DirectX::SimpleMath::Vector3 INIT_SCALE;
private:
	//�X�e�[�g�̃��X�g
	std::vector<MoveState*> m_stateList;

	//JumpState
	std::unique_ptr<JumpState> m_jumpState;

	//�񕜗�
	float						m_healPower;
public:
	//�R���X�g���N�^
	Heal(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~Heal()override {};

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
	void SetTexture(ITextureBase* texture)override { Item::SetTexture(texture); };

	//�n���h����ݒ肷��
	void SetHandle(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& handle)override { Item::SetHandle(handle); };

	//���W��ݒ肷��
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { Item::SetPosition(pos); };

	//�x�N�g����ݒ肷��
	void SetVelocity(const DirectX::SimpleMath::Vector3& vel)override { Item::SetVelocity(vel); };

	//�؂�o���摜�̏���ݒ肷��
	void SetRect(const RECT& rect)override { Item::SetRect(rect); };

	//�摜�̐F����ݒ肷��
	void SetColor(const DirectX::SimpleMath::Color& color)override { Item::SetColor(color); };

	//�摜�̉�]�p�x��ݒ肷��
	void SetAngle(const float& angle)override { Item::SetAngle(angle); };

	//�摜�̊g��{����ݒ肷��
	void SetScale(const DirectX::SimpleMath::Vector3& scale)override { Item::SetScale(scale); };

	//�摜�̏�Ԃ�ݒ肷��
	void SetState(const ITextureBase::STATE& state)override { Item::SetState(state); };

	//�摜�̖��O��ݒ肷��
	void SetName(const std::string& name)override { Item::SetName(name); };

	//�摜�̕`�揇��ݒ肷��
	void SetDrawOrder(const int& order)override { Item::SetDrawOrder(order); };

	//�摜�̃p�X��ݒ肷��
	void SetPath(const std::wstring& path)override { Item::SetPath(path); };

	//�摜�̒��S�ʒu��ݒ肷��
	void SetOrigin(const DirectX::SimpleMath::Vector2& origin)override { Item::SetOrigin(origin); };

	//�A�C�e���̎擾�ς݃t���O��ݒ肷��
	void SetItemFlag(const bool& flag)override { Item::SetItemFlag(flag); }

	//----------------�Z�b�^�[-----------------------------------------------------------//

	//----------------�Q�b�^�[------------------------------------------------------------//

	//�n���h�����擾����
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetHandle()const override { return Item::GetHandle(); };

	//���W���擾����
	const DirectX::SimpleMath::Vector3& GetPosition()const override { return Item::GetPosition(); };

	//�x�N�g�����擾����
	const DirectX::SimpleMath::Vector3& GetVelocity()const override { return Item::GetVelocity(); };

	//�؂�o���摜�̏����擾����
	const RECT& GetRect()const override { return Item::GetRect(); };

	//�摜�̐F�����擾����
	const DirectX::SimpleMath::Color& GetColor()const override { return Item::GetColor(); };

	//�摜�̉�]�p�x���擾����
	const float& GetAngle()const override { return Item::GetAngle(); };

	//�摜�̊g��{�����擾����
	const DirectX::SimpleMath::Vector3& GetScale()const override { return Item::GetScale(); };

	//�摜�̏�Ԃ��擾����
	const ITextureBase::STATE& GetState()const override { return Item::GetState(); };

	//�摜�̖��O���擾����
	const std::string& GetName()const override { return Item::GetName(); };

	//�摜�̕`�揇���擾����
	const int& GetDrawOrder()const override { return Item::GetDrawOrder(); };

	//�摜�̃p�X��ݒ肷��
	const std::wstring& GetPath()const override { return Item::GetPath(); };

	//�摜�̒��S�ʒu���擾����
	const DirectX::SimpleMath::Vector2& GetOrigin()const override { return Item::GetOrigin(); };

	//�A�C�e���̎擾�ς݃t���O���擾����
	const bool& GetItemFlag()const override { return Item::GetItemFlag(); }

	//----------------�Q�b�^�[-------------------------------------------------------------//

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

	//���g�̓����蔻����擾����
	ICollisionBase<ITextureBase>* GetThisCollision() { return Item::GetThisCollision(); };

	//�����������ǂ����̃t���O���擾����
	const bool& GetCheck()const override { return Item::GetCheck(); };

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

	//�摜�̐؂�o���ʒu����摜�̒��S�_�����߂�
	const DirectX::SimpleMath::Vector2 AskOrigin()const override { return Item::AskOrigin(); };
public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const BOUND& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const BOUND& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const BOUND& GetPreState()const override { return State::GetPreState(); };

public:
	//��������
	void OnFall(const DirectX::SimpleMath::Vector3& pos);

	//�񕜗ʂ�ݒ肷��
	void SetHealPower(const float& heal) { m_healPower = heal; }

	//�񕜗ʂ��擾����
	const float& GetHealPower()const { return m_healPower; }

private:
	//�X�e�[�g�̍X�V����
	void UpdateState();

	//State_Bound�̏���
	void State_Bound();

	//State_RESULT�̏���
	void State_Result();

	//Player�ɓ����������̏���
	void OnCollision_Player(ITextureBase* obj);

	//floor�ɓ��������Ƃ��̏���
	void OnCollision_Floor(ITextureBase* obj);

	//wall�ɓ��������Ƃ��̏���
	void OnCollision_Wall(ITextureBase* obj);
};

