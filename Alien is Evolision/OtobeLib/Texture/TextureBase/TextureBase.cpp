#include "pch.h"
#include "TextureBase.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/Collision/Collision.h"
#include "Utils.h"
#include <locale>
#include <codecvt>
#include <string>

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

//コンストラクタ(デフォルト)
TextureBase::TextureBase() :m_Handle(nullptr), m_Position(Vector3::Zero), m_Velocity(Vector3::Zero), m_Rect(RECT{0,0,0,0}),
m_Color(Colors::White),m_Angle(0.0f),m_Scale(Vector3(1.0f,1.0f,1.0f)),m_State(STATE::DRAW),
m_DrawOrder(NULL),m_Path(L""),m_IsVal(false),m_Origin(Vector2::Zero),m_Effect(SpriteEffects::SpriteEffects_None),
m_Active(false),m_LayerDenpth(0.0f)
{
}

//コンストラクタ(上書き)
TextureBase::TextureBase(ITextureBase* texture)
{
	//テクスチャの情報を上書きする
	this->SetTexture(texture);
}

//デストラクタ
TextureBase::~TextureBase()
{
}

void TextureBase::Initialize()
{
}

void TextureBase::Update()
{
	//削除リクエストがあったら、何もしない
	if (this->GetIsVal())return;

	//点滅の更新
	Blinking::Update();

	//等速直線運動
	m_Position += m_Velocity;
}

void TextureBase::Render()
{
	//削除リクエストがあったら、何もしない
	if (this->GetIsVal())return;

	//テクスチャハンドルが入ってなかったら何もしない
	if (this->GetHandle() == nullptr)return;

	//点滅がoffのときは何もしない
	if (!Blinking::NowBlinking())return;

	//描画がoffのときは何もしない
	if (this->GetState() != STATE::DRAW)return;
	
	//描画
	this->DrawTexture();
}

void TextureBase::Finalize()
{
}

void TextureBase::SetTexture(ITextureBase* texture)
{
	//テクスチャハンドルを設定する
	this->SetHandle(texture->GetHandle());

	//座標を設定する
	this->SetPosition(texture->GetPosition());

	//ベクトルを設定する
	this->SetVelocity(texture->GetVelocity());

	//始点及び終点を設定する
	this->SetRect(texture->GetRect());

	//色を設定する
	this->SetColor(texture->GetColor());

	//回転を設定する
	this->SetAngle(texture->GetAngle());

	//拡大倍率を設定する
	this->SetScale(texture->GetScale());

	//名前を設定する
	this->SetName(texture->GetName());

	//状態を設定する
	this->SetState(texture->GetState());

	//描画順を設定する
	this->SetDrawOrder(texture->GetDrawOrder());

	//パスを設定する
	this->SetPath(texture->GetPath());

	//削除フラグを設定する
	this->SetIsVal(false);

	//画像の中心位置を設定する
	this->SetOrigin(Vector2::Zero);

	//SpriteEffectを設定する
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);

	//更新関数を設定する
	this->SetActive(false);

	//描画順を設定する（リストに影響されない）
	this->SetLayerDenpth(0.0f);
}

const Vector2 TextureBase::AskOrigin() const
{
	//切り取り位置を取得する
	RECT rect = this->GetRect();

	//切り出し位置から画像の中心位置を求める
	Vector2 origin = Vector2((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

	// TODO: return ステートメントをここに挿入します
	return origin;
}

void TextureBase::Extrude(ITextureBase* texture)
{
	this->SetPosition(this->GetPosition() - texture->GetVelocity());
}

void TextureBase::ExtrudeX(ITextureBase* texture)
{
	this->SetPosition(this->GetPosition() - Vector3(texture->GetVelocity().x, 0.0f, 0.0f));
}

void TextureBase::ExtrudeY(ITextureBase* texture)
{
	this->SetPosition(this->GetPosition() - Vector3(0.0f, texture->GetVelocity().y, 0.0f));
}

void TextureBase::ChangeSpriteEffect()
{
	if (m_Effect == SpriteEffects::SpriteEffects_None)
	{
		m_Effect = SpriteEffects::SpriteEffects_FlipHorizontally;
		return;
	}

	if (m_Effect == SpriteEffects::SpriteEffects_FlipHorizontally)
	{
		m_Effect = SpriteEffects::SpriteEffects_None;
	}
}

void TextureBase::PositionCorrection(ITextureBase* texture)
{
	//送信されてきた相手オブジェクトの座標で、補正する
	//始点終点の取得
	RECT rect1 = this->GetRect();
	RECT rect2 = texture->GetRect();

	//画像の半径の取得
	Vector3 size1 = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f) * this->GetScale();
	Vector3 size2 = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f) * texture->GetScale();

	Vector3 halfsize1 = size1 * 0.5f;
	Vector3 halfsize2 = size2 * 0.5f;

	//それぞれの座標を求める（座標は画像の中心で統一する）
	Vector3 pos1 = this->GetPosition() + halfsize1 - this->GetOrigin();
	Vector3 pos2 = texture->GetPosition() + halfsize2 - texture->GetOrigin();

	Vector3 answer = (Length(pos1, pos2)) - (halfsize1 + halfsize2);

	Vector3 vel = texture->GetVelocity();
	vel.Normalize();

	answer *= vel;

	this->SetPosition(this->GetPosition() - answer);
}

