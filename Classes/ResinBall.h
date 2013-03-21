#ifndef __RESIN_BALL_SPRITE_H__
#define __RESIN_BALL_SPRITE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include "vector"

class ResinBallBody{
public:
	vector<b2Body*> _blocks;//圆环链条的方块数组
	vector<b2Joint*> _blockJoints;//方块之间的旋转关节的存储数组
	b2Body* _ballBody;//实心球刚体
	b2World* _world;//世界
	cocos2d::CCPoint _position;//物体所在位置,即实心圆的圆心坐标，单位像素
	float32 _currentBlockHeight;//小方块长度
	float32 _currentBlockWidth;//当前小方块宽度
	float32 _radius;//外围半径
public:
	virtual bool init(b2World* world);
	ResinBallBody();
	virtual ~ResinBallBody();
	void createCircleBridge(int number,float32 radius);
	void shrinkResinBallBody(float32 scale);//缩小树脂球模型刚体

	void accelerateCallBack(cocos2d::CCAcceleration* pAccelerationValue);//加速器变化时（倾斜手机屏幕）的回调函数
};

class ResinBall : public cocos2d::CCSprite
{
protected:
	ResinBallBody* _resinBallBody;//树脂球模型刚体
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
  //  virtual bool init();  
	ResinBall();
	virtual ~ResinBall();
	/*
    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
	void tick(float dt);
	void changeXYZShow(float dt);
	void createWorld();
	void createDebugDraw();
	void createWrapWall();//创建四周边界墙
	void createResinModel(int radius);//根据指定半径创建树脂球模型
	void createCircleBridge(int segmentNumber,int radius);//根据指定的数量和半径，创建圆链条
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
	*/
    // implement the "static node()" method manually
   // CREATE_FUNC(HelloWorld);
};


#endif  // __RESIN_BALL_SPRITE_H__