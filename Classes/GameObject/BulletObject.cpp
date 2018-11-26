#include "GameObject/BulletObject.h"
#include "Parameter.h"
#include "GameObject/PZombie.h"
#include "GamePlayLayer.h"

BulletObject::BulletObject()
{
	_willBeDestroy = false;
}

BulletObject::~BulletObject()
{
}

bool BulletObject::init(Vec2 location)
{
	if (!GameObject::init())
	{
		return false;
	}
	auto winSize = Director::getInstance()->getWinSize();
	_bulletPhysicBody = PhysicsBody::createCircle(8.0f);
	_bulletPhysicBody->setContactTestBitmask(true);
	_bulletPhysicBody->setDynamic(false);
	_bulletPhysicBody->setGroup(-2);
	this->setPhysicsBody(_bulletPhysicBody);
	bulletFire(location);
	this->setTag(TAG_BULLET);
	scheduleUpdate();
	return true;
}

BulletObject *BulletObject::create(Vec2 location)
{
	BulletObject *pRet = new(std::nothrow) BulletObject();
	if (pRet && pRet->init(location))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void BulletObject::reset(Vec2 location)
{
	Size winSize = Director::getInstance()->getWinSize();
	this->setPosition(winSize.width * 0.25f, winSize.height * 0.25f);
	this->setVisible(true);
	bulletFire(location);
	scheduleUpdate();
}

float BulletObject::getDamage()
{
	return _Dmg;
}

void BulletObject::onCollission(GameObject *obj)
{
	if (obj->getTag() == TAG_ZOMBIE1 || obj->getTag() == TAG_ZOMBIE2 ||
		obj->getTag() == TAG_ZOMBIE3 || obj->getTag() == TAG_ZOMBIE4 ||
		obj->getTag() == TAG_BORDER)
	{
		_willBeDestroy = true;
	}

}

void BulletObject::setOnDestroyCallback(OnBulletDestroyCallback callback)
{
	_onBulletDestroyCallback = callback;
}

void BulletObject::bulletFire(Vec2 location)
{
	Size winSize = Director::getInstance()->getWinSize();
	float recoil = random(-15, 40);
	auto startPos = this->getPosition();
	auto target = location;
	auto distance = target - startPos;
	distance.y += recoil;
	vector = distance.getNormalized() * BULLET_VEC;
	auto aBulletFire = MoveBy::create(1.0f, vector);
	CallFunc *callback = CallFunc::create([=]
	{
		_willBeDestroy = true;
	}
	);
	this->runAction(Sequence::create(aBulletFire, callback, NULL));
}

void BulletObject::setDamageBullet(int Dmg)
{
	_Dmg = Dmg;
}
void BulletObject::update(float delta)
{
	if (_willBeDestroy)
	{
		if (_onBulletDestroyCallback)
		{
			_onBulletDestroyCallback(this);
		}
		stopAllActions();
		this->removeFromParent();
		_willBeDestroy = false;
	}
}