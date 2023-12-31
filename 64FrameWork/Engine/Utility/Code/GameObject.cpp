#include "GameObject.h"

USING(Engine)

Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CGameObject::~CGameObject()
{

}

void CGameObject::Set_Enable(bool bEnable)
{
	m_bEnable = bEnable;
}

void CGameObject::Set_ObjType(OBJECTTYPE objType)
{
}

void CGameObject::Set_MonsterType(MONSTERTYPE monsterType)
{
}

void CGameObject::Set_ItemType(ITEMTYPE itemType)
{
}

void CGameObject::Set_ObjName(const _tchar * objName)
{
	m_ObjName = objName;
}

_uint CGameObject::Get_ObjIdx()
{
	return m_uiIdx;
}

void CGameObject::Set_ObjIdx(_uint uiIDx)
{
	m_uiIdx = uiIDx;
}

bool CGameObject::Get_Enable(void) const
{
	return m_bEnable;
}

OBJECTTYPE CGameObject::Get_ObjType()
{
	return OBJECTTYPE();
}

MONSTERTYPE CGameObject::Get_MonsterType()
{
	return MONSTERTYPE();
}

ITEMTYPE CGameObject::Get_ItemType()
{
	return ITEMTYPE();
}

wstring CGameObject::Get_ObjName()
{
	return m_ObjName;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = m_pComponentMap[eID].begin();

	for (; iter != m_pComponentMap[eID].end(); iter++)
	{
		if (iter->first == pComponentTag)
			return iter->second;
	}
	return nullptr;
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);
	
	return pComponent;
}

HRESULT Engine::CGameObject::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CGameObject::LateReady_GameObject(void)
{
	return S_OK;
}

_int Engine::CGameObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& iter : m_pComponentMap[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

_int CGameObject::LateUpdate_GameObject(const _float & fTimeDelta)
{
	for (auto& iter : m_pComponentMap[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return S_OK;
}

void Engine::CGameObject::Render_GameObject(void)
{

}

void Engine::CGameObject::Free(void)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		for_each(m_pComponentMap[i].begin(), m_pComponentMap[i].end(), CDeleteMap());
		m_pComponentMap[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}

