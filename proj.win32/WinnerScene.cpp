#include "WinnerScene.h"
USING_NS_CC;

bool WinnerScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));


		CCLabelTTF* label = CCLabelTTF::create("YOU WIN", "Arial", 30);
		CC_BREAK_IF(!label);
		label->setColor(ccc3(0, 0, 0));

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		label->setPosition(ccp(size.width / 2, size.height / 2));
		this->addChild(label);

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