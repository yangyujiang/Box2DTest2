#ifndef __MY_CONTACT_LISTENER_H__
#define __MY_CONTACT_LISTENER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

class myContactListener:public b2ContactListener
{
protected:
	
public:
	myContactListener();
    /// Called when two fixtures begin to touch.
    virtual void BeginContact(b2Contact* contact);
	 /// Called when two fixtures cease to touch.
    virtual void EndContact(b2Contact* contact);
};

#endif //__MY_CONTACT_LISTENER_H__