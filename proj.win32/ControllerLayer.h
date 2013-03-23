#ifndef CONTROLLER_LAYER_H
#define CONTROLLER_LAYER_H
#include "cocos2d.h"
class GameLayer;

class ControllerLayer : public cocos2d::CCLayer{
public:
	ControllerLayer();
	virtual bool init();
	CREATE_FUNC(ControllerLayer);
	void onFire(CCObject* sender);

	void setGameLayer(GameLayer* layer){
		this->gameLayer = layer;
	}
	
	cocos2d::CCLabelBMFont* getPlayerNum(){
		return playerNum;
	}
	cocos2d::CCLabelBMFont* getEnemyNum(){
		return enemyNum;
	}

	void setPlayerNum(const char* num){
		playerNum->setString(num);
	}
	void setEnemyNum(const char* num){
		enemyNum->setString(num);
	}

private:
	cocos2d::CCMenuItemImage* fireButton;
	cocos2d::CCLabelBMFont* playerNum;
	cocos2d::CCLabelBMFont* enemyNum;
	GameLayer* gameLayer;
};

#endif