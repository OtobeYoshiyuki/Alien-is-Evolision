#include "pch.h"
#include "DeathblowGage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

DeathblowGage::DeathblowGage(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_Max(NULL)//�摜�̏c�����̕��̍ő�l
{
}

void DeathblowGage::Initialize()
{
}

void DeathblowGage::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();
}

void DeathblowGage::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void DeathblowGage::Finalize()
{
}
