#ifndef __INSECCT_ACTION_H__
#define __INSECCT_ACTION_H__

#include "cocos2d.h"

class InsectAction:public cocos2d::CCNode
{
protected:
	
public:
    //bool virtual init();
	//CREATE_FUNC(InsectAction);
	InsectAction();
	virtual ~InsectAction();

	static cocos2d::CCActionInterval* createRandomAction(float velocity);
	static cocos2d::CCActionInterval* createRotationAction(float time,float angle);
	static cocos2d::CCAction* createBezierAction(float velocity,float angle,cocos2d::CCPoint currentPosition);
};

#endif //__INSECCT_ACTION_H__