#include "myContactListener.h"
#include "string"

using namespace cocos2d;

myContactListener::myContactListener(){
	b2ContactListener();
}

void myContactListener::BeginContact(b2Contact* contact){
		CCSprite *o1 = (CCSprite*)contact->GetFixtureA()->GetBody()->GetUserData();
		CCSprite *o2 = (CCSprite*)contact->GetFixtureB()->GetBody()->GetUserData();
	//	o1->setPositionX(10);
	//	o2->setPositionY(10);
		if(o1==NULL||o2==NULL) return;
		if(o1->getTag()==3||o2->getTag()==3){
		CCLog("start contact");
		}
}

void myContactListener::EndContact(b2Contact* contact){
	
		//CCLog("end contact");
}