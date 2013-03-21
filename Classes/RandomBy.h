#ifndef __RANDOM_BY_H__
#define __RANDOM_BY_H__

#include "cocos2d.h"

class CCRandomBy : public cocos2d::CCActionInterval
{
	protected:
    cocos2d::CCPoint m_endPosition;
    cocos2d::CCPoint m_startPosition;
    cocos2d::CCPoint m_delta;
	float m_velocity;
	int _moveState;//�˶�״̬��0��ʾֱ�ߣ�1��ʾ����
	float _sita;//΢���ı�ֵ
	float _countTime;
	float _preChangeTime;
	const float ODDS;//�ı��˶�״̬�ļ���
public:
    /** initializes the action */
    bool initWithDuration(float duration, const float velocity);

    virtual CCObject* copyWithZone(cocos2d::CCZone* pZone);
    virtual void startWithTarget(cocos2d::CCNode *pTarget);
    virtual void update(float time);
	CCRandomBy():_moveState(0),_countTime(0),_preChangeTime(0),ODDS(0.8){};
	void changeState();

public:
    /** creates the action */
    static CCRandomBy* create(float duration,  const float velocity);

};

#endif //__RANDOM_BY_H__