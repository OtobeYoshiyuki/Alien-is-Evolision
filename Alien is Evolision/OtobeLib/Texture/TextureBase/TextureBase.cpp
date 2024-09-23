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

//�R���X�g���N�^(�f�t�H���g)
TextureBase::TextureBase() :m_Handle(nullptr), m_Position(Vector3::Zero), m_Velocity(Vector3::Zero), m_Rect(RECT{0,0,0,0}),
m_Color(Colors::White),m_Angle(0.0f),m_Scale(Vector3(1.0f,1.0f,1.0f)),m_State(STATE::DRAW),
m_DrawOrder(NULL),m_Path(L""),m_IsVal(false),m_Origin(Vector2::Zero),m_Effect(SpriteEffects::SpriteEffects_None),
m_Active(false),m_LayerDenpth(0.0f)
{
}

//�R���X�g���N�^(�㏑��)
TextureBase::TextureBase(ITextureBase* texture)
{
	//�e�N�X�`���̏����㏑������
	this->SetTexture(texture);
}

//�f�X�g���N�^
TextureBase::~TextureBase()
{
}

void TextureBase::Initialize()
{
}

void TextureBase::Update()
{
	//�폜���N�G�X�g����������A�������Ȃ�
	if (this->GetIsVal())return;

	//�_�ł̍X�V
	Blinking::Update();

	//���������^��
	m_Position += m_Velocity;
}

void TextureBase::Render()
{
	//�폜���N�G�X�g����������A�������Ȃ�
	if (this->GetIsVal())return;

	//�e�N�X�`���n���h���������ĂȂ������牽�����Ȃ�
	if (this->GetHandle() == nullptr)return;

	//�_�ł�off�̂Ƃ��͉������Ȃ�
	if (!Blinking::NowBlinking())return;

	//�`�悪off�̂Ƃ��͉������Ȃ�
	if (this->GetState() != STATE::DRAW)return;
	
	//�`��
	this->DrawTexture();
}

void TextureBase::Finalize()
{
}

void TextureBase::SetTexture(ITextureBase* texture)
{
	//�e�N�X�`���n���h����ݒ肷��
	this->SetHandle(texture->GetHandle());

	//���W��ݒ肷��
	this->SetPosition(texture->GetPosition());

	//�x�N�g����ݒ肷��
	this->SetVelocity(texture->GetVelocity());

	//�n�_�y�яI�_��ݒ肷��
	this->SetRect(texture->GetRect());

	//�F��ݒ肷��
	this->SetColor(texture->GetColor());

	//��]��ݒ肷��
	this->SetAngle(texture->GetAngle());

	//�g��{����ݒ肷��
	this->SetScale(texture->GetScale());

	//���O��ݒ肷��
	this->SetName(texture->GetName());

	//��Ԃ�ݒ肷��
	this->SetState(texture->GetState());

	//�`�揇��ݒ肷��
	this->SetDrawOrder(texture->GetDrawOrder());

	//�p�X��ݒ肷��
	this->SetPath(texture->GetPath());

	//�폜�t���O��ݒ肷��
	this->SetIsVal(false);

	//�摜�̒��S�ʒu��ݒ肷��
	this->SetOrigin(Vector2::Zero);

	//SpriteEffect��ݒ肷��
	this->SetSpriteEffect(SpriteEffects::SpriteEffects_None);

	//�X�V�֐���ݒ肷��
	this->SetActive(false);

	//�`�揇��ݒ肷��i���X�g�ɉe������Ȃ��j
	this->SetLayerDenpth(0.0f);
}

