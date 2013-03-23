#include "LoserScene.h"

USING_NS_CC;

bool LoserScene::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255, 0, 0, 0)));


		/*CCLabelTTF* label = CCLabelTTF::create("YOU LOSE", "Arial", 30);
		CC_BREAK_IF(!label);
		label->setColor(ccc3(0, 0, 0));*/

		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* gameover = CCSprite::create("gameover.png");
		CC_BREAK_IF(!gameover);
		gameover->setPosition(ccp(size.width / 2, size.height / 2));
		this->addChild(gameover);

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