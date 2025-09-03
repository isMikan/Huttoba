#pragma once

//一旦間に合わないからスルーそのうち！！

class CBoundingSphere;
class CPlayer;
class CItemManager;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	//他の球体との当たり判定
	bool IsHit(const CBoundingSphere& pBSphere);

private:

};
