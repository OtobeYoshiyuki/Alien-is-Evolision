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

	//ファイルを開くのに成功したら開く
	if (File->Open(L"FileData/1バイト文字.csv", Stream::FILE::STREAM_IFSTREAM))
	{
		//1バイト文字の情報を読み込む
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
				//中身が空の時処理を飛ばす
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
	const std::string& str,//対象の文字
	const int& number,//桁数
	const std::string& add,//追加する文字
	const int mode//0(後ろから)1(前から)
)
{
	//Listを使用する
	std::list<std::string> strVec;
	strVec.push_back(str);

	//変換する桁数を求める
	int ask = number - (int)str.length();

	//返す文字列
	std::string answer;

	//桁数がマイナスの場合は、何もしない
	if (ask < NULL)return answer;

	for (int i = 0; i < ask; i++)
	{
		//指定した文字を後ろに追加する
		if (mode == 0)strVec.push_back(add);
		//指定した文字を前に追加する
		else strVec.push_front(add);
	}

	//範囲forで回す
	for (std::string& s : strVec)
	{
		answer += s;
	}

	//追加した文字を返す
	return answer;
}

