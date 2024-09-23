#include "pch.h"
#include "TrackingState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TrackingState::TrackingState():
	m_target(nullptr),//�ǐՑΏ�
	m_this(nullptr),//�������g�̍��W
	m_correction(Vector3::Zero)//�x�N�g���̕␳�l
{
}

const Vector3 TrackingState::Update()
{
	//�x�N�g���̏�����
	Vector3 vel = Vector3::Zero;

	//���s�֐���false�̂Ƃ��͉������Ȃ�
	if (!this->GetExecute())return vel;

	//�ǐՑΏۂ��ݒ肳��Ă��Ȃ���΁A�������Ȃ�
	if (!m_target || !m_this)return vel;

	//�␳�l���ݒ肳��Ă��Ȃ���΁A�������Ȃ�
	if (m_correction == Vector3::Zero)return vel;

	//���g�ƑΏۂ̋��������߂�
	Vector3 len = *m_target - *m_this;

	//�����𐳋K������
	len.Normalize();

	//�x�N�g����␳����
	len *= m_correction;

	//�x�N�g����n��
	vel = len;

	//�x�N�g����Ԃ�
	return vel;
}
