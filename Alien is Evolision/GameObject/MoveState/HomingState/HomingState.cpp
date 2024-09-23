#include "pch.h"
#include "HomingState.h"
#include "Utils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//�R���X�g���N�^
HomingState::HomingState():
	m_length(Vector3::Zero),//����
	m_angle(0.0f),//�p�x(�f�O���[)
	m_start(false),//�J�n�t���O
	m_finish(false),//�I���t���O
	m_scalar(Vector3(1.0f)),//�X�J���l
	m_maxVel(Vector3::Zero),//�x�N�g���̍ő�l
	m_minVel(Vector3::Zero),//�x�N�g���̍ŏ��l
	m_dirVel(Vector3::Zero)//�x�N�g���̌���
{
}

//�X�V����
const Vector3 HomingState::Update()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//���s�֐���false�̎��́A�������Ȃ�
	if (!this->GetExecute())return vel;

	//�J�n�t���O���I��
	if (m_start)
	{
		//Y�x�N�g�����ő�ȏ�̎�
		if (m_length.y <= m_maxVel.y)
		{
			//�t���O��؂�
			m_start = false;

			//�t���O�𗧂Ă�
			m_finish = true;
		}
		else
		{
			//�x�N�g����������
			vel = m_dirVel;

			//�x�N�g����␳����
			vel *= m_scalar;

			//�x�N�g�������Z����
			m_length += vel;
		}
	}

	//�I���t���O���I��
	if (m_finish)
	{
		//360����]���邽�тɁA������␳����
		if ((int)m_angle % 360 == 0 && (int)m_angle != NULL)
		{
			//������Y�x�N�g����0���傫���Ƃ�
			if (m_dirVel.y > 0.0f)
			{
				//�����̃x�N�g����␳����
				m_dirVel.y -= 0.01f;
			}
			else
			{
				//�������Ƃ��́A�I���t���O��؂�
				m_finish = false;
			}
		}

		//�p�x���X�V����
		m_angle += 36.0f;

		//��]�s��𐶐�
		Matrix rot = Matrix::CreateRotationZ(XMConvertToRadians(m_angle));

		//�x�N�g������]������
		vel = Vector3::Transform(m_dirVel, rot);

		//�x�N�g����␳����
		vel *= m_scalar;

		//�x�N�g�������Z����
		m_length += vel;

	}

	//�x�N�g����Ԃ�
	return vel;
}

void HomingState::InitState(const DirectX::SimpleMath::Vector3& dir)
{
	//����������������
	m_length = Vector3::Zero;

	//�t���O������������
	m_start = false;
	m_finish = false;

	//�p�x������������
	m_angle = 0.0f;

	//�x�N�g���̌���������������
	m_dirVel = dir;
}
