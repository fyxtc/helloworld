#include "WinnerScene.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"
USING_NS_CC;

bool WinnerScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255, 0, 0, 0)));


		CCLabelTTF* label = CCLabelTTF::create("YOU WIN", "Arial", 30);
		CC_BREAK_IF(!label);
		label->setColor(ccc3(0, 0, 255));


		CCSize size = CCDirector::sharedDirector()->getWinSize();
		label->setPosition(ccp(size.width / 2, size.height / 2));
		this->addChild(label);

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("winner.wav", true);

		CCMenuItemFont* again = CCMenuItemFont::create("Play Again", this, menu_selector(WinnerScene::loadNewGame));
		again->setPosition(ccp(size.width - 80, 40));
		again->setFontSize(22);

		CCMenu* menu = CCMenu::create(again, NULL);
		menu->setPosition(CCPointZero);
		this->addChild(menu);

		bRet = true;
	}while(0);

	return bRet;
}

CCScene* WinnerScene::scene(){
	CCScene* scene = NULL;
	do{
		scene = CCScene::create();
		CC_BREAK_IF(!scene);
		WinnerScene* winnerLayer = WinnerScene::create();
		CC_BREAK_IF(!winnerLayer);
		
		scene->addChild(winnerLayer);

	}while(0);
	return scene;
}

void WinnerScene::loadNewGame( CCObject* sender )
{
	CCDirector::sharedDirector()->replaceScene(GameLayer::scene());
}