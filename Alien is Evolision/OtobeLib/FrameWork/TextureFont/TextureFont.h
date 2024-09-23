#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include "OtobeLib/Texture/Blinking/Blinking.h"
#include <vector>
#include <string>
namespace OtobeLib
{
	//�t�H���g�֘A�̃N���X
	class TextureFont:public Singleton<TextureFont>
	{
		friend Singleton<TextureFont>;
	private:
		std::vector<std::vector<std::string>>		m_OneByteString;//1�o�C�g�����̃��X�g
	private:
		TextureFont();
	public:
		~TextureFont()override {};
		void DrawFormatString(const std::string& str,DirectX::SimpleMath::Vector2 pos,const float scale = 1.0f,
			const DirectX::XMVECTORF32 color = DirectX::Colors::White, const int interval = 0);
		void Draw_MousePos(const DirectX::SimpleMath::Vector2 posx, const DirectX::SimpleMath::Vector2 posy);
		const std::string Convert_DecimalPoint(const float& point);
		//��������w�肵�������ɕϊ�����
		const std::string Convert_NumberOfDigits(const std::string& str, const int& number,
			const std::string& add,const int mode = 0);
	};
}