const Vector2 TextureBase::AskOrigin() const
{
	//�؂���ʒu���擾����
	RECT rect = this->GetRect();

	//�؂�o���ʒu����摜�̒��S�ʒu�����߂�
	Vector2 origin = Vector2((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
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
	//���M����Ă�������I�u�W�F�N�g�̍��W�ŁA�␳����
	//�n�_�I�_�̎擾
	RECT rect1 = this->GetRect();
	RECT rect2 = texture->GetRect();

	//�摜�̔��a�̎擾
	Vector3 size1 = Vector3(rect1.right - rect1.left, rect1.bottom - rect1.top, 0.0f) * this->GetScale();
	Vector3 size2 = Vector3(rect2.right - rect2.left, rect2.bottom - rect2.top, 0.0f) * texture->GetScale();

	Vector3 halfsize1 = size1 * 0.5f;
	Vector3 halfsize2 = size2 * 0.5f;

	//���ꂼ��̍��W�����߂�i���W�͉摜�̒��S�œ��ꂷ��j
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
	//�E���������Ă���
	if (this->GetSpriteEffect() == SpriteEffects::SpriteEffects_None)
	{
		return 1;
	}

	//�����������Ă���
	if (this->GetSpriteEffect() == SpriteEffects::SpriteEffects_FlipHorizontally)
	{
		return -1;
	}

	return 0;
}

void TextureBase::DrawTexture()
{
	//�`��n�̏��
	Graphics* Graph = Graphics::GetInstance();
	SpriteBatch* Sprite = Graph->GetSpriteBatch();
	CommonStates* State = Graph->GetCommonStates();

	//�`��
	Sprite->Draw(this->GetHandle().Get(), this->GetPosition(), &this->GetRect(), this->GetColor(),
		XMConvertToRadians(this->GetAngle()), this->GetOrigin(), this->GetScale(),this->GetSpriteEffect());
}

const std::vector<TextureBase> TextureBase::Cast(const std::vector<std::vector<std::string>>& input)
{
	//Texture��vector��錾����
	std::vector<TextureBase> texture;

	//vector�̃T�C�Y�͓ǂݍ��񂾃f�[�^�̏c�̃T�C�Y�|1�Ƃ���i1�s�ڂ̃f�[�^�̓t�B���^�|�̂��߁j
	texture.resize(input.size() - 1);

	//int��vector��vector��錾����i2�����z��:�ǂݍ��񂾃f�[�^��std::string�Ȃ��߁j
	std::vector<std::vector<int>>texture_data;

	//vector�̃T�C�Y�͓ǂݍ��񂾃f�[�^�̏c�̃T�C�Y�|1�Ƃ���i1�s�ڂ̃f�[�^�̓t�B���^�|�̂��߁j
	texture_data.resize(input.size() - 1);

	//std::string��vector��錾����(���̂܂ܓ���悤�Ƃ���ƁA�z����ɂ��ꂪ�����邽��)
	std::vector<std::string> tag;

	//std::wstring��vector��錾����(���̂܂ܓ���悤�Ƃ���ƁA�z����ɂ��ꂪ�����邽��)
	std::vector<std::wstring> path;

	//�ǂݍ��񂾃f�[�^��2�s�ڂ���f�[�^�Ƃ��Ĉ����i1�s�ڂ̃f�[�^�̓t�B���^�|�̂��߁j
	for (size_t i = 0; i < input.size() - 1; i++)
	{
		for (size_t j = 0; j < input[i].size(); j++)
		{
			if (j == 0)tag.push_back(input[i + 1][j]);
			else if (j == input[i].size() - 1)path.push_back(TextureBase::Convert(input[i + 1][j]));
			else texture_data[i].push_back(std::stoi(input[i + 1][j]));
		}
	}

	//�ǂݍ��񂾃f�[�^��ϊ�����
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

	//�ǂݍ��񂾃f�[�^��Ԃ�
	return texture;
}

//std::string����std::wstring�ւ̕ϊ�
const std::wstring TextureBase::Convert(const std::string byteString)
{
	// ������
	size_t length = byteString.size();
	// �ϊ��㕶����
	size_t convLength;
	// ���̎󂯎M��p��
	wchar_t* wc = (wchar_t*)malloc(sizeof(wchar_t) * (length + 2));
	// �ϊ�
	mbstowcs_s(&convLength, wc, length + 1, byteString.c_str(), _TRUNCATE);
	// �Ԃ�l�֕ۑ�
	std::wstring str(wc);
	// ���̎󂯎M��j��
	free(wc);

	return str;
}
