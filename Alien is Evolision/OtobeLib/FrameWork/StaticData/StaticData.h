#pragma once
#include "OtobeLib/FrameWork/Singleton/Singleton.h"
#include <map>
#include <string>

namespace OtobeLib
{
	class StaticData :public Singleton<StaticData>
	{
		friend Singleton<StaticData>;
	private:
		std::map<std::string, int>m_IntData;
		std::map<std::string, float>m_FloatData;
		std::map<std::string, bool>m_BoolData;
		std::map<std::string, std::string>m_StringData;
	private:
		StaticData();
	public:
		~StaticData();
		void Add(const std::string& tag,const int& data);
		void Add(const std::string& tag, const float& data);
		void Add(const std::string& tag, const DirectX::SimpleMath::Vector3& data);
		void Add(const std::string& tag, const bool& data);
		void Add(const std::string& tag, const std::string& data);
		const int& Get_IntData(const std::string& tag)const;
		const float& Get_FloatData(const std::string& tag)const;
		const DirectX::SimpleMath::Vector3 Get_Vector3(const std::string& tag);
		const bool& Get_BoolData(const std::string& tag)const;
		const std::string& Get_StringData(const std::string& tag)const;
		const bool Request_Data(const std::string& tag)const;

		void IntRemove(const std::string& tag) { m_IntData.erase(tag); }
		void FloatRemove(const std::string& tag) { m_FloatData.erase(tag); };
		void Vector3Remove(const std::string& tag) 
		{ 
			m_FloatData.erase(tag + "X"); 
			m_FloatData.erase(tag + "Y");
			m_FloatData.erase(tag + "Z");
		};
		void boolRemove(const std::string& tag) { m_BoolData.erase(tag); };
		void StringRemove(const std::string& tag) { m_StringData.erase(tag); };
	};
}

