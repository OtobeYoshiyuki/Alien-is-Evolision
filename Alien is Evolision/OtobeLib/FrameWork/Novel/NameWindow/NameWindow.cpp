#include "pch.h"
#include "NameWindow.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NameWindow::NameWindow(ITextureBase* texture):
	TextureBase(texture)//���N���X�ɏ��𑗐M����
{
}

void NameWindow::Initialize()
{
}

void NameWindow::Update()
{
	//���L�҂̃|�C���^���擾
	Novel* novel = this->GetOwner();
	TextWindow* textWindow = novel->GetTextWindow();

	//�e�L�X�g���\������Ă��Ȃ��Ƃ��͉������Ȃ�
	if (!textWindow->GetDrawCheck())return;

	//�摜�̍X�V����
	TextureBase::Update();
}

void NameWindow::Render()
{
	Novel* novel = this->GetOwner();
	TextWindow* textWindow = novel->GetTextWindow();

	//�e�L�X�g���\������Ă��Ȃ��Ƃ��͉������Ȃ�
	if (!textWindow->GetDrawCheck())return;

	//�摜�̕`�揈��
	TextureBase::Render();
}

void NameWindow::Finalize()
{
}
