#ifndef TANK_H
#define TANK_H

#include "cocos2d.h"
class GameLayer;
class BulletSprite;
USING_NS_CC;

enum kTankState{
	kUP = 0,
	kDown,
	kLeft,
	kRight,
	kFire,
	kFiring,
};

class TankSprite : public CCSprite{
public:
	TankSprite();
	~TankSprite();
	static TankSprite* createWithLayer(GameLayer* layer, const char* image);
	CREATE_FUNC(TankSprite);
	void onFire(CCObject* sender);
	void initBullet();
	void finishFire();

	void explodeAt(CCPoint explodePoint);

	void setState(kTankState state){
		this->state = state;
	}
	const kTankState getState() const{
		return this->state;
	}

	void setGameLayer(GameLayer* layer){
		gameLayer = layer;
	}

	void update(float dt);

	void hitPlayer( CCPoint bulletPos );

	void loadLoserScene();

	void hitBrick( CCPoint bulletPos );

	void hitEnemy( CCPoint bulletPos );

	CCPoint checkAround( CCPoint coordPos, unsigned int gid );

	void setEnemy(bool isEnemy){
		this->isEnemy = isEnemy;
	}
	const bool getEnemy() const{
		return isEnemy;
	}

	void tankInit();

	void randomAction();
	void moveDown();
	void moveLeft();
	void moveRight();

	void hitHome(CCPoint bulletPos);

	void setPlayerHP(int hp){
		this->playerHP = hp;
	}

private:
	GameLayer* gameLayer;
	kTankState state;
	BulletSprite* bulletSprite;
	bool isEnemy;
	CCSprite* explodeSprite;
	bool firing;

	const static int gameWidth = 760;
	const static int gameHeight = 600;
	const static int fireFrequency = 4;
	int playerHP;

};

#endif