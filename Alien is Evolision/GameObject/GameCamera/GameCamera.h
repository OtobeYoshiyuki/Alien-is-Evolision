#pragma once
#include "OtobeLib/Camera/Base/CameraBase.h"
class GameCamera:public OtobeLib::CameraBase
{
public:
	//�R���X�g���N�^
	GameCamera();

	//�f�X�g���N�^
	~GameCamera()override {};

	//������
	void Initialize()override;

	//�X�V����
	void Update()override;

	//�I������
	void Finalize()override;

	//-------------------------------�Z�b�^�[---------------------------------//
		//���W��ݒ肷��
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) { CameraBase::SetPosition(pos); }

	//�s���ݒ肷��
	virtual void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { CameraBase::SetMatrix(matrix); }

	//�I�u�W�F�N�g�̈ړ��̗L����ݒ肷��
	virtual void SetObjectMove(const bool& move) { CameraBase::SetObjectMove(move); }
	//-------------------------------�Z�b�^�[---------------------------------//

	//-------------------------------�Q�b�^�[---------------------------------//
		//���W��ݒ肷��
	virtual const DirectX::SimpleMath::Vector3& GetPosition() { return CameraBase::GetPosition(); }

	//�s���ݒ肷��
	virtual const DirectX::SimpleMath::Matrix& GetMatrix() { return CameraBase::GetMatrix(); }

	//�I�u�W�F�N�g�̈ړ��̗L����ݒ肷��
	virtual const bool& GetObjectMove() { return CameraBase::GetObjectMove(); }
	//-------------------------------�Q�b�^�[---------------------------------//
};

