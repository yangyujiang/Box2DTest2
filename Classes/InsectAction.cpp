#include "InsectAction.h"
#include "RandomBy.h"

using namespace cocos2d;

InsectAction::InsectAction(void)
{
}


InsectAction::~InsectAction(void)
{
}

CCActionInterval* InsectAction::createRandomAction(float velocity){
	CCActionInterval *randomAction=CCRandomBy::create(10,velocity);
	CCActionInterval *action=CCRepeatForever::create(randomAction);
	return action;
}

CCAction* InsectAction::createBezierAction(float velocity,float angle,CCPoint currentPosition){
	CCPoint point1=currentPosition;//ccp(400,150);
	CCPoint point2=ccp(400,260);
	CCPoint endPosition=ccp(340,100);

	float distance_2=velocity*2;//2秒所走的路程
	float dis_cto1=ccpDistance(currentPosition,point1);
	float dis_1to2=ccpDistance(point1,point2);

	ccPointSize(50);
	ccDrawColor4B(255,0,0,255);
	ccDrawPoint(point1);
	ccDrawPoint(point2);
	ccDrawPoint(endPosition);
	CCActionInterval *action;
	ccBezierConfig bezierCon;
	bezierCon.controlPoint_1=point1;
	bezierCon.controlPoint_2=point2;
	bezierCon.endPosition=endPosition;
	CCActionInterval *bezierAction=CCBezierBy::create(2,bezierCon);

	action=bezierAction;
	return action;
}