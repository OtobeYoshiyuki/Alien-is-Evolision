#include "pch.h"
#include "TechniqueGage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TechniqueGage::TechniqueGage(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_Max(NULL)//�摜�̏c�����̍ő�l
{
}

void TechniqueGage::Initialize()
{
}

void TechniqueGage::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();
}

void TechniqueGage::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void TechniqueGage::Finalize()
{
}
