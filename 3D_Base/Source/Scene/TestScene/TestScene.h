#pragma once
#include <memory>
#include <d3dx9.h>
#include "Collision/Collider/CollisionSphere/CollisionSphere.h"
#include "Collision/CollisionManager/CollisionManager.h"
#include "Scene/CScene.h"

class CollisionTestScene 
	: public CScene
{
public:

	CollisionTestScene();
	~CollisionTestScene();
	HRESULT Create()	override;
	HRESULT LoadData()	override;
	void Destroy()		override;
	void Update()		override;
	void Draw()			override;

private:
    std::unique_ptr<CollisionSphere> m_SphereA;
    std::unique_ptr<CollisionSphere> m_SphereB;
    D3DXMATRIX m_ViewProj; // DirectXMath Å® D3DXMATRIX Ç…ïœçX
};
