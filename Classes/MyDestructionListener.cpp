  
#include "MyDestructionListener.h"  

using namespace cocos2d;
  
MyDestructionListener::MyDestructionListener() {  
      
}  
  
MyDestructionListener::~MyDestructionListener() {  
      
}  
  
void MyDestructionListener::SayGoodbye(b2Joint* joint) {  
    CCLog("joint be destroyed!");  
}  
  
  
void MyDestructionListener::SayGoodbye(b2Fixture* fixture) {  
    CCLog("fixture be destroyed!");  
} 