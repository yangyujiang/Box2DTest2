#include "ResinBall.h"

#include "B2EasyBox2D.h"

#define PTM_RATIO 32.0f //Box2D世界与屏幕坐标的换算比例，1米=32像素点

using namespace cocos2d;

const float32 LIMIT_ANGLE=CC_DEGREES_TO_RADIANS(30);//限制旋转关节的旋转角度
const float32 BALL_FRICTION=0.1f;//实心球与地面的摩擦系数
const float32 BALL_GRAVITY=150;//实心球重力（N）,固定不变
const float32 BLOCK_FRICTION=0.6f;//每一个小方块与地面的摩擦系数
const float32 BLOCK_GRAVITY=0.8f;//每一个小方块的重力（N），固定不变
const float32 g=10;//模拟重力系数
const float32 MAX_VELOCITY=18;//主角树脂球的最大速度（米/秒）

ResinBall::ResinBall(void)
{
}


ResinBall::~ResinBall(void)
{
}







ResinBallBody::ResinBallBody():_ballBody(NULL),_world(NULL),_position(ccp(0,0))
	,_currentBlockHeight(0),_currentBlockWidth(0)
{

}

ResinBallBody::~ResinBallBody(){
	delete _world;
	_world=NULL;
}

bool ResinBallBody::init(b2World* world){
	bool pRet=false;
	do{
		_world=world;
		
		const float32 radius=20;//实心圆半径
		const float32 magnification=1.5f;//环形链半径比圆圆环的放大倍数

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float centerX = winSize.width / 2.f;
	float centerY = winSize.height / 2.f;
	_position=ccp(centerX,centerY);
	CCLog("%f,%f",_position.x,_position.y);
	//this->createRigidCircle(24,radius*1.1f);
	
	_ballBody=B2EasyBox2D::createCircle(_world,centerX,centerY,radius,false,NULL,10.0f,0.8f,0,false);
	_ballBody->SetBullet(true);
	/*_body1=B2EasyBox2D::createCircle(_world,centerX-radius+3*radius/5.0f,centerY,radius*3/5.0f,false,NULL,10.0f,0.8f,0,false);
	_body1->SetBullet(true);
	B2EasyBox2D::createCircle(_world,centerX+radius-2*radius/5.0f,centerY,2*radius/5.0f,false,NULL,0.0001f,0.0f,0.0f,false)->SetBullet(true);
*/
	this->createCircleBridge(20,magnification*radius);

		pRet=true;
	}while(0);
	return pRet;
}


b2Joint* createRevoluteJoint(b2World* world,b2Body* body1,b2Body* body2,b2Vec2 anchor){//根据两个body和一个节点创建一个旋转关节并返回
		//定义关节需求
		b2RevoluteJointDef revoluteJoint;
		revoluteJoint.Initialize( body1, body2, anchor);
		revoluteJoint.collideConnected = false;//此关节的两个刚体是否碰撞检测
		revoluteJoint.lowerAngle=-LIMIT_ANGLE;
		revoluteJoint.upperAngle=LIMIT_ANGLE;
		revoluteJoint.enableLimit=true;
		//开启马达
		revoluteJoint.enableMotor = true;
		//设置马达的最大角速度，单位为 弧度/秒，如设置为Math.PI，即每秒钟转180度
		revoluteJoint.motorSpeed =1;
		revoluteJoint.maxMotorTorque = 500;//设置最大的扭力值
		return world->CreateJoint(&revoluteJoint);
}
void ResinBallBody::createCircleBridge(int number,float32 radius){//根据圆环链半径和所需小方块数量创建圆环
	double alta=b2_pi/number;
	double r_small=radius*cos(alta);//radius为以节点组成的圆的半径，r_small为以小方块中心点组成的圆
	//根据半径和个数计算线段的长度
	double segmentHeight = 2.f*radius * sin(alta);//每个小方块的长度
	double segmentWidth =segmentHeight*0.3f;//每个小方块的宽度
	_currentBlockHeight=segmentHeight;
	_currentBlockWidth=segmentWidth;
	int i=0;
	//计算每个线段的角度、坐标
	double angle = i*1.f/ number *b2_pi*2;//从右侧最中间的位置起，即VisibleRect.right
	double bx = r_small * cos(angle)+_position.x;
	double by = r_small * sin(angle)+_position.y;
	
	b2Body *firstBody=B2EasyBox2D::createBox(_world,bx,by,segmentWidth,segmentHeight,false,angle,1.0f,1.f,0.0f);
	_blocks.push_back(firstBody);//存入_blocks数组中

	b2Body *body;//当前body
	b2Body *preBody=firstBody;//上一个body

	//for循环创建number个线段，合成圆形边界
	for (i = 1; i < number; i++) {
		//计算每个线段的角度、坐标
		angle = i*1.f/number *b2_pi*2;
		float bx = r_small * cos(angle)+_position.x;
		float by = r_small * sin(angle)+_position.y;

		//创建有方向的矩形刚体，合成总的圆形刚体
		body=B2EasyBox2D::createBox(_world,bx,by,segmentWidth,segmentHeight,false,angle,1.0f,1.f,0.0f);
		_blocks.push_back(body);//存入_blocks数组中

		//定义节点，两小方块相交处点坐标的公式为（r*cos(0.5*2*pi/n+k*2*pi/n),r*sin(0.5*2*pi/n+k*2*pi/n)）+centerXY
		b2Vec2 anchor=b2Vec2((radius*cos((2*i-1)*alta)+_position.x)/PTM_RATIO,(radius*sin((2*i-1)*alta)+_position.y)/PTM_RATIO);		
		b2Joint* revoluteJoint=createRevoluteJoint(_world,preBody,body,anchor);//创建旋转关节
		
		_blockJoints.push_back(revoluteJoint);//将旋转关节加入到_blockJoint数组中
		
		preBody = body;//设置preBody引用当前的刚体
	}//for
	//定义节点
	b2Vec2 anchor=b2Vec2((radius*cos((2*i-1)*alta)+_position.x)/PTM_RATIO,(radius*sin((2*i-1)*alta)+_position.y)/PTM_RATIO);
	b2Joint* revoluteJoint=createRevoluteJoint(_world,body,firstBody,anchor);//将最后一个节点链接到firstBody
		
	_blockJoints.push_back(revoluteJoint);
}


