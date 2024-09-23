#pragma once
#include <string>
#include <vector>
namespace OtobeLib
{
	class ISceneBase
	{
	public:
		virtual ~ISceneBase()
		{}
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Finalize() = 0;
		virtual void Create() = 0;
	};
}

