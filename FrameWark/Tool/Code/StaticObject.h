#pragma once


#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
END


class CStaticObject : public Engine::CGameObject
{
public:
	explicit				CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx);
	virtual					~CStaticObject(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;


private:
	HRESULT					Add_Component(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

public:
	static CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev ,wstring wstrName ,_uint uiIdx=0);

private:
	virtual void			Free(void) override;
};

