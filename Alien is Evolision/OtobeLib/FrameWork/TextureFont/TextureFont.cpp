#include "pch.h"
#include "TextureFont.h"
#include "OtobeLib/FrameWork/Stream/Stream.h"
#include "OtobeLib/FrameWork/Graphics/Graphics.h"
#include "OtobeLib/FrameWork/ResourceManager/ResourceManager.h"
#include "OtobeLib/FrameWork/Input/Input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

TextureFont::TextureFont()
{
	Stream* File = Stream::GetInstance();

	//�t�@�C�����J���̂ɐ���������J��
	if (File->Open(L"FileData/1�o�C�g����.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//1�o�C�g�����̏���ǂݍ���
		m_OneByteString = File->Load_Data();
		File->Close();
	}
} 

void TextureFont::DrawFormatString(const std::string& str,Vector2 pos, const float scale,
	const XMVECTORF32 color,const int interval)
{
	ResourceManager* resourceManager = ResourceManager::GetInstance();

	int Value = NULL;

	for (size_t i = 0; i < str.size(); i+= Value)
	{
		int count = NULL;
		for (size_t j = 0; j < m_OneByteString.size(); j++)
		{
			for (size_t k = 0; k < m_OneByteString[j].size(); k++)
			{
				//���g����̎��������΂�
				if (m_OneByteString[j][k] == "")continue;
				if (m_OneByteString[j][k] == str.substr(i, 1))count++;
			}
		}

		if (str.substr(i, 1) == " ")
		{
			pos.x += (32.0f) * scale + interval;
			Value = 1;
			continue;
		}

		if (count == NULL)Value = 2;
		else Value = 1;

		ITextureBase* useTex = resourceManager->GetTexture(str.substr(i, Value));
		std::unique_ptr<ITextureBase> tx = std::make_unique<TextureBase>(useTex);
		Graphics::GetInstance()->GetSpriteBatch()->Draw(tx->GetHandle().Get(), pos, &tx->GetRect(),
			color, 0.0f, Vector2::Zero, Vector3(scale));
		pos.x += (tx->GetRect().right - tx->GetRect().left) * scale + interval;
	}
}


void TextureFont::Draw_MousePos(const Vector2 posx, const Vector2 posy)
{
	const auto& Mouse_State = Input::GetInstance()->GetMouse_State();
	Vector2 Mouse_Pos = Vector2(static_cast<float>(Mouse_State.x),static_cast<float>(Mouse_State.y));

	std::string X = std::to_string(Mouse_Pos.x);
	std::string Y = std::to_string(Mouse_Pos.y);

	int value = NULL;
	for (size_t i = 0; i < X.size(); i++)
	{
		if (X[i] == '.')
		{
			value = i;
			break;
		}
	}

	int value2 = NULL;
	for (size_t i = 0; i < Y.size(); i++)
	{
		if (Y[i] == '.')
		{
			value2 = i;
			break;
		}
	}


	X = X.substr(0, value);
	Y = Y.substr(0, value2);

	DrawFormatString(X, posx, 2.0f, Colors::White);
	DrawFormatString(Y, posy, 2.0f, Colors::White);
}

const std::string TextureFont::Convert_DecimalPoint(const float& point)
{
	std::string String = std::to_string(point);

	int value = NULL;
	for (size_t i = 0; i < String.size(); i++)
	{
		if (String[i] == '.')
		{
			value = i;
			break;
		}
	}
	return String.substr(0, value);
}

const std::string TextureFont::Convert_NumberOfDigits
(
	const std::string& str,//�Ώۂ̕���
	const int& number,//����
	const std::string& add,//�ǉ����镶��
	const int mode//0(��납��)1(�O����)
)
{
	//List���g�p����
	std::list<std::string> strVec;
	strVec.push_back(str);

	//�ϊ����錅�������߂�
	int ask = number - (int)str.length();

	//�Ԃ�������
	std::string answer;

	//�������}�C�i�X�̏ꍇ�́A�������Ȃ�
	if (ask < NULL)return answer;

	for (int i = 0; i < ask; i++)
	{
		//�w�肵�����������ɒǉ�����
		if (mode == 0)strVec.push_back(add);
		//�w�肵��������O�ɒǉ�����
		else strVec.push_front(add);
	}

	//�͈�for�ŉ�
	for (std::string& s : strVec)
	{
		answer += s;
	}

	//�ǉ�����������Ԃ�
	return answer;
}

