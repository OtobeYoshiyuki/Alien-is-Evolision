#include "pch.h"
#include "CameraBase.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace OtobeLib;

CameraBase::CameraBase():m_pos(Vector3::Zero),m_matrix(Matrix::Identity),m_objectMove(true)
{
}


