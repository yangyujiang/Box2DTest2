#ifndef __INSECCT_H__
#define __INSECCT_H__

#include "cocos2d.h"

#include "Box2D\Box2D.h"

class Insect:public cocos2d::CCNode
{
public:
	static b2World* _world;
protected:
	cocos2d::CCSprite* _sprite;
	cocos2d::CCAction* _randomAction;
	float _velocity;//虫子速度
	cocos2d::CCPoint _prePosition;

	b2Body* _body;//虫子身体区域
public:
	static void createWorld(b2World* world);
    bool virtual init();
	CREATE_FUNC(Insect);
	Insect():_randomAction(NULL)
		,_sprite(NULL)
		,_velocity(200)
		,_prePosition(ccp(0,0))
		,_body(NULL){}
	virtual ~Insect();

	void createInsectBody();
	void addBodyForSprite();

	virtual void runAction(cocos2d::CCAction* action);
	void runRandom();
	void stopRandomRunning();
	void pauseRandomRunning();
	void rotateToAndRestartRandomRunning(float angle);
	void rotateTo(float angle);
	bool boundsChecking();//边界检查
	void tick(float dt);
};

#endif //__INSECCT_H__