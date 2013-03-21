#include "Insect.h"
#include "InsectAction.h"
#include "string"
#include "HelloWorldScene.h"
#include "B2EasyBox2D.h"
#include "myContactListener.h"

#define PTM_RATIO 32.0

using namespace cocos2d;
/*Insect::Insect()
{
}*/

b2World* Insect::_world=NULL;

Insect::~Insect()
{
	CC_SAFE_RELEASE(_sprite);
	CC_SAFE_RELEASE(_randomAction);
}

bool Insect::init(){
	bool pRet=false;
	do{
		_sprite=CCSprite::create("ant_0.png");
		_sprite->setRotation(90);
		CC_BREAK_IF(!_sprite);
		_sprite->retain();
		_sprite->setPosition(ccp(100,300));
		this->addChild(_sprite,0);

		_randomAction=InsectAction::createRandomAction(_velocity);
		_randomAction->retain();

		createInsectBody();
		
		CCAnimation *animation=CCAnimation::create();
		char* imageNames[]={"ant_0.png","ant_1.png","ant_2.png","ant_3.png"};
		for(int i=0;i<4;i++){
			CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage(imageNames[i]);
			CCSpriteFrame *frame0=CCSpriteFrame::createWithTexture(texture,CCRectMake(0,0,texture->getPixelsWide(),texture->getPixelsHigh())); 
			animation->addSpriteFrame(frame0);
		}
		animation->setDelayPerUnit(0.2f);
		animation->setLoops(-1);
		CCAnimate *action=CCAnimate::create(animation);
		_sprite->runAction(action);

		pRet=true;
	}while(0);
	return pRet;
}

void Insect::runAction(CCAction* action){
	_sprite->runAction(action);
}
void Insect::runRandom(){
	runAction(_randomAction);
}
void Insect::stopRandomRunning(){
	_sprite->stopAction(_randomAction);
}
 
void Insect::rotateToAndRestartRandomRunning(float angle){	
	_sprite->stopAction(_randomAction);
	CC_SAFE_RELEASE(_randomAction);
	_randomAction=InsectAction::createRandomAction(_velocity);
		_randomAction->retain();
	
	float rotateVelocity=180;//每秒钟转180度
	float deltaAngle=fabs(angle-_sprite->getRotation());
	float duration=deltaAngle/rotateVelocity;
	CCActionInterval *rotateTo=CCRotateTo::create(duration,angle);
	CCCallFunc *funCall=CCCallFunc::create(this,callfunc_selector(Insect::runRandom));
	CCFiniteTimeAction *action=CCSequence::create(rotateTo,funCall,NULL);
	runAction(action);
}

bool Insect::boundsChecking(){
	bool isReachBounds=false;
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	CCPoint point=_sprite->getPosition();
	if(ccpDistanceSQ(_prePosition,point)==0) return isReachBounds;

	float x=-1,y=-1;
	if(_sprite->getPositionX()<=0){
		x=1;
	}
	else if(_sprite->getPositionX()>=winSize.width) x=winSize.width-1;
	if(_sprite->getPositionY()<=0) y=1;
	else if(_sprite->getPositionY()>=winSize.height) y=winSize.height-1;
	CCLog("(%f,%f)",winSize.width,winSize.height);
	CCLog("(%f,%f)",_sprite->getPositionX(),_sprite->getPositionY());
	CCLog("(%f,%f)",x,y);
	if(x!=-1||y!=-1){
		if(x==-1) x=point.x;
		if(y==-1) y=point.y;
		_sprite->setPosition(ccp(x,y));
		float angle=360*CCRANDOM_0_1();
		rotateToAndRestartRandomRunning(angle);
		//restartRandomRunning();
		isReachBounds=true;
	}
	
	_prePosition=_sprite->getPosition();

	return isReachBounds;
}

void Insect::tick(float dt){
	CCLog("%d,%f",this->boundsChecking(),dt);
	b2Vec2 b2Position = b2Vec2(_sprite->getPositionX()/PTM_RATIO,
		_sprite->getPositionY()/PTM_RATIO);
	float32 b2Angle =-1* CC_DEGREES_TO_RADIANS(_sprite->getRotation());
	_body->SetTransform(b2Position, b2Angle);
}

void Insect::createWorld(b2World* world){
	/*_world=world;*/
	if(_world!=NULL) return; 
	bool doSleep = false;
	//4.创建b2World世界
	_world = new b2World(b2Vec2(0,0));
	_world->SetAllowSleeping(doSleep);
	myContactListener *lis=new myContactListener();
	_world->SetContactListener(lis);
}

void Insect::createInsectBody(){
	if(_world==NULL) return;
	b2BodyDef bodyRequest;
	bodyRequest.type=b2_dynamicBody;
	bodyRequest.userData=_sprite;
	bodyRequest.position.Set(_sprite->getPositionX()/PTM_RATIO,_sprite->getPositionY()/PTM_RATIO);
	_body=_world->CreateBody(&bodyRequest);

	b2PolygonShape shapeRequest;
	shapeRequest.SetAsBox(_sprite->getContentSize().width/2.0f/PTM_RATIO,
		_sprite->getContentSize().height/2.0f/PTM_RATIO,b2Vec2(0,0),
		CC_DEGREES_TO_RADIANS(_sprite->getRotation()));

	b2FixtureDef fixtureRequest;
	fixtureRequest.shape=&shapeRequest;
	fixtureRequest.isSensor=true;

	b2Filter filter;
	filter.groupIndex=2;
	filter.categoryBits=2;
	filter.maskBits=2;

	fixtureRequest.filter=filter;

	b2Fixture *fixture=_body->CreateFixture(&fixtureRequest);	
}

void Insect::addBodyForSprite(){

}