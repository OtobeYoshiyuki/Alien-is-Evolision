#include "pch.h"
#include "Collision.h"
#include "OtobeLib/FrameWork/Input/Input.h"
#include "Game.h"
#include <Windows.h>

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;

Collision::Collision()
{
}


Collision::~Collision()
{ 
}

bool Collision::HitCheck_AABB(ITextureBase* obj1, ITextureBase* obj2)
{
	//�n�_�I�_�̎擾
	RECT rect1 = obj1->GetRect();
	RECT rect2 = obj2->GetRect();

	//�摜�̔��a�̎擾
	Vector3 Size1 = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f);
	Vector3 Size2 = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f);

	if (fabsf(obj1->GetPosition().x - obj2->GetPosition().x) > Size1.x / 2 + Size2.x / 2)return false;
	if (fabsf(obj1->GetPosition().y - obj2->GetPosition().y) > Size1.y / 2 + Size2.y / 2)return false;
	if (fabsf(obj1->GetPosition().z - obj2->GetPosition().z) > Size1.z / 2 + Size2.z / 2)return false;

	return true;
}

bool Collision::HitCheck_OBB(ITextureBase* obj1, ITextureBase* obj2)
{
	if (!obj1 || !obj2)return false;

	ICollisionBase<ITextureBase>* col1 = obj1->GetThisCollision();
	ICollisionBase<ITextureBase>* col2 = obj2->GetThisCollision();

	FLOAT ra, rb;
	FLOAT r[3][3], absR[3][3];
	Matrix rot_a, rot_b;
	Vector3 size_a, size_b;

	OBB a, b;

	if (obj1->GetName() == "SickChar")
	{
		rot_a = Matrix::Identity;
		rot_b = Matrix::CreateRotationZ(XMConvertToRadians(obj2->GetAngle()));
	}
	else
	{
		rot_a = Matrix::CreateRotationZ(XMConvertToRadians(obj1->GetAngle()));
		rot_b = Matrix::CreateRotationZ(XMConvertToRadians(obj2->GetAngle()));
	}

	RECT rect1 = obj1->GetRect();
	RECT rect2 = obj2->GetRect();

	//�摜�̔��a�̎擾
	size_a = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f);
	size_b = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f);

	size_a *= 0.5f;
	size_b *= 0.5f;

	Vector3 origin1 = obj1->GetOrigin();
	Vector3 origin2 = obj2->GetOrigin();

	Vector3 scale1 = obj1->GetScale();
	Vector3 scale2 = obj2->GetScale();

	a.center = obj1->GetPosition() + (size_a - origin1);
	a.e[0] = Vector3(rot_a._11, rot_a._12, rot_a._13);
	a.e[1] = Vector3(rot_a._21, rot_a._22, rot_a._23);
	a.e[2] = Vector3(rot_a._31, rot_a._32, rot_a._33);
	a.r[0] = size_a.x * scale1.x;
	a.r[1] = size_a.y * scale1.y;
	a.r[2] = size_a.z * scale2.z;

	b.center = obj2->GetPosition() + (size_b - origin2);
	b.e[0] = Vector3(rot_b._11, rot_b._12, rot_b._13);
	b.e[1] = Vector3(rot_b._21, rot_b._22, rot_b._23);
	b.e[2] = Vector3(rot_b._31, rot_b._32, rot_b._33);
	b.r[0] = size_b.x * scale2.x;
	b.r[1] = size_b.y * scale2.y;
	b.r[2] = size_b.z * scale2.z;

	//	�Q�̂n�a�a�̒��S�����ԃx�N�g����a�̍��W�n�ɕϊ�	
	Vector3 vec = b.center - a.center;
	vec = Vector3(vec.Dot(a.e[0]),
		vec.Dot(a.e[1]),
		vec.Dot(a.e[2]));
	//	a�̍��W�n�̒���b��\����]�s����쐬
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			r[i][j] = a.e[i].Dot(b.e[j]);
			//	��Βl���Ɍv�Z���Ă���
			absR[i][j] = fabsf(r[i][j]) + FLT_EPSILON;
		}
	}

	float vel[3] = { vec.x,vec.y,vec.z };

	//	a�̍��W�n�̊e���֓��e���Ĕ���
	for (int i = 0; i < 3; i++) {
		ra = a.r[i];
		rb = b.r[0] * absR[i][0] + b.r[1] * absR[i][1] + b.r[2] * absR[i][2];
		if (fabsf(vel[i]) > ra + rb) return FALSE;
	}

	//	b�̍��W�n�̊e���֓��e���Ĕ���
	for (int i = 0; i < 3; i++) {
		ra = a.r[0] * absR[0][i] + a.r[1] * absR[1][i] + a.r[2] * absR[2][i];
		rb = b.r[i];
		if (fabsf(vec.x * r[0][i] + vec.y * r[1][i] + vec.z * r[2][i]) > ra + rb) return FALSE;
	}

	//	a�̂w����b�̂w���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[1] * absR[2][0] + a.r[2] * absR[1][0];
	rb = b.r[1] * absR[0][2] + b.r[2] * absR[0][1];
	if (fabsf(vec.z * r[1][0] - vec.y * r[2][0]) > ra + rb) return FALSE;

	//	a�̂w����b�̂x���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[1] * absR[2][1] + a.r[2] * absR[1][1];
	rb = b.r[0] * absR[0][2] + b.r[2] * absR[0][0];
	if (fabsf(vec.z * r[1][1] - vec.y * r[2][1]) > ra + rb) return FALSE;

	//	a�̂w����b�̂y���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[1] * absR[2][2] + a.r[2] * absR[1][2];
	rb = b.r[0] * absR[0][2] + b.r[1] * absR[0][0];
	if (fabsf(vec.z * r[1][2] - vec.y * r[2][2]) > ra + rb) return FALSE;


	//	a�̂x����b�̂w���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[0] * absR[2][0] + a.r[2] * absR[0][0];
	rb = b.r[1] * absR[1][2] + b.r[2] * absR[1][1];
	if (fabsf(vec.x * r[2][0] - vec.z * r[0][0]) > ra + rb) return FALSE;

	//	a�̂x����b�̂x���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[0] * absR[2][1] + a.r[2] * absR[0][1];
	rb = b.r[0] * absR[1][2] + b.r[2] * absR[1][0];
	if (fabsf(vec.x * r[2][1] - vec.z * r[0][1]) > ra + rb) return FALSE;

	//	a�̂x����b�̂y���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[0] * absR[2][2] + a.r[2] * absR[0][2];
	rb = b.r[0] * absR[1][1] + b.r[1] * absR[1][0];
	if (fabsf(vec.x * r[2][2] - vec.z * r[0][2]) > ra + rb) return FALSE;


	//	a�̂y����b�̂w���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[0] * absR[1][0] + a.r[1] * absR[0][0];
	rb = b.r[1] * absR[2][2] + b.r[2] * absR[2][1];
	if (fabsf(vec.y * r[0][0] - vec.x * r[1][0]) > ra + rb) return FALSE;

	//	a�̂y����b�̂x���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[0] * absR[1][1] + a.r[1] * absR[0][1];
	rb = b.r[0] * absR[2][2] + b.r[2] * absR[2][0];
	if (fabsf(vec.y * r[0][1] - vec.x * r[1][1]) > ra + rb) return FALSE;

	//	a�̂y����b�̂y���̊O�σx�N�g���֓��e���Ĕ���
	ra = a.r[0] * absR[1][2] + a.r[1] * absR[0][2];
	rb = b.r[0] * absR[2][1] + b.r[1] * absR[2][0];
	if (fabsf(vec.y * r[0][2] - vec.x * r[1][2]) > ra + rb) return FALSE;


	return TRUE;
}

