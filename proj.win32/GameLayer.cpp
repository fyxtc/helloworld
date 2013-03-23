#include "GameLayer.h"
#include "TankSprite.h"
#include "ControllerLayer.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;



void GameLayer::doRandomAction(float dt){
	CCObject* obj;
	TankSprite* enemy;
	CCARRAY_FOREACH(enemyList, obj){
		enemy = dynamic_cast<TankSprite*>(obj);
		CCAssert(enemy != NULL, "this is not enemy");
		enemy->randomAction();
		//this->setTankPosition(enemy->getPosition());
	}
}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
	CCPoint pos = pTouch->getLocation();
	pos = this->convertToNodeSpace(pos);

	CCPoint actualPos = tankSprite->getPosition();
	CCPoint tankPos = actualPos;
	CCSize size = tankSprite->getContentSize();

	CCPoint diff = ccpSub(pos, tankPos);
	if(abs(diff.x) > abs(diff.y)){
		
		if(diff.x > 0){
			//move right
			//tankSprite->moveRight();
			tankSprite->setRotation(90.0f);
			tankPos.x += map->getTileSize().width;
			tankSprite->setState(kRight);

		}else{
			//move left
			//tankSprite->moveLeft();
			tankSprite->setRotation(-90.0f);
			tankPos.x -= map->getTileSize().width;
			tankSprite->setState(kLeft);
		}
	}else{
		if(diff.y > 0){
			//move up
			//tankSprite->moveUp();
			tankSprite->setRotation(0.0f);
			tankPos.y += map->getTileSize().height;
			tankSprite->setState(kUP);
		}else{
			//move down
			//tankSprite->moveDown();
			tankSprite->setRotation(180.0f);
			tankPos.y -= map->getTileSize().height;
			tankSprite->setState(kDown);
		}
	}

	//keep player tank in bound
	keepInBound(tankPos, size);



	if(pos.x <= map->getMapSize().width * map->getTileSize().width 
		&& pos.y <= map->getMapSize().height * map->getTileSize().height
		&& pos.x >= 0 && pos.y >= 0){
			this->setTankPosition(tankPos, tankSprite);
	}

	this->setViewpointCenter(tankSprite->getPosition());
	return true;
}

void GameLayer::setTankPosition(CCPoint position, TankSprite* tank){

	bool step1, step2;
	CCPoint tileCoord = this->tileCoordForPosition(position);
	//如果point是在地图块十字交叉处，则返回的是右下角的坐标，因此需要多个判断
	//以后重构为状态模式....现在先完成任务...
	if(tank->getState() == kUP){
		if(--tileCoord.y < 0){
			++tileCoord.y;
		}
		step1 = this->isCollided(tileCoord);
		if(--tileCoord.x < 0){
			++tileCoord.x;
		}
		step2 = this->isCollided(tileCoord);
		if(step1 || step2){
			return;
		}
	}

	if(tank->getState() == kLeft){
		if(--tileCoord.x < 0){
			++tileCoord.x;
		}
		step1 = this->isCollided(tileCoord);
		if(--tileCoord.y < 0){
			++tileCoord.y;
		}
		step2 = this->isCollided(tileCoord);
		if(step1 || step2){
			return;
		}
	}

	if(tank->getState() == kRight){
		step1 = this->isCollided(tileCoord);
		if(--tileCoord.y < 0){
			++tileCoord.y;
		}
		step2 = this->isCollided(tileCoord);
		if(step1 || step2){
			return;
		}
	}

	if(tank->getState() == kDown){
		step1 = this->isCollided(tileCoord);
		if(--tileCoord.x < 0){
			++tileCoord.x;
		}
		step2 = this->isCollided(tileCoord);
		if(step1 || step2){
			return;
		}
	}

	//敌人和敌人不能重合，玩家和敌人也不能重合，构建所有tank的列表，依次判断
	CCArray* allTank = CCArray::createWithCapacity(10);
	CCAssert(allTank != NULL, "allTank list is null");
	allTank->initWithArray(enemyList);
	CCAssert(allTank->count() > 0, "count is 0");
	allTank->addObject(tankSprite);

	CCSize size = tank->getContentSize();
	CCPoint pos = position;
	//这里-1是很有必要的，否则40*40和40*40的碰撞，会占用太大空间了！！！
	CCRect rect = CCRectMake(pos.x- size.width / 2, pos.y - size.height / 2, size.width - 1, size.height - 1);

	CCObject* obj;
	TankSprite* anotherTank;

	CCSize anotherSize;
	CCPoint anotherPos;
	CCRect anotherRect;

	CCARRAY_FOREACH(allTank, obj){
		anotherTank = dynamic_cast<TankSprite*>(obj);
		CCAssert(anotherTank != NULL, "it's not TankSprite");

		if(tank == anotherTank){
			//如果是自己和自己判断则跳过
			continue;
		}else{
			anotherSize = anotherTank->getContentSize();
			anotherPos = anotherTank->getPosition();
			anotherRect = CCRectMake(anotherPos.x - anotherSize.width / 2, anotherPos.y - anotherSize.height / 2, anotherSize.width - 1, anotherSize.height - 1);
			if(rect.intersectsRect(anotherRect)){
				return;
			}
		}
	}

	//keep all tank tank in bound
	this->keepInBound(position, size);

	tank->setPosition(position);
}

