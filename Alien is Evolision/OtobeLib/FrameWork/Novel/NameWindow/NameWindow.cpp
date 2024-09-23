#include "pch.h"
#include "NameWindow.h"
#include "GameContext.h"
#include "OtobeLib/FrameWork/Novel/Novel.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

NameWindow::NameWindow(ITextureBase* texture):
	TextureBase(texture)//基底クラスに情報を送信する
{
}

void NameWindow::Initialize()
{
}

void NameWindow::Update()
{
	//所有者のポインタを取得
	Novel* novel = this->GetOwner();
	TextWindow* textWindow = novel->GetTextWindow();

	//テキストが表示されていないときは何もしない
	if (!textWindow->GetDrawCheck())return;

	//画像の更新処理
	TextureBase::Update();
}

void NameWindow::Render()
{
	Novel* novel = this->GetOwner();
	TextWindow* textWindow = novel->GetTextWindow();

	//テキストが表示されていないときは何もしない
	if (!textWindow->GetDrawCheck())return;

	//画像の描画処理
	TextureBase::Render();
}

void NameWindow::Finalize()
{
}
