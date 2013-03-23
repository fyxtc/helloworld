#include "TankSprite.h"
#include "GameLayer.h"
#include "BulletSprite.h"
#include "WinnerScene.h"
#include "LoserScene.h"
#include "ControllerLayer.h"
USING_NS_CC;





void TankSprite::randomAction(){
	//tankInit();
	switch(rand() % 4){
	case 0:
	case 1:
		moveDown();
		break;
	case 2:
		moveLeft();
		break;
	case 3:
		moveRight();
		break;
	default:
		CCLog("invalid move");
	}

	//if tank has fired one bullet, now can't fire another
	if((rand() % fireFrequency == 0) && (!bulletSprite->isVisible())){
		this->onFire(this);
	}
}



void TankSprite::moveDown(){
	CCPoint tankPos = this->getPosition();
	this->setRotation(180.0f);
	tankPos.y -= gameLayer->getMap()->getTileSize().height;
	tankPos.y = MAX(getTexture()->getContentSize().height, tankPos.y);
	this->setState(kDown);
	gameLayer->setTankPosition(tankPos, this);
}

void TankSprite::moveLeft(){
	CCPoint tankPos = this->getPosition();
	this->setRotation(-90.0f);
	tankPos.x -= gameLayer->getMap()->getTileSize().width;
	tankPos.x = MAX(getTexture()->getContentSize().width, tankPos.x);
	this->setState(kLeft);
	gameLayer->setTankPosition(tankPos, this);
}

void TankSprite::moveRight(){

	CCPoint tankPos = this->getPosition();
	this->setRotation(90.0f);
	tankPos.x += gameLayer->getMap()->getTileSize().width;
	tankPos.x = MIN(tankPos.x, gameWidth - getTexture()->getContentSize().width / 2);

	this->setState(kRight);
	gameLayer->setTankPosition(tankPos, this);
}

void TankSprite::tankInit(){
	//unsigned int gameHeight = gameLayer->getMap()->getMapSize().height * gameLayer->getMap()->getTileSize().height;
	this->state = kDown;
}

void TankSprite::update(float dt){
	

	CCPoint bulletPos = bulletSprite->getPosition();
	CCPoint bulletPosUpDownFix = ccp(bulletPos.x -2, bulletPos.y);
	CCPoint bulletPosLeftRightFix = ccp(bulletPos.x, bulletPos.y + 2);

	if(!this->isEnemy){

		//check whether player hit the enemy
		hitEnemy(bulletPos);
	}else{
		//check whether enemy hit the player
		hitPlayer(bulletPos);

	}

	//check whether hit the brick or home
	hitBrick(bulletPos);
	hitHome(bulletPos);

	//在up/down的情况下，左边的有误差打不着，left/right情况下，上边打不着，需要人工辅助判断....
	if(state == kUP || state == kDown){
		hitBrick(bulletPosUpDownFix);
		hitHome(bulletPosUpDownFix);
	}else{
		hitBrick(bulletPosLeftRightFix);
		hitHome(bulletPosLeftRightFix);
	}
	
	

}

TankSprite* TankSprite::createWithLayer(GameLayer* layer, const char* image){
	CCSpriteBatchNode* spriteBatch = CCSpriteBatchNode::create(image);
	TankSprite* tankSprite = TankSprite::create();
	if(tankSprite){
		tankSprite->initWithTexture(spriteBatch->getTexture(), CCRectMake(0, 0, 40, 40));
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		tankSprite->setPosition(ccp(tankSprite->getTexture()->getContentSize().width/2, tankSprite->getTexture()->getContentSize().height/2));
		spriteBatch->addChild(tankSprite);
		//spriteBatch->setPosition(ccp(tankSprite->getTexture()->getContentSize().width/2, tankSprite->getTexture()->getContentSize().height/2));
		layer->addChild(spriteBatch, 1);
		tankSprite->setGameLayer(layer);
		return tankSprite;
	}
	CC_SAFE_DELETE(spriteBatch);
	CC_SAFE_DELETE(tankSprite);
	return NULL;
}