const int TextureBase::DirectX()
{
	//右側を向いている
	if (this->GetSpriteEffect() == SpriteEffects::SpriteEffects_None)
	{
		return 1;
	}

	//左側を向いている
	if (this->GetSpriteEffect() == SpriteEffects::SpriteEffects_FlipHorizontally)
	{
		return -1;
	}

	return 0;
}

void TextureBase::DrawTexture()
{
	//描画系の情報
	Graphics* Graph = Graphics::GetInstance();
	SpriteBatch* Sprite = Graph->GetSpriteBatch();
	CommonStates* State = Graph->GetCommonStates();

	//描画
	Sprite->Draw(this->GetHandle().Get(), this->GetPosition(), &this->GetRect(), this->GetColor(),
		XMConvertToRadians(this->GetAngle()), this->GetOrigin(), this->GetScale(),this->GetSpriteEffect());
}

const std::vector<TextureBase> TextureBase::Cast(const std::vector<std::vector<std::string>>& input)
{
	//Textureのvectorを宣言する
	std::vector<TextureBase> texture;

	//vectorのサイズは読み込んだデータの縦のサイズ－1とする（1行目のデータはフィルタ－のため）
	texture.resize(input.size() - 1);

	//intのvectorのvectorを宣言する（2次元配列:読み込んだデータがstd::stringなため）
	std::vector<std::vector<int>>texture_data;

	//vectorのサイズは読み込んだデータの縦のサイズ－1とする（1行目のデータはフィルタ－のため）
	texture_data.resize(input.size() - 1);

	//std::stringのvectorを宣言する(そのまま入れようとすると、配列情報にずれが生じるため)
	std::vector<std::string> tag;

	//std::wstringのvectorを宣言する(そのまま入れようとすると、配列情報にずれが生じるため)
	std::vector<std::wstring> path;

	//読み込んだデータは2行目からデータとして扱う（1行目のデータはフィルタ－のため）
	for (size_t i = 0; i < input.size() - 1; i++)
	{
		for (size_t j = 0; j < input[i].size(); j++)
		{
			if (j == 0)tag.push_back(input[i + 1][j]);
			else if (j == input[i].size() - 1)path.push_back(TextureBase::Convert(input[i + 1][j]));
			else texture_data[i].push_back(std::stoi(input[i + 1][j]));
		}
	}

	//読み込んだデータを変換する
	for (size_t i = 0; i < texture.size(); i++)
	{
		texture[i].SetPosition(Vector3(static_cast<float>(texture_data[i][0]), static_cast<float>(texture_data[i][1]), static_cast<float>(texture_data[i][2])));
		texture[i].SetVelocity(Vector3(static_cast<float>(texture_data[i][3]), static_cast<float>(texture_data[i][4]), static_cast<float>(texture_data[i][5])));
		texture[i].SetRect(RECT{ static_cast<long> (texture_data[i][6]),static_cast<long> (texture_data[i][7]),static_cast<long> (texture_data[i][8]),static_cast<long> (texture_data[i][9]) });
		texture[i].SetColor(Color{ static_cast<float>(texture_data[i][10]),static_cast<float>(texture_data[i][11]), static_cast<float>(texture_data[i][12]), static_cast<float>(texture_data[i][13]) });
		texture[i].SetAngle(static_cast<float>(texture_data[i][14]));
		texture[i].SetScale(Vector3(static_cast<float>(texture_data[i][15]), static_cast<float>(texture_data[i][16]), static_cast<float>(texture_data[i][17])));
		texture[i].SetState(static_cast<STATE>(texture_data[i][18]));
		texture[i].SetName(tag[i]);
		texture[i].SetPath(path[i]);
		texture[i].SetSpriteEffect(SpriteEffects::SpriteEffects_None);
		texture[i].SetLayerDenpth(0.0f);
	}

	//読み込んだデータを返す
	return texture;
}

//std::stringからstd::wstringへの変換
const std::wstring TextureBase::Convert(const std::string byteString)
{
	// 文字数
	size_t length = byteString.size();
	// 変換後文字数
	size_t convLength;
	// 仮の受け皿を用意
	wchar_t* wc = (wchar_t*)malloc(sizeof(wchar_t) * (length + 2));
	// 変換
	mbstowcs_s(&convLength, wc, length + 1, byteString.c_str(), _TRUNCATE);
	// 返り値へ保存
	std::wstring str(wc);
	// 仮の受け皿を破棄
	free(wc);

	return str;
}