bool GameLayer::isCollided(CCPoint tileCoord){
	int tileGID = foreground->tileGIDAt(tileCoord);

	if(tileGID){
		CCDictionary* properties = map->propertiesForGID(tileGID);
		if(properties){
			CCString* result = const_cast<CCString*>(properties->valueForKey("Brick"));
			if(result && result->isEqual(CCString::create("True"))){

				/*CCSprite* tile = foreground->tileAt(tileCoord);
				CCRect tileRect = tile->getTextureRect();
				CCPoint tankPos = tankSprite->getPosition();
				CCSize tankSize = tankSprite->getTexture()->getContentSize();

				CCRect tankRect = CCRectMake(tankPos.x - tankSize.width, tankPos.y - tankSize.height, tankSize.width, tankSize.height);
				if(tankRect.intersectsRect(tileRect)){
					return;
				}*/
				return true;
			}
			result = const_cast<CCString*>(properties->valueForKey("Home"));
			if(result && result->isEqual(CCString::create("True"))){
				return true;
			}
		}
	}
	return false;
}

CCPoint GameLayer::tileCoordForPosition(CCPoint pos){
	int x = pos.x / map->getTileSize().width;
	int y = (map->getMapSize().height * map->getTileSize().height - pos.y) / map->getTileSize().height;
	return ccp(x, y);
}

void GameLayer::setViewpointCenter(CCPoint pos){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int x = MAX(pos.x, size.width / 2);
	int y = MAX(pos.y, size.height / 2);

	x = MIN(x, map->getMapSize().width * map->getTileSize().width - size.width / 2);
	y = MIN(y, map->getMapSize().height * map->getTileSize().height - size.height / 2);

	CCPoint actualPos = ccp(x, y);
	CCPoint centerOfView = ccp(size.width / 2, size.height / 2);
	CCPoint viewPoint = ccpSub(centerOfView, actualPos);
	this->setPosition(viewPoint);
}