void ResinBallBody::accelerateCallBack(CCAcceleration* pAccelerationValue){//加速器变化时（倾斜手机屏幕）的回调函数
	double x=pAccelerationValue->x;
	double y=pAccelerationValue->y;
	double z=pAccelerationValue->z;

	//实心球的重力、摩擦力模拟
	float32 zz=b2Vec2(x,y).Length();
	float32 F=BALL_GRAVITY*zz;//动力
	float32 f=fabs(BALL_FRICTION*BALL_GRAVITY*z);//摩擦力
	float FF=F-f;//总力(N)
	b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
	if(_ballBody->GetLinearVelocity().Length()>0||FF>0){//body有速度，或者总力为动力
		if(_ballBody->GetLinearVelocity().Length()<MAX_VELOCITY||FF<0)//如果没有达到最大速度或者总力为摩擦阻力
			_ballBody->ApplyForceToCenter(force);//则把总力施加到body上
	}
	
	//每个小方块的摩擦力的模拟
	for(int i=0;i<_blocks.size();i++){
		float32 zz=b2Vec2(x,y).Length();
		float32 F=BLOCK_GRAVITY*zz;//动力
		float32 f=fabs(BLOCK_FRICTION*BLOCK_GRAVITY*z);//摩擦力
		/*float FF=F-f;//总力(N)*/
		//b2Vec2 force=b2Vec2(x*fabs(FF)/zz,y*fabs(FF)/zz);
		b2Vec2 nowVelocity=_blocks[i]->GetLinearVelocity();
		float32 V=nowVelocity.Length();//当前小方块的速度
		b2Vec2 force=b2Vec2(-nowVelocity.x*f/V,-nowVelocity.y*f/V);//得到摩擦力向量与当前速度反向,所以加负号
		if(V>0){//如果当前速度不为0才施加反向摩擦力；否则不施加力
			_blocks[i]->ApplyForceToCenter(force);
		}else _blocks[i]->ApplyForceToCenter(b2Vec2(0,0));
	}
}

void ResinBallBody::shrinkResinBallBody(float32 scale){
	_ballBody->GetFixtureList()->GetShape()->m_radius=0.1;

	vector<b2Vec2> anchors;
	for(int i=0;i<_blocks.size();i++){
	B2EasyBox2D::shrinkBox(_blocks[i],_currentBlockHeight,_currentBlockWidth,0.5f);
	b2Vec2 currentPosition=_blocks[i]->GetWorldCenter();
	b2Vec2 center=b2Vec2(_position.x/PTM_RATIO,_position.y/PTM_RATIO);
	b2Vec2 newPosition=b2Vec2(center.x+(currentPosition.x-center.x)*scale,center.y+(currentPosition.y-center.y)*scale);
	_blocks[i]->SetTransform(newPosition,0);

	b2Vec2 position=b2Vec2(center.x+(_blockJoints[i]->GetAnchorA().x-center.x)*scale,
		center.y+(_blockJoints[i]->GetAnchorA().y-center.y)*scale);
	anchors.push_back(position);
	}
	for(int i=0;i<_blockJoints.size();i++){
		int nexti=i+1;
		if(nexti==_blockJoints.size()) nexti=0;
	//	b2Joint* newJoint=createRevoluteJoint(_world,_blocks[i],_blocks[nexti],anchors[i]);
		_world->DestroyJoint(_blockJoints[i]);
		//_blockJoints[i]=newJoint;
	}
	
}