bool Collision::HitCheck_Circle(ITextureBase* obj1, ITextureBase* obj2)
{
	// ���S�Ԃ̋����̕������v�Z

	Vector3 d = obj1->GetPosition() - obj2->GetPosition();

	float dist2 = d.Dot(d);

	// �������������������������a�̍��v�����������ꍇ�ɋ��͌������Ă���
	//�n�_�I�_�̎擾
	RECT rect1 = obj1->GetRect();
	RECT rect2 = obj2->GetRect();

	//�摜�̔��a�̎擾
	Vector3 Sizehalf1 = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f);
	Vector3 Sizehalf2 = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f);

	Vector3 radiusSum = Sizehalf1 + Sizehalf2;

	if (dist2 <= radiusSum.x * radiusSum.x)return true;

	return false;
}

bool Collision::HitCheck_Circle(const Vector3& pos1, const Vector3& pos2, const float& radius)
{
	// ���S�Ԃ̋����̕������v�Z
	Vector3 d = pos1 - pos2;

	float dist2 = d.Dot(d);

	// �������������������������a�̍��v�����������ꍇ�ɋ��͌������Ă���
	if (dist2 <= radius * radius)return true;

	return false;
}

bool Collision::HitCheck_Mouse_To_Texture(ITextureBase* tex)
{
	//�}�E�X�̏�Ԃ��擾
	const auto& Mouse_State = Input::GetInstance()->GetMouse_State();

	//�e�N�X�`���̊g��{�����擾
	float scale = tex->GetScale().x + tex->GetScale().y + tex->GetScale().z;

	//�}�E�X�̍��W���擾
	Vector2 Mouse_Pos = Vector2(static_cast<float>(Mouse_State.x), static_cast<float>(Mouse_State.y));
	
	//�ÓI�f�[�^�̎擾
	StaticData* staticData = StaticData::GetInstance();

	//���j�^�[�̉�ʃT�C�Y���擾����
	int dispx = GetSystemMetrics(SM_CXSCREEN);
	int dispy = GetSystemMetrics(SM_CYSCREEN);

	//�N�����̃Q�[���̃E�B���h�E�T�C�Y
	Vector2 GameWindow = Vector2(Game::MAX_WIDTH, Game::MAX_HEIGHT);

	//��ʂ̃A�X�y�N�g��(�ʏ탂�[�h�̎��͂��̂܂�)
	Vector2 aspect = Vector2(1.0f, 1.0f);

	//�t���X�N���[���̂Ƃ��}�E�X���W���t���X�N���[���ɕϊ�����
	if (staticData->Get_BoolData("�t���X�N���[��"))
	{
		aspect = Vector2(GameWindow.x / dispx,GameWindow.y / dispy);
	}

	//�}�E�X�̍��W���A�X�y�N�g��ɍ��킹�ĕϊ�����
	Mouse_Pos *= aspect;
	
	if (Mouse_Pos.x >= tex->GetPosition().x && Mouse_Pos.x <= tex->GetPosition().x + ((tex->GetRect().right - tex->GetRect().left) * scale / 3) &&
		Mouse_Pos.y >= tex->GetPosition().y && Mouse_Pos.y <= tex->GetPosition().y + ((tex->GetRect().bottom - tex->GetRect().top) * scale / 3))
		return true;

	return false;
}