bool GameLayer::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayer::init());

		map = CCTMXTiledMap::create("map.tmx");
		CC_BREAK_IF(!map);
		map->retain();

		background = map->layerNamed("Background");
		CC_BREAK_IF(!background);
		background->retain();

		foreground = map->layerNamed("Foreground");
		CC_BREAK_IF(!foreground);
		foreground->retain();

		tankSprite = TankSprite::createWithLayer(this, "tank.png");
		CC_BREAK_IF(!tankSprite);
		tankSprite->retain();
		tankSprite->setEnemy(false);
		tankSprite->initBullet();
		tankSprite->setPlayerHP(3);
		

		enemyList = CCArray::createWithCapacity(8);
		CC_BREAK_IF(!enemyList);
		enemyList->retain();
		
		for(int i = 1; i != 9; ++i){
			TankSprite* enemy = TankSprite::createWithLayer(this, "enemy.png");
			enemy->setPosition(ccp(i*80 - 20, 560));
			enemy->setEnemy(true);
			enemy->initBullet();
			enemy->setState(kDown);
			enemyList->addObject(enemy);
		}

		CCPoint tileCoord = this->tileCoordForPosition(tankSprite->getPosition());
		int tileGID = foreground->tileGIDAt(tileCoord);

		//if schedule, unchedule is necessary(in TankSprite)
		this->schedule(schedule_selector(GameLayer::doRandomAction), 1.0f);
		
		this->addChild(map);

		this->setTouchEnabled(true);

		//load explode anmitaion
		for(int i = 0; i != 8; ++i){
			CCSpriteFrame* frame = CCSpriteFrame::create("explode.png", CCRectMake(23 * i, 0, 23, 23));
			CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			cache->addSpriteFrame(frame, CCString::createWithFormat("explode%d", i)->getCString());
		}

		//load bgm
		CocosDenshion::SimpleAudioEngine* audioEngine = CocosDenshion::SimpleAudioEngine::sharedEngine();
		CC_BREAK_IF(!audioEngine);
		audioEngine->preloadEffect("hit.wav");
		audioEngine->preloadEffect("Gunfire.wav");
		audioEngine->preloadEffect("Bang.wav");
		//audioEngine->preloadEffect("home.wav");
		audioEngine->playBackgroundMusic("bgm.wav", true);
		

		bRet = true;
	}while(0);
	return bRet;
}

void GameLayer::onFire(CCObject* sender){
	CCLog("fire!!!!!");
	tankSprite->onFire(this);
}

GameLayer::GameLayer() : map(NULL), background(NULL), foreground(NULL), tankSprite(NULL), enemyList(NULL){

}

GameLayer::~GameLayer(){

	/*CC_SAFE_DELETE(map);
	CC_SAFE_DELETE(background);
	CC_SAFE_DELETE(foreground);
	CC_SAFE_DELETE(tankSprite);
	CC_SAFE_DELETE(fireButton);*/

	if(map){
		map->release();
		map = NULL;
	}
	if(background){
		background->release();
		background = NULL;
	}
	if(foreground){
		foreground->release();
		foreground = NULL;
	}
	if(tankSprite){
		tankSprite->release();
		tankSprite = NULL;
	}
	if(enemyList){
		enemyList->release();
		enemyList = NULL;
	}
}


CCScene* GameLayer::scene(){
	CCScene* scene = CCScene::create();
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		GameLayer* gameLayer = GameLayer::create();
		CC_BREAK_IF(!gameLayer);

		ControllerLayer* controllerLayer = ControllerLayer::create();
		CC_BREAK_IF(!controllerLayer);
		controllerLayer->setGameLayer(gameLayer);
		

		scene->addChild(gameLayer);
		scene->addChild(controllerLayer, 2, ContorllerLayerTag);
	}while(0);
	return scene;
}

void GameLayer::keepInBound( CCPoint &tankPos, CCSize &size )
{
	tankPos.x = MAX(size.width / 2, tankPos.x);
	tankPos.y = MAX(size.height / 2, tankPos.y);
	tankPos.x = MIN(tankPos.x, map->getMapSize().width * map->getTileSize().width - size.width / 2);
	tankPos.y = MIN(tankPos.y, map->getMapSize().height * map->getTileSize().height - size.height / 2);
}

void GameLayer::playHitBrickSound()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
}

void GameLayer::playHitTankSound()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Bang.wav");
}

void GameLayer::playFireSound()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("Gunfire.wav");
}





