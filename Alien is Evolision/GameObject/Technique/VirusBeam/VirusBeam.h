#pragma once
#include "OtobeLib/Texture/TextureBase/TextureBase.h"
#include "OtobeLib/FrameWork/Collision/CollisionTemplate/CollisionBase.h"
#include "GameObject/Technique/VirusBeam/MainParticle/MainParticle.h"
#include "GameObject/Technique/VirusBeam/TrackingParticle/TrackingParticle.h"
#include "TemplateState.h"
#include "ENERGYCHAGE.h"
#include <list>
#include <vector>
class VirusBeam :public OtobeLib::TextureBase,OtobeLib::CollisionBase<OtobeLib::ITextureBase>,State<ENERGY_CHAGE>
{
public:
	//���͂̃p�[�e�B�N���̐�
	static const int OTHER_PARTICLE_NUM = 10;

	//�Q�[�W�̉񕜗�(�R�}���h�Z)
	static const int TECHNIQUE_HEAL_COMMAND = 25;

	//�Q�[�W�̉񕜗�(�K�E�Z)
	static const int TECHNIQUE_HEAL_SPECIAL = 50;

	//�擾�X�R�A
	static const int BASE_SCORE = 250;

	//�R�}���h�Z�̃Q�[�W�̏����
	static const int GAGE_COST = 100;
private:
	//�r�[���̒��S�̃p�[�e�B�N��
	MainParticle* m_MainParticle;
	//���S�ɏW�܂��Ă���p�[�e�B�N��
	std::vector<TrackingParticle*> m_OtherParticleList;
	//�S�Ẵp�[�e�B�N��
	std::vector<Particle*> m_AllParticleList;
	//�`�悷�鎞��
	int					m_DrawTimeLimit;
	//�`�悷��X�̏I�_
	int					m_DrawFinal;
	//�`�悷����E�l
	int					m_DrawLimit;
	//�t���[���J�E���g
	int					m_FrameCount;
	//�r�[���̍X�V���x
	int					m_BeamSpeed;
	//ID�̊Ǘ��N���X
	std::vector<int>	m_IdManager;
public:
	//�R���X�g���N�^
	VirusBeam(OtobeLib::ITextureBase* texture);

	//�f�X�g���N�^
	~VirusBeam()override {};

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
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)override { TextureBase::SetPosition(pos);};

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

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//

	//���̃I�u�W�F�N�g�Ɠ����������ɌĂ΂��֐�
	void OnCollisionEnter(ITextureBase* obj)override;

	//���̃I�u�W�F�N�g�Ɠ������Ă��Ȃ��Ƃ��ɌĂ΂��֐�
	void NonCollisionEnter(ITextureBase* obj)override;

	//���g�̓����蔻����擾����
	ICollisionBase<ITextureBase>* GetThisCollision() { return CollisionBase::GetThisCollision(); };

	//---------------------------�����蔻��p�̊֐�---------------------------------------------------//


	//�v���C���[�Ɠ����s�����Ƃ�
	void WithPlayer(OtobeLib::ITextureBase* texture);

public:
	//�r�[���̒��S�̃p�[�e�B�N�����擾����
	MainParticle* GetMainParticle() { return m_MainParticle; }

	//���S�ɏW�܂��Ă���p�[�e�B�N�����擾����
	std::vector<TrackingParticle*>& GetParticleList() { return m_OtherParticleList; }

	//�S�Ẵp�[�e�B�N�����擾����
	std::vector<Particle*>& GetAllParticleList() { return m_AllParticleList; }

	//�`��̌��E�l��ݒ肷��
	void SetDrawLimit(const int& limit) { m_DrawLimit = limit; }

	//�I�_��ݒ肷��
	void SetDrawFinal(const int& Final) { m_DrawFinal = Final; }

	//�`�悷�鎞�Ԃ�ݒ肷��
	void SetDrawTimeLimit(const int& limit) { m_DrawTimeLimit = limit; }

	//�r�[���̃X�s�[�h��ݒ肷��
	void SetBeamSpeed(const int& speed) { m_BeamSpeed = speed; }

	//�r�[���̏�����������
	void InitBeam();

	//�p�[�e�B�N���̍��W�̐ݒ������
	void SetParticlesPos(const DirectX::SimpleMath::Vector3& pos)
	{
		//���S�̃p�[�e�B�N���̍��W��ݒ肷��
		m_MainParticle->SetPosition(pos);

		//�p�x������������
		float angle = 0.0f;

		//�Z�o�����x�N�g������A���W��ݒ肷��
		for (TrackingParticle*& particle : m_OtherParticleList)
		{
			//�x�N�g��������������
			DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(0.0f, -50.0f, 0.0f);

			//�x�N�g�����X�V
			this->AskCircleVectol(vel, angle);

			//���W�����S�̃p�[�e�B�N���̎��͂ɗ���悤�ɂ���
			particle->SetPosition(m_MainParticle->GetPosition() + vel);
		}
	}

public:
	//���݂̏�Ԃ�ݒ肷��
	void SetNowState(const ENERGY_CHAGE& now)override { State::SetNowState(now); };

	//���݂̏�Ԃ��擾����
	const ENERGY_CHAGE& GetNowState()const override { return State::GetNowState(); };

	//�ЂƂO�̏�Ԃ��擾����
	const ENERGY_CHAGE& GetPreState()const override { return State::GetPreState(); };

private:
	//�r�[���p�̃p�[�e�B�N���̍X�V����
	void UpdateParticle();

	//���S����~��ɂȂ�悤�Ƀx�N�g�����Z�o����
	static void AskCircleVectol(DirectX::SimpleMath::Vector3& vel,float& angle);
};

