#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include "vector"

#include "ResinBall.h"
#include "Insect.h"
#include "myContactListener.h"

class HelloWorld : public cocos2d::CCLayer
{
protected:
	static const int MAX_VELOCITY=20;//单位米/秒
	cocos2d::CCLabelTTF* _pLabel;
	b2World *_world;
	b2MouseJoint *_mouseJoint;
	b2Fixture *_circleFix;
	b2Body *_groundBody;
	b2Body *_body1;//主角 实心球的刚体body
	vector<b2Body*> _blocks;
	const char *showXYZ;
	vector<b2Joint*> _blockJoints;

	ResinBallBody* resinBallBody;//主角的刚体
	vector<Insect*> _insects;//虫子

	myContactListener *contactListener;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(); 
	HelloWorld();
	virtual ~HelloWorld();
	void registerContactListener();
	void unregisterContactListener();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
	void initInsects();

	void tick(float dt);
	void changeXYZShow(float dt);
	void createWorld();
	void createDebugDraw();
	void createWrapWall();//创建四周边界墙
	void createBridge();
	void createRigidCircle(int segmentNumber,int radius);
	void createRopeJoint(int number);//参数为关节个数
	void didAccelerateFor(b2Body *body,double x,double y,double z);//模拟body的重力和摩擦力，
	void drawResin();
	void draw();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	void ccTouchesBegan(cocos2d::CCSet *touched,cocos2d::CCEvent *event);
	void ccTouchesMoved(cocos2d::CCSet *touches,cocos2d::CCEvent *event);
	void ccTouchesCancelled(cocos2d::CCSet *touches, cocos2d::CCEvent *event);
	void ccTouchesEnded(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

	void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__