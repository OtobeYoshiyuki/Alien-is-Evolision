#pragma once
enum class SELECT :int
{
	NONE = -1,//�������Ȃ�
	WAITKEY,//�L�[���͑҂�
	CALCING,//�v�Z��
	CHANGE_CUTIN,//Scene�̐؂�ւ�
	CHANGE_AND_DELETE,//Scene�̐؂�ւ��ƍ폜
};