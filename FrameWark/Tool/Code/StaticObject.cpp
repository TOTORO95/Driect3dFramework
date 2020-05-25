#include "stdafx.h"
#include "StaticObject.h"

#include "Export_Function.h"



CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = wstrName;
}

CStaticObject::~CStaticObject()
{
}

HRESULT CStaticObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(DefaultMeshScale);
	m_pTransformCom->Set_Pos(DefaultObjectPos);

	return S_OK;
}

_int CStaticObject::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return S_OK;
}

void CStaticObject::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();
	_matrix		matWorld;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(m_bColl), &matWorld);
}

HRESULT CStaticObject::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, m_ObjName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_StaticMesh", pComponent);// º¯°æ

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
		m_pMeshCom->Get_VtxPos(),
		m_pMeshCom->Get_NumVtx(),
		m_pMeshCom->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

CStaticObject * CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
{
	CStaticObject*	pInstance = new CStaticObject(pGraphicDev, wstrName, uiIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
void CStaticObject::Free(void)
{
	Engine::CGameObject::Free();
}
