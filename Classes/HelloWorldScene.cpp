#include "HelloWorldScene.h"
#include "B2DebugDrawLayer.h"
#include "B2EasyBox2D.h"
#include "math.h"
#include "string"

#define PTM_RATIO 32.0f //Box2D世界与屏幕坐标的换算比例，1米=32像素点

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld(){
	this->resinBallBody=new ResinBallBody();
}
HelloWorld::~HelloWorld(){
	this->resinBallBody->~ResinBallBody();
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
	//	CCLayerColor *layer=CCLayerColor::create(ccc4(2255,255,255,255));
		//addChild(layer);
		_mouseJoint=NULL;
		showXYZ="X:0, Y:0, Z:10";
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        _pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
		
        CC_BREAK_IF(! _pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        _pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(_pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
       // this->addChild(pSprite, 0);

		//Box2D///////////////////////////////////////////
		this->createWorld();
		this->createDebugDraw();
		this->createWrapWall();
	//	this->createRigidCircle(20,20);
	//	this->createRigidCircle(22,30);
	//	this->createRigidCircle(24,40);
		this->createBridge();
		//this->createRopeJoint(3);//创建实心圆到小方块的绳索关节
		this->setTouchEnabled(true);
		this->setAccelerometerEnabled(true);

		resinBallBody->init(_world);
		//resinBallBody->shrinkResinBallBody(1);

		this->initInsects();

		CCSprite* sprite=CCSprite::create("CloseNormal.png");
		sprite->setPosition(ccp(150,150));
		addChild(sprite);

b2BodyDef bodyDef;
bodyDef.type = b2_dynamicBody;
bodyDef.position.Set(150/PTM_RATIO, 150/PTM_RATIO);
bodyDef.userData=sprite;
b2Body* body = _world->CreateBody(&bodyDef);

b2CircleShape bodyShap;
bodyShap.m_radius=sprite->getContentSize().width/2.0f/PTM_RATIO;
body->CreateFixture(&bodyShap,5.0f);

b2Vec2 force = b2Vec2(10, 10);
		//body->ApplyLinearImpulse(force, bodyDef.position);
body->ApplyForceToCenter(b2Vec2(109,109));
		this->schedule(schedule_selector(HelloWorld::tick));
		this->schedule(schedule_selector(HelloWorld::changeXYZShow),1);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::initInsects(){
	Insect::createWorld(_world);
	for(int i=0;i<5;i++){
		Insect *insect=Insect::create();
		addChild(insect,0);
		
		insect->runRandom();
		_insects.push_back(insect);
	}
}

void HelloWorld::createWorld(){//创建一个Box2D世界
	//2.声明重力
	b2Vec2 gravity = b2Vec2(0, 0);
	//3.睡着的对象是否模拟
	bool doSleep = true;
	//4.创建b2World世界
	_world = new b2World(gravity);
	_world->SetAllowSleeping(doSleep);
}
void HelloWorld::createDebugDraw(){	
	B2DebugDrawLayer *dDraw=B2DebugDrawLayer::create(_world,PTM_RATIO);
	addChild(dDraw,9999);
}

void HelloWorld::createWrapWall(){//创建四周墙

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();

	_groundBody=B2EasyBox2D::createBox(_world,winSize.width/2,winSize.height,winSize.width,PTM_RATIO,true,0,1,0);
	B2EasyBox2D::createBox(_world,0,winSize.height/2,PTM_RATIO,winSize.height,true,0,1,0);
	B2EasyBox2D::createBox(_world,winSize.width,winSize.height/2,PTM_RATIO,winSize.height,true,0,1,0);
	B2EasyBox2D::createBox(_world,winSize.width/2,0,winSize.width,PTM_RATIO,true,0,1,0);
	//创建静态障碍物
	B2EasyBox2D::createBox(_world,100,100,50,150,true,0,1,0);
	B2EasyBox2D::createBox(_world,600,100,50,50,true,0,1,0);
	B2EasyBox2D::createBox(_world,300,300,100,100,true,0,1,0);
	B2EasyBox2D::createCircle(_world,500,500,50,true);
}

void HelloWorld::createRigidCircle(int segmentNum,int radius){
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float centerX = winSize.width / 2.f;
	float centerY = winSize.height / 2.f;
	
	//1.创建刚体需求b2BodyDef
	b2BodyDef bodyRequest;
	bodyRequest.type=b2_dynamicBody;
	bodyRequest.bullet=true;
	bodyRequest.fixedRotation=true;
	bodyRequest.position.Set(centerX /PTM_RATIO, centerY / PTM_RATIO);//记得米和像素的转换关系
			
	//2.Box2D世界工厂更具需求创建createBody()生产刚体
	b2Body *body = _world->CreateBody(&bodyRequest);

	//3.创建刚体形状需求b2ShapeDef的子类
	//根据半径和个数计算线段的长度
	float segmentHeight = radius * sin(b2_pi/segmentNum);
	//for循环创建segmentNum个线段，合成圆形边界
	for (int i = 0; i < segmentNum; i++) {
		//定义形状需求
		b2PolygonShape shapeRequest;
		b2FixtureDef fixRequest;
		//形状的质量、摩擦系数、硬度
		fixRequest.density = 0.0001f;
		fixRequest.friction = 1.f;
		fixRequest.restitution = 0.0f;
		//计算每个线段的角度、坐标
		float angle = i*1.f/segmentNum *b2_pi*2;
		float bx = radius * cos(angle);
		float by = radius * sin(angle);
		//创建有方向的矩形刚体，合成总的圆形刚体
		shapeRequest.SetAsBox(2.5f/PTM_RATIO,segmentHeight/PTM_RATIO,b2Vec2(bx/PTM_RATIO,by/PTM_RATIO),angle);
		fixRequest.shape=&shapeRequest;
		//4.b2Body刚体工厂根据需求createShape生产形状
		body->CreateFixture(&fixRequest);
	}
}

void HelloWorld::draw(){
	this->drawResin();
}

void HelloWorld::drawResin(){//画出树脂球形状
	CCPoint filledVertives[100];//={ccp(100,100),ccp(500,100),ccp(500,500),ccp(250,250),ccp(100,500)};
	for(int i=0;i<this->resinBallBody->_blockJoints.size();i++){
		filledVertives[i]=ccp(this->resinBallBody->_blockJoints[i]->GetAnchorA().x*PTM_RATIO,
			this->resinBallBody->_blockJoints[i]->GetAnchorA().y*PTM_RATIO);
	}
	ccDrawColor4B(255,0,0,255);//设置颜色为蓝色
 	ccDrawSolidPoly(filledVertives,this->resinBallBody->_blockJoints.size(),ccc4f(0,0,255,255));
	//free(filledVertives);
}

void HelloWorld::createRopeJoint(int number){
	int alta=_blocks.size()/number;
	b2Vec2 vecB=_body1->GetPosition();
	float b_radius=_body1->GetFixtureList()->GetShape()->m_radius;//实心圆半径
	for(int i=0;i<number;i+=alta){
		b2Vec2 vecA=_blocks[i]->GetPosition();
		float atob_radius=b2Sqrt(pow(vecA.x-vecB.x,2)+pow(vecA.y-vecB.y,2));//实心圆的圆心到此小方块的距离（米）
		b2Vec2 anchorB=b2Vec2(b_radius*sin(vecA.x-vecB.x)/atob_radius,b_radius*sin(vecA.y-vecB.y)/atob_radius);

		b2RopeJointDef ropeRequest;
		ropeRequest.bodyA=_blocks[i];
		ropeRequest.bodyB=_body1;
		ropeRequest.localAnchorA=b2Vec2(0,0);
		ropeRequest.localAnchorB=anchorB;
		ropeRequest.maxLength=(atob_radius-b_radius)*2;
		CCLog("m_radius:%f,atob:%f",b_radius,atob_radius);
		_world->CreateJoint(&ropeRequest);
	}
}

void HelloWorld::createBridge() 
		{
			//设置第一节刚体开始的位置
			float initx = 100;
			float inity = 250;
				//两个节点之间的间距
			float gapBetweenAnchor = 50;
			//定义节点
			b2Vec2 anchor;
			//当前刚体
			b2Body *body;
			//前一个刚体
			b2Body *preBody;
			//在for循环之外预先定义第一个刚体，定义成GetGroundBody，固定锁链的起点
			//preBody = _world->GetGroundBody();
			preBody= B2EasyBox2D::createBox(_world, initx+25, inity, gapBetweenAnchor, 10,false);
			//定义关节
			b2RevoluteJointDef revoluteJoint;
			int i = 1;
			for (i=1; i < 6; i++) {
				//循环创建刚体，它们的x坐标都基于节点的位置向右偏移15个像素(也就是刚体的半宽)
				body = B2EasyBox2D::createBox(_world, initx + i * gapBetweenAnchor+25, inity, gapBetweenAnchor, 10,false);
				//设置节点的坐标
				anchor.Set((initx + i * gapBetweenAnchor) / PTM_RATIO, inity / PTM_RATIO);
				//初始化关节
				revoluteJoint.Initialize( preBody, body, anchor);
				revoluteJoint.collideConnected=true;
		revoluteJoint.lowerAngle=CC_DEGREES_TO_RADIANS(-15);
		revoluteJoint.upperAngle=CC_DEGREES_TO_RADIANS(15);
				_world->CreateJoint(&revoluteJoint);
				//设置preBody引用当前的刚体
				preBody = body;
			}
			//设置最后一个节点
			//anchor.Set((initx + i * 50) / PTM_RATIO, inity / PTM_RATIO);
			//将最后一个节点链接到GetGroundBody，固定锁链的终点
		//	revoluteJoint.Initialize( preBody, world.GetGroundBody(), anchor);			
			//world.CreateJoint(revoluteJoint).GetAnchorB().x;
}


void HelloWorld::ccTouchesBegan(CCSet *touches,CCEvent *event){
	
if (_mouseJoint!=NULL) return;

CCTouch *myTouch =(CCTouch*) touches->anyObject();
CCPoint location = myTouch->getLocationInView();
location = CCDirector::sharedDirector()->convertToGL(location);
b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

if (resinBallBody->_ballBody->GetFixtureList()->TestPoint(locationWorld)) {
b2MouseJointDef md;
md.bodyA = _groundBody;
md.bodyB =resinBallBody->_ballBody;
md.target =resinBallBody->_ballBody->GetWorldCenter();
md.collideConnected =true;
md.maxForce =1000.0f* resinBallBody->_ballBody->GetMass();
_mouseJoint = (b2MouseJoint *)_world->CreateJoint(&md);
resinBallBody->_ballBody->SetAwake(true);
}
}
 

void HelloWorld::ccTouchesMoved(CCSet *touches,CCEvent *event) {

if (_mouseJoint == NULL) return;

CCTouch *myTouch =(CCTouch*) touches->anyObject();
CCPoint location = myTouch->getLocationInView();
location = CCDirector::sharedDirector()->convertToGL(location);
b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

_mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesCancelled(CCSet *touches, CCEvent *event) {

if (_mouseJoint) {
_world->DestroyJoint(_mouseJoint);
_mouseJoint = NULL;
}

}

void HelloWorld::ccTouchesEnded(CCSet *touches, CCEvent *event) {
if (_mouseJoint) {
_world->DestroyJoint(_mouseJoint);
_mouseJoint = NULL;
} 
}

void HelloWorld::didAccelerateFor(b2Body* body,double x,double y,double z){
	float32 u=0.1f;//body->GetFixtureList()->GetFriction();//body实心圆的摩擦系数
	float32 g=10;//重力系数
	float32 G=150;//body->GetMass()*g;//body的重力（牛顿N）
	CCLog("resinball.G:%f",G);
	float32 zz=b2Vec2(x,y).Length();
	float32 F=G*zz;//动力
	float32 f=fabs(u*G*z);//摩擦力
	float FF=F-f;//总力(N)
	b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
	if(body->GetLinearVelocity().Length()>0||FF>0){//body有速度，或者总力为动力
		if(body->GetLinearVelocity().Length()<MAX_VELOCITY||FF<0)//如果没有达到最大速度或者总力为摩擦阻力
			body->ApplyForceToCenter(force);//则把总力施加到body上
	}
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue ){
	/*b2Vec2 gravity=b2Vec2((float32)(pAccelerationValue->x * 30), (float32)(pAccelerationValue->y *30));
	//_world->SetGravity(gravity);
	_body1->ApplyForceToCenter(gravity);*/
	double x=pAccelerationValue->x;
	double y=pAccelerationValue->y;
	double z=pAccelerationValue->z;
	
	std::ostringstream strs;
strs<<x;
std::string str = "x:"+strs.str();
std::ostringstream strs2;
	strs2<<y;
	str=str+",y:"+strs2.str();
	std::ostringstream strs3;
	strs3<<z;
	str+=",z:"+strs3.str();
	showXYZ=str.c_str();//获得x,y,z的值
	

	this->resinBallBody->accelerateCallBack(pAccelerationValue);
	/*this->didAccelerateFor(_body1,x,y,z);

	float32 u=0.6f;//body实心圆的摩擦系数
		float32 g=10;//重力系数
		float32 G=1;//_blocks[0]->GetMass()*g;//body的重力（牛顿N）
	CCLog("_blocks.G:%f",G);
	for(int i=0;i<_blocks.size();i++){
		float32 zz=b2Vec2(x,y).Length();
		float32 F=G*zz;//动力
		float32 f=fabs(u*G*z);//摩擦力
		/*float FF=F-f;//总力(N)
		//b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
		b2Vec2 nowVelocity=_blocks[i]->GetLinearVelocity();
		float32 V=nowVelocity.Length();
		CCLog("V:%f",V);
		b2Vec2 force=b2Vec2(-nowVelocity.x*f/V,-nowVelocity.y*f/V);
		if(V>0){
			_blocks[i]->ApplyForceToCenter(force);
		}else _blocks[i]->ApplyForceToCenter(b2Vec2(0,0));
	}*/
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}


void HelloWorld::tick(float dt){
	for(int i=0;i<_insects.size();i++){
		_insects[i]->tick(dt);
	}
	_world->Step(dt, 10, 10); 
	for(b2Body *b = _world->GetBodyList(); b; b=b->GetNext()) { 
		if (b->GetUserData() != NULL) {
			CCSprite *sprite = (CCSprite *)b->GetUserData(); 
			sprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO,
			b->GetPosition().y * PTM_RATIO));
			sprite->setRotation(-1* CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}
void HelloWorld::changeXYZShow(float dt){	
	_pLabel->setString(showXYZ);
}