void TankSprite::onFire(CCObject* sender){
	//gameLayer->getFireButton()->setVisible(false);
	
	
	if(firing){
		return;
	}
	firing = true;
	if(!isEnemy){
		gameLayer->playFireSound();
	}
	
	CCPoint tankPos = this->getPosition();
	CCSize tankSize = this->getTexture()->getContentSize();
	//CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint bulletPos, bulletTarget;

	switch(this->getState()){
	case kUP:
		bulletPos = ccp(tankPos.x, tankPos.y + tankSize.height / 2);
		bulletTarget = ccp(tankPos.x, tankPos.y + gameHeight);
		break;
	case kDown:
		bulletPos = ccp(tankPos.x, tankPos.y - tankSize.height / 2);
		bulletTarget = ccp(tankPos.x, tankPos.y - gameHeight);
		break;
	case kLeft:
		bulletPos = ccp(tankPos.x - tankSize.width / 2, tankPos.y);
		bulletTarget = ccp(tankPos.x - gameWidth, tankPos.y);
		break;
	case kRight:
		bulletPos = ccp(tankPos.x + tankSize.width / 2, tankPos.y);
		bulletTarget = ccp(tankPos.x + gameWidth, tankPos.y);
		break;
	default:
		CCLog("invalid fire");
	}
	bulletTarget.x = MAX(0, bulletTarget.x);
	bulletTarget.x = MIN(bulletTarget.x, gameWidth);
	bulletTarget.y = MAX(0, bulletTarget.y);
	bulletTarget.y = MIN(bulletTarget.y, gameHeight);

	float moveTime;
	moveTime = MAX(abs(bulletTarget.x - bulletPos.x), abs(bulletTarget.y - bulletPos.y)) / 200.0f;


	bulletSprite->setPosition(bulletPos);
	bulletSprite->setVisible(true);
	CCAssert((bulletPos.x == bulletTarget.x) || (bulletPos.y == bulletTarget.y), "no line" );
	this->scheduleUpdate();
	bulletSprite->runAction(CCSequence::create(CCShow::create(), CCMoveTo::create(moveTime, bulletTarget),CCCallFunc::create(this,callfunc_selector(TankSprite::finishFire)), NULL));

}

void TankSprite::initBullet(){
	bulletSprite = BulletSprite::createWithLayer(gameLayer);
	bulletSprite->retain();
}

void TankSprite::finishFire(){
	
	bulletSprite->setVisible(false);
	this->unscheduleUpdate();
	firing = false;
	
}

TankSprite::TankSprite() : gameLayer(NULL), state(kUP), bulletSprite(NULL), firing(false), playerHP(0){

}

TankSprite::~TankSprite(){
	if(bulletSprite){
		bulletSprite->release();
		bulletSprite = NULL;
	}
	
}

void TankSprite::hitEnemy( CCPoint bulletPos )
{

	CCObject* obj;
	TankSprite* enemy;
	CCSize tankSize;
	CCPoint tankPos;
	CCArray* enemyList = gameLayer->getEnemyList();
	CCARRAY_FOREACH(enemyList, obj){
		enemy = dynamic_cast<TankSprite*>(obj);
		CCAssert(enemy != NULL, "enemy is null");
		if(enemy->isVisible()){
			tankSize = enemy->getContentSize();
			tankPos = enemy->getPosition();
			CCRect rect = CCRectMake(tankPos.x - tankSize.width, tankPos.y - tankSize.height, tankSize.width, tankSize.height);
			if(rect.containsPoint(bulletPos)){

				gameLayer->playHitTankSound();

				enemy->stopAllActions();
				bulletSprite->setVisible(false);
				bulletSprite->stopAllActions();
				CCTintBy *ac1 = CCTintBy::create(0.2f, 0, 255, 255);
				CCHide *ac2 = CCHide::create();
				explodeAt(tankPos);
				enemy->runAction(CCSequence::create(ac1, ac2, NULL));
				enemyList->removeObject(enemy);

				firing = false;
				this->unscheduleUpdate();

				int count = enemyList->count();
				if(count == 0){
					CCDirector::sharedDirector()->replaceScene(WinnerScene::scene());
				}else{
					ControllerLayer* layer = dynamic_cast<ControllerLayer*>(gameLayer->getParent()->getChildByTag(GameLayer::ContorllerLayerTag));
					layer->setEnemyNum(CCString::createWithFormat("%d", count)->getCString());

				}

			}
		}

	}

}

