#include "RandomBy.h"

using namespace cocos2d;

CCRandomBy* CCRandomBy::create(float duration,  const float velocity)
{
    CCRandomBy *pRandomBy = new CCRandomBy();
    pRandomBy->initWithDuration(duration, velocity);
    pRandomBy->autorelease();

    return pRandomBy;
}

bool CCRandomBy::initWithDuration(float duration,  const float velocity)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_velocity = velocity;
        return true;
    }

    return false;
}

CCObject* CCRandomBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CCRandomBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCRandomBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCRandomBy();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_velocity);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCRandomBy::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
	this->changeState();
	/*double degree=pTarget->getRotation()/180*M_PI;
	double distance=m_velocity*m_fDuration;
	m_delta = ccp(distance*cos(degree), distance*sin(degree));*/
}

void CCRandomBy::update(float time)
{CCLog("running");
    if (m_pTarget)
    {
		if(time*m_fDuration-_preChangeTime>1){
			_preChangeTime+=1;
			this->changeState();
		}
		float newAngle=0;
		switch(_moveState){
		case 1:
			newAngle=m_pTarget->getRotation()+_sita;
			newAngle=(int)newAngle%360;
			//if(newAngle>180) newAngle=360-newAngle;
			//if(newAngle<-180) newAngle=360+newAngle;
			m_pTarget->setRotation(newAngle);//设置新的角度
			CCLog("newAngle:%f",newAngle);
		case 0:
			double degree=(m_pTarget->getRotation()-90)/180*M_PI;
			double distance=m_velocity*(m_fDuration*time-_countTime);
			m_delta = ccp(distance*cos(degree), -distance*sin(degree));
			CCPoint newPosition=ccp(m_startPosition.x + m_delta.x ,m_startPosition.y + m_delta.y);
			m_pTarget->setPosition(newPosition);
			m_startPosition=newPosition;
			CCLog("(%f)",distance);
			CCLog("degree:%f",degree);
			CCLog("(%f,%f)",m_delta.x,m_delta.y);
			_countTime=m_fDuration*time;
			break;
		}//switch
    }
}

void CCRandomBy::changeState(){
	if(CCRANDOM_0_1()>ODDS) return;

	_moveState=(_moveState+1)%2;
	switch(_moveState){
	case 0:

		break;
	case 1:
		_sita=180*CCRANDOM_MINUS1_1()*0.01f;
		break;
	default:
		break;
	}
}