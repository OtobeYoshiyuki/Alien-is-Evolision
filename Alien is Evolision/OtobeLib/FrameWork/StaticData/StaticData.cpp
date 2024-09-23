#include "pch.h"
#include "StaticData.h"

using namespace OtobeLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;

StaticData::StaticData()
{

}

StaticData::~StaticData()
{
	m_IntData.clear();
	m_FloatData.clear();
	m_BoolData.clear();
	m_StringData.clear();
}

void StaticData::Add(const std::string& tag,const int& data)
{
	m_IntData[tag] = data;
}

void StaticData::Add(const std::string& tag, const float& data)
{
	m_FloatData[tag] = data;
}

void StaticData::Add(const std::string& tag, const Vector3& data)
{
	m_FloatData[tag + "X"] = data.x;
	m_FloatData[tag + "Y"] = data.y;
	m_FloatData[tag + "Z"] = data.z;
}

void StaticData::Add(const std::string& tag, const bool& data)
{
	m_BoolData[tag] = data;
}

void StaticData::Add(const std::string& tag, const std::string& data)
{
	m_StringData[tag] = data;
}

const int& StaticData::Get_IntData(const std::string& tag) const
{
	// TODO: return ステートメントをここに挿入します
	std::map<std::string, int>::const_iterator iterInt;
	iterInt = m_IntData.find(tag);
	return iterInt->second;
}

const float& StaticData::Get_FloatData(const std::string& tag) const
{
	// TODO: return ステートメントをここに挿入します
	std::map<std::string, float>::const_iterator iterFloat;
	iterFloat = m_FloatData.find(tag);
	return iterFloat->second;
}

const Vector3 StaticData::Get_Vector3(const std::string& tag)
{
	// TODO: return ステートメントをここに挿入します
	std::map<std::string, float>::const_iterator iterVector[3];
	iterVector[0] = m_FloatData.find(tag + "X");
	iterVector[1] = m_FloatData.find(tag + "Y");
	iterVector[2] = m_FloatData.find(tag + "Z");
	return Vector3(iterVector[0]->second,iterVector[1]->second,iterVector[2]->second);
}

const bool& StaticData::Get_BoolData(const std::string& tag) const
{
	std::map<std::string, bool>::const_iterator iterBool;
	iterBool = m_BoolData.find(tag);
	return iterBool->second;
	// TODO: return ステートメントをここに挿入します
}

const std::string& StaticData::Get_StringData(const std::string& tag) const
{
	std::map<std::string, std::string>::const_iterator iterString;
	iterString = m_StringData.find(tag);
	return iterString->second;
	// TODO: return ステートメントをここに挿入します
}

const bool StaticData::Request_Data(const std::string& tag) const
{
	std::map<std::string, int>::const_iterator iterInt;
	iterInt = m_IntData.find(tag);
	if (iterInt != m_IntData.end())return true;

	std::map<std::string, float>::const_iterator iterFloat;
	iterFloat = m_FloatData.find(tag);
	if (iterFloat != m_FloatData.end())return true;

	std::map<std::string, bool>::const_iterator iterBool;
	iterBool = m_BoolData.find(tag);
	if (iterBool != m_BoolData.end())return true;

	std::map<std::string, std::string>::const_iterator iterString;
	iterString = m_StringData.find(tag);
	if (iterString != m_StringData.end())return true;

	return false;
	// TODO: return ステートメントをここに挿入します
}
