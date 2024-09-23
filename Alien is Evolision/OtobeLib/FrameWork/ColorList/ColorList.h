#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include <vector>

namespace OtobeLib
{
	class ColorList:public Singleton<ColorList>//色の情報を持った配列を持つ
	{
		friend class Singleton<ColorList>;//基本的にメンバにアクセスしない
	public:
		enum class COLORLIST :int
		{
		 AliceBlue,
		AntiqueWhite,
		Aqua,
		Aquamarine,
		Azure,
		Beige,
		Bisque,
		Black,
		BlanchedAlmond,
		Blue,
		BlueViolet,
		Brown,
		BurlyWood,
		CadetBlue,
		Chartreuse,
		Chocolate,
		Coral,
		CornflowerBlue,
		Cornsilk,
		Crimson,
		Cyan,
		DarkBlue,
		DarkCyan,
		DarkGoldenrod,
		DarkGray,
		DarkGreen,
		DarkKhaki,
		DarkMagenta,
		DarkOliveGreen,
		DarkOrange,
		DarkOrchid,
		DarkRed,
		DarkSalmon,
		DarkSeaGreen,
		DarkSlateBlue,
		DarkSlateGray,
		DarkTurquoise,
		DarkViolet,
		DeepPink,
		DeepSkyBlue,
		DimGray,
		DodgerBlue,
		Firebrick,
		FloralWhite,
		ForestGreen,
		Fuchsia,
		Gainsboro,
		GhostWhite,
		Gold,
		Goldenrod,
		Gray,
		Green,
		GreenYellow,
		Honeydew,
		HotPink,
		IndianRed,
		Indigo,
		Ivory,
		Khaki,
		Lavender,
		LavenderBlush,
		LawnGreen,
		LemonChiffon,
		LightBlue,
		LightCoral,
		LightCyan,
		LightGoldenrodYellow,
		LightGreen,
		LightGray,
		LightPink,
		LightSalmon,
		LightSeaGreen,
		LightSkyBlue,
		LightSlateGray,
		LightSteelBlue,
		LightYellow,
		Lime,
		LimeGreen,
		Linen,
		Magenta,
		Maroon,
		MediumAquamarine,
		MediumBlue,
		MediumOrchid,
		MediumPurple,
		MediumSeaGreen,
		MediumSlateBlue,
		MediumSpringGreen,
		MediumTurquoise,
		MediumVioletRed,
		MidnightBlue,
		MintCream,
		MistyRose,
		Moccasin,
		NavajoWhite,
		Navy,
		OldLace,
		Olive,
		OliveDrab,
		Orange,
		OrangeRed,
		Orchid,
		PaleGoldenrod,
		PaleGreen,
		PaleTurquoise,
		PaleVioletRed,
		PapayaWhip,
		PeachPuff,
		Peru,
		Pink,
		Plum,
		PowderBlue,
		Purple,
		Red,
		RosyBrown,
		RoyalBlue,
		SaddleBrown,
		Salmon,
		SandyBrown,
		SeaGreen,
		SeaShell,
		Sienna,
		Silver,
		SkyBlue,
		SlateBlue,
		SlateGray,
		Snow,
		SpringGreen,
		SteelBlue,
		Tan,
		Teal,
		Thistle,
		Tomato,
		Transparent,
		Turquoise,
		Violet,
		Wheat,
		White,
		WhiteSmoke,
		Yellow,
		YellowGreen,
		NUM,
		};
		public:
			static const int MAX_COLOR = static_cast<int>(COLORLIST::NUM);
	private:
		std::vector<DirectX::SimpleMath::Color>	m_ColorList;//カラーリスト
		int m_color_number;//カラーの色
		int m_time;//当たった時のタイマー
		private:
			static const int TIMER_INTERVAL = 30;
	private:
		ColorList();
	public:
		~ColorList();
		void Create();//デバイスを生成
		void Initialize();//リスト作成
		void CountUp();//更新
		const DirectX::SimpleMath::Color& GetColorList(bool flag,int list = NULL)const;//フラグがtrueの時は1秒ごとに色が変る　そうでなければ、指定した色を参照
		void NumberUp();//タイマー更新
		std::vector<DirectX::SimpleMath::Color> Cast(std::vector<std::vector<std::string>>& input);
		const DirectX::SimpleMath::Color& GetColor(const int& index)const { return m_ColorList[index]; }
	};
}

