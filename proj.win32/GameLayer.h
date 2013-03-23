#ifndef GAME_LAYER_H
#define GAME_LAYER_H

#include "cocos2d.h"
class TankSprite;
USING_NS_CC;

class GameLayer : public CCLayer{
public:
	virtual bool init();
	static CCScene* scene();
	CREATE_FUNC(GameLayer);
	GameLayer();
	~GameLayer();
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);

	void keepInBound( CCPoint &tankPos, CCSize &size );

	void registerWithTouchDispatcher(){
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	}
	void setTankPosition(CCPoint pos, TankSprite* tankSprite);
	CCPoint tileCoordForPosition(CCPoint pos);

	bool isCollided(CCPoint tileCoord);
	void onFire(CCObject* sender);
	CCTMXLayer* getForeground(){
		return foreground;
	}
	CCTMXTiledMap* getMap(){
		return map;
	}
	CCArray* getEnemyList(){
		return enemyList;
	}

	TankSprite* getTankSprite(){
		return tankSprite;
	}

	void playHitBrickSound();
	void playHitTankSound();
	void playFireSound();
	
	void doRandomAction(float dt);



	const static int ContorllerLayerTag = 99;
private:
	CCTMXLayer* background;
	CCTMXLayer* foreground;
	CCTMXTiledMap* map;
	TankSprite* tankSprite;
	CCArray* enemyList;

	void setViewpointCenter(CCPoint pos);

};


#endif