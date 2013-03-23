#include "LoserScene.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;

bool LoserScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255, 0, 0, 0)));


		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* gameover = CCSprite::create("gameover.png");
		CC_BREAK_IF(!gameover);
		gameover->setPosition(ccp(size.width / 2, size.height / 2));
		this->addChild(gameover);

		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("home.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("home.wav");

		CCMenuItemFont* again = CCMenuItemFont::create("Play Again", this, menu_selector(LoserScene::loadNewGame));
		again->setPosition(ccp(size.width - 80, 40));
		again->setFontSize(22);

		CCMenu* menu = CCMenu::create(again, NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu);

		bRet = true;
	}while(0);

	return bRet;
}

CCScene* LoserScene::scene(){
	CCScene* scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		LoserScene* loserLayer = LoserScene::create();
		CC_BREAK_IF(!loserLayer);

		scene->addChild(loserLayer);

	}while(0);
	return scene;
}

void LoserScene::loadNewGame( CCObject* sender )
{
	CCDirector::sharedDirector()->replaceScene(GameLayer::scene());
}
