#include "pch.h"
#include "Item.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

Item::Item(ITextureBase* texture):
	TextureBase(texture),//���N���X�ɏ��𑗐M����
	m_getFlag(false)//�擾�ς݃t���O
{
}

void Item::Initialize()
{
}

void Item::Update()
{
	//�摜�̍X�V����
	TextureBase::Update();
}

void Item::Render()
{
	//�摜�̕`�揈��
	TextureBase::Render();
}

void Item::Finalize()
{
}

void Item::OnCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::OnCollisionEnter(obj);
}

void Item::NonCollisionEnter(ITextureBase* obj)
{
	//�����蔻��̏���
	CollisionBase::NonCollisionEnter(obj);
}
