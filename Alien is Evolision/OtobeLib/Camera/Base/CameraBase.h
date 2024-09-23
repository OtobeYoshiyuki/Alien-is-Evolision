#pragma once

namespace OtobeLib
{
	//�J�����̃x�[�X�N���X
	class CameraBase
	{
	private:
		//���W
		DirectX::SimpleMath::Vector3 m_pos;

		//�s��
		DirectX::SimpleMath::Matrix m_matrix;

		//�I�u�W�F�N�g�̍X�V�̗L��
		bool						m_objectMove;
	public:
		//�R���X�g���N�^
		CameraBase();

		//�f�X�g���N�^
		virtual ~CameraBase() {}

		//������
		virtual void Initialize() {}

		//�X�V����
		virtual void Update() {}

		//�I������
		virtual void Finalize() {}

		//-------------------------------�Z�b�^�[---------------------------------//
			//���W��ݒ肷��
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }

			//�s���ݒ肷��
		virtual void SetMatrix(const DirectX::SimpleMath::Matrix& matrix) { m_matrix = matrix; }

			//�I�u�W�F�N�g�̈ړ��̗L����ݒ肷��
		virtual void SetObjectMove(const bool& move) { m_objectMove = move; }
		//-------------------------------�Z�b�^�[---------------------------------//

		//-------------------------------�Q�b�^�[---------------------------------//
			//���W��ݒ肷��
		virtual const DirectX::SimpleMath::Vector3& GetPosition() { return m_pos; }

			//�s���ݒ肷��
		virtual const DirectX::SimpleMath::Matrix& GetMatrix() { return m_matrix; }

			//�I�u�W�F�N�g�̈ړ��̗L����ݒ肷��
		virtual const bool& GetObjectMove() { return m_objectMove; }
		//-------------------------------�Q�b�^�[---------------------------------//
	};

}

