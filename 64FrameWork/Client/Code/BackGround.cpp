#include "stdafx.h"
#include "BackGround.h"
#include "Engine_Defines.h"
#include "Export_Function.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBackGround::~CBackGround(void)
{

}

HRESULT CBackGround::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale.x = m_pTextureCom->Get_ImageInfo().Width;
	m_pTransformCom->m_vScale.y = m_pTextureCom->Get_ImageInfo().Height;
	m_pTransformCom->m_vScale.z = 0.5f;
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = _vec3{ 0,20,0.01f };

	
	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	m_fFrameCnt += m_fFrameMax * fTimeDelta * 0.5f;

	if (m_fFrameCnt >= m_fFrameMax)
		m_fFrameCnt = 0.f;


	Engine::CGameObject::Update_GameObject(fTimeDelta);	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_UI, this);


	return 0;
}

void CBackGround::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_pTransformCom->m_matWorld);
	m_pTextureCom->Render_Texture((_uint)m_fFrameCnt);
	m_pBufferCom->Render_Buffer();
}

HRESULT CBackGround::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);

	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);
	m_pComponentMap[Engine::ID_STATIC].insert(make_pair(L"Com_Buffer", pComponent));


	pComponent = m_pTransformCom = Engine::CTransform::Create();
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//assert(pComponent == nullptr);
	m_pComponentMap[Engine::ID_DYNAMIC].insert(make_pair(L"Com_Transform", pComponent));

	//m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_LOGO, L"Texture_Logo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].insert(make_pair(L"Com_Texture", pComponent));

	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].insert(make_pair(L"Com_Renderer", pComponent));

	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	return S_OK;
}



CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround*	pInstance = new CBackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CBackGround::Free(void)
{


	Engine::CGameObject::Free();
}

