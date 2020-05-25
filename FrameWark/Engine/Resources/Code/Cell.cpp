#include "Cell.h"
#include "Line.h"

USING(Engine)

Engine::CCell::CCell(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * NEIGHBOR_END);
}

Engine::CCell::~CCell(void)
{

}

void CCell::Set_NaviData(Engine::NAVI_DATA naviData)
{
	m_vPoint[POINT_A] = naviData.vPosition1;
	m_vPoint[POINT_B] = naviData.vPosition2;
	m_vPoint[POINT_C] = naviData.vPosition3;
}

Engine::NAVI_DATA CCell::Get_NaviData()
{
	Engine::NAVI_DATA naviMesh;
	naviMesh.vPosition1 = m_vPoint[POINT_A];
	naviMesh.vPosition2 = m_vPoint[POINT_B];
	naviMesh.vPosition3 = m_vPoint[POINT_C];

	return naviMesh;
}

HRESULT Engine::CCell::Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	m_dwIndex = dwIndex;

	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;

	m_pLine[LINE_AB] = CLine::Create(&_vec3(m_vPoint[POINT_A].x,m_vPoint[POINT_A].y, m_vPoint[POINT_A].z), 
									 &_vec3(m_vPoint[POINT_B].x,m_vPoint[POINT_B].y, m_vPoint[POINT_B].z));

	m_pLine[LINE_BC] = CLine::Create(&_vec3(m_vPoint[POINT_B].x,m_vPoint[POINT_B].y, m_vPoint[POINT_B].z),
									 &_vec3(m_vPoint[POINT_C].x,m_vPoint[POINT_C].y, m_vPoint[POINT_C].z));

	m_pLine[LINE_CA] = CLine::Create(&_vec3(m_vPoint[POINT_C].x,m_vPoint[POINT_C].y, m_vPoint[POINT_C].z),
									 &_vec3(m_vPoint[POINT_A].x,m_vPoint[POINT_A].y, m_vPoint[POINT_A].z));

#ifdef _DEBUG
	FAILED_CHECK_RETURN(D3DXCreateLine(m_pGraphicDev, &m_pD3DXLine), E_FAIL);

#endif
	return S_OK;
}

_bool Engine::CCell::Compare_Point(const _vec3* pPointF, const _vec3* pPointS, CCell* pCell)
{
	if (m_vPoint[POINT_A] == *pPointF)
	{
		if (m_vPoint[POINT_B] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_B] == *pPointF)
	{
		if (m_vPoint[POINT_A] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		if (m_vPoint[POINT_C] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoint[POINT_C] == *pPointF)
	{
		if (m_vPoint[POINT_B] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
		if (m_vPoint[POINT_A] == *pPointS)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}


	return false;
}

void Engine::CCell::Render_Cell(void)
{
	_vec3	vPoint[4];

	vPoint[0] = m_vPoint[POINT_A];
	vPoint[1] = m_vPoint[POINT_B];
	vPoint[2] = m_vPoint[POINT_C];
	vPoint[3] = m_vPoint[POINT_A];

	_matrix		matView, matProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f);	// 라인의 굵기를 결정하는 함수
	m_pGraphicDev->EndScene();
	m_pGraphicDev->BeginScene();

	m_pD3DXLine->Begin();

	_matrix matTemp;

	m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pD3DXLine->End();

}

Engine::CCell* Engine::CCell::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CCell*	pInstance = new CCell(pGraphicDev);

	if (FAILED(pInstance->Ready_Cell(dwIndex, pPointA, pPointB, pPointC)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCell::Free(void)
{
	for (_uint i = 0; i < LINE_END; ++i)
		Safe_Release(m_pLine[i]);

	Safe_Release(m_pD3DXLine);
	Safe_Release(m_pGraphicDev);
}

Engine::CCell::COMPARE Engine::CCell::Compare(const _vec3* pEndPos, _ulong* pIndex)
{
	for (_ulong i = 0; i < LINE_END; ++i)
	{
		if (CLine::COMPARE_LEFT == m_pLine[i]->Compare(&_vec3(pEndPos->x, pEndPos->y, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return COMPARE_STOP;
			else
			{
				*pIndex = *m_pNeighbor[i]->Get_Index();
				return COMPARE_MOVE;
			}
		}
	}

	return COMPARE_MOVE;
}