void TankSprite::hitBrick( CCPoint bulletPos )
{
	CCPoint coordPos = gameLayer->tileCoordForPosition(bulletPos);
	unsigned int gid = gameLayer->getForeground()->tileGIDAt(coordPos);
	if(gid == 2){
		//this->explodeAt(bulletPos);
		if(!isEnemy){
			gameLayer->playHitBrickSound();
		}
		
		bulletSprite->stopAllActions();
		bulletSprite->setVisible(false);
		gameLayer->getForeground()->setTileGID(4, coordPos);
		
		//TODO: check around brick
		CCPoint aroundPos = ccp(0, 0);
		if(state == kUP || state == kDown){
			//left
			aroundPos = ccp(coordPos.x - 1, coordPos.y);
			gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
			if(gid == 2){
				gameLayer->getForeground()->setTileGID(4, aroundPos);
			}else{
				//right
				aroundPos = ccp(coordPos.x + 1, coordPos.y);
				gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
				if(gid == 2){
					gameLayer->getForeground()->setTileGID(4, aroundPos);
				}
			}

		}else{
			//up 
			aroundPos = ccp(coordPos.x, coordPos.y - 1);
			gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
			if(gid == 2){
				gameLayer->getForeground()->setTileGID(4, aroundPos);
			}else{
				//down
				aroundPos = ccp(coordPos.x, coordPos.y + 1);
				gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
				if(gid == 2){
					gameLayer->getForeground()->setTileGID(4, aroundPos);
				}
			}

		}
		firing = false;
		this->unscheduleUpdate();
	}

}

void TankSprite::hitPlayer( CCPoint bulletPos )
{

	TankSprite* player = gameLayer->getTankSprite();
	CCPoint playerPos = player->getPosition();
	CCSize size = player->getContentSize();
	CCRect rect = CCRectMake(playerPos.x - size.width, playerPos.y - size.height, size.width, size.height);
	if(rect.containsPoint(bulletPos)){
		CCLog("YOU LOSE");
		this->unscheduleUpdate();
		bulletSprite->stopAllActions();
		bulletSprite->setVisible(false);
		explodeAt(bulletPos);
		gameLayer->playHitTankSound();

		//千万不能用this，必需是player！！！否则取到的HP都是调用hitPlayer方法的敌人的
		--player->playerHP;
		if(player->playerHP < 0){
			loadLoserScene();
		}else{
			ControllerLayer* layer = dynamic_cast<ControllerLayer*>(gameLayer->getParent()->getChildByTag(GameLayer::ContorllerLayerTag));
			CCAssert(layer != NULL, "it's not contoller layer");
			layer->setPlayerNum(CCString::createWithFormat("%d", player->playerHP)->getCString());
		}
		

	}

}

void TankSprite::hitHome(CCPoint bulletPos)
{
	CCPoint coordPos = gameLayer->tileCoordForPosition(bulletPos);
	unsigned int gid = gameLayer->getForeground()->tileGIDAt(coordPos);
	if(gid == 3){
		
		CCPoint aroundPos = ccp(0, 0);
		if(state == kUP || state == kDown){
			//left
			aroundPos = ccp(coordPos.x - 1, coordPos.y);
			gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
			if(gid == 3){
				this->loadLoserScene();
			}else{
				//right
				aroundPos = ccp(coordPos.x + 1, coordPos.y);
				gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
				if(gid == 3){
					this->loadLoserScene();
				}
			}

		}else{
			//up 
			aroundPos = ccp(coordPos.x, coordPos.y - 1);
			gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
			if(gid == 3){
				this->loadLoserScene();
			}else{
				//down
				aroundPos = ccp(coordPos.x, coordPos.y + 1);
				gid = gameLayer->getForeground()->tileGIDAt(aroundPos);
				if(gid == 3){
					this->loadLoserScene();
				}
			}

		}
	}
}

void TankSprite::explodeAt(CCPoint explodePoint)
{

	explodeSprite = CCSprite::create();
	explodeSprite->setPosition(explodePoint);
	gameLayer->addChild(explodeSprite);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCAnimation* animation = CCAnimation::create();
	animation->setDelayPerUnit(0.05f);
	for(int i = 0; i != 8; ++i){
		CCSpriteFrame* frame = cache->spriteFrameByName(CCString::createWithFormat("explode%d", i)->getCString());
		animation->addSpriteFrame(frame);
	}
	CCAnimate* animate = CCAnimate::create(animation);
	explodeSprite->runAction(animate);
}

void TankSprite::loadLoserScene()
{
	gameLayer->playHitTankSound();
	CCDirector::sharedDirector()->replaceScene(LoserScene::scene());
}




