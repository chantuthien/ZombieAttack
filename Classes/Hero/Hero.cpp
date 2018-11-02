#include "Hero.h"
#include "Parameter.h"
#include "Zombie/PZombie.h"

USING_NS_CC;

#define HEALTH_HERO = 100.0f;

Hero::Hero() 
{
	_health = 100.0f;
}

Hero::~Hero()
{
}

std::map<Hero::AnimationType, AnimationInfo> Hero::_mapAnimation =
{
	{Hero::AnimationType::IDLE, AnimationInfo("idle%2d.png",1,6,CC_REPEAT_FOREVER)}
};

bool Hero::init()
{
	if (!GameObject::init())
	{
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
	_sprhero = Sprite::createWithSpriteFrameName("Redneck0.png");
	this->addChild(_sprhero);
	this->setTag(HERO_TAG);
	_sprhero->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	this->setContentSize(_sprhero->getContentSize());
	_sprhero->setScale(1.0f * SCALE_PARAMETER_);

	auto physicForHero = PhysicsBody::createBox(_sprhero->getContentSize() * 2.0f);
	physicForHero->setDynamic(false);
	physicForHero->setContactTestBitmask(true);
	this->setPhysicsBody(physicForHero);

	Animation* fatguyanim = Animation::create();

	for (int i = 0; i < 8; i++)
	{
		std::string name = StringUtils::format("Redneck%d.png", i);
		fatguyanim->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
	}
	fatguyanim->setDelayPerUnit(1 / 4.0f);
	Animate* animate = Animate::create(fatguyanim);
	_sprhero->runAction(RepeatForever::create(animate));

	return true;
}

void Hero::onCollission(GameObject *obj)
{
	if (obj->getTag() == ZOMBIE_TAG)
	{
		_health -= PZombie::damageOfZombie;
		updateHealthBar(_health);
		if (_health <= 0)
		{
			CCLOG("Dead");
		}
	}
}

void Hero::setHealthBar(float percent)
{
	auto winSize = Director::getInstance()->getWinSize();

	_healthbarHero = ui::LoadingBar::create("HeroBar.png");
	this->addChild(_healthbarHero);
	_healthbarHero->setDirection(ui::LoadingBar::Direction::LEFT);
	_healthbarHero->setScaleX(0.4f);
	_healthbarHero->setScaleY(0.4f);
	_healthbarHero->setPercent(percent);
	_healthbarHero->setPosition(Vec2(50.0f, 150.0f));
}
void Hero::getHealthBar(float percent)
{
	setHealthBar(percent);
}
void Hero::updateHealthBar(float percent)
{
	_healthbarHero->removeFromParent();
	setHealthBar(percent);
}

void Hero::shootAnimation()
{
	/*Size winSize = Director::getInstance()->getWinSize();
	Animation* animation = Animation::create();

	for (int i = 1; i < 11; i++)
	{
		std::string name = StringUtils::format("fireSG%02d.png", i);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
	}
	animation->setDelayPerUnit(1 / 30.0f);
	Animate* animate = Animate::create(animation);
	_sprhero->runAction(animate);*/
}

void Hero::playAnimation(AnimationInfo info)
{
	Animation* animation = Animation::create();
	for (int i = 0; i < info._numframe; i++)
	{
		std::string name = StringUtils::format("%02d.png", i);
		animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(info._name + name));
	}
	animation->setDelayPerUnit(1 / info._fps);
	Animate* animate = Animate::create(animation);
	_sprhero->runAction(animate);
}

void Hero::playAnimation(std::string name, int numframe, int fps, int loop )
{
	cocos2d::Animation* charAnimation = cocos2d::AnimationCache::getInstance()->getAnimation(_name);
	if (charAnimation == NULL) { // not in cached yet
		cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
		for (int i = 0; i < numframe; i++) {
			cocos2d::SpriteFrame* frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat(_name.c_str(), i)->getCString());
			if (frame == nullptr || frame == (void*)0x1) 
			{
				continue;
			}
			else 
			{
				animFrames.pushBack(frame);
			}
		}
		charAnimation = cocos2d::Animation::createWithSpriteFrames(animFrames, 1.0f / fps);
		cocos2d::AnimationCache::getInstance()->addAnimation(charAnimation, name);
	}
	auto sequenceAction = cocos2d::Sequence::create(Repeat::create(cocos2d::Animate::create(charAnimation), loop), nullptr);
	_sprhero->runAction(sequenceAction);
}