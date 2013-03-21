#ifndef __MYDESTRUCION_LISTENER_H__
#define __MYDESTRUCION_LISTENER_H__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

class MyDestructionListener:public b2DestructionListener
{
	 MyDestructionListener();  
    virtual ~MyDestructionListener();  
      
      
    /// Called when any joint is about to be destroyed due   
    /// to the destruction of one of its attached bodies.   
    virtual void SayGoodbye(b2Joint* joint);  
      
      
    /// Called when any fixture is about to be destroyed due   
    /// to the destruction of its parent body.   
    virtual void SayGoodbye(b2Fixture* fixture);  

};

#endif //__MYDESTRUCION_LISTENER_H__