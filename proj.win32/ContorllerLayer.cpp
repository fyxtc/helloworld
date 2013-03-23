#include "ControllerLayer.h"
#include "GameLayer.h"
USING_NS_CC;

void ControllerLayer::onFire(CCObject* sender){
	gameLayer->onFire(sender);
}

bool ControllerLayer::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!CCLayer::init());

		//fire button use itemimgae is to selected effect
		fireButton = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", "CloseSelected.png", this, menu_selector(ControllerLayer::onFire));
		CC_BREAK_IF(!fireButton);
		fireButton->setPosition(ccp(420, 60));

		CCSize size = CCDirector::sharedDirector()->getWinSize();


		//it's too bad, i use menuimage present icon
		CCSprite* playerIcon = CCSprite::create("tank.png");
		playerIcon->setScale(0.6f);
		playerIcon->setPosition(ccp(playerIcon->getContentSize().width / 2, size.height - playerIcon->getContentSize().height / 2));

		CCSprite* enemyIcon = CCSprite::create("enemyIcon.png");
		enemyIcon->setScale(0.6f);
		enemyIcon->setPosition(ccp(size.width - enemyIcon->getContentSize().width / 2 - 20, size.height - enemyIcon->getContentSize().height / 2));


		CCMenu* menu = CCMenu::create(fireButton, NULL);
		menu->setPosition(CCPointZero);
		CC_BREAK_IF(! menu);

		playerNum = CCLabelBMFont::create("3", "font09.fnt");
		CCPoint playerNumPos = ccp(playerIcon->getPosition().x + 20, playerIcon->getPosition().y);
		playerNum->setPosition(playerNumPos);
		playerNum->setScale(0.7f);

		enemyNum = CCLabelBMFont::create("8", "font09.fnt");
		CCPoint enemyNumPos = ccp(enemyIcon->getPosition().x + 20, enemyIcon->getPosition().y);
		enemyNum->setPosition(enemyNumPos);
		enemyNum->setScale(0.7f);

		this->addChild(menu);
		this->addChild(playerIcon);
		this->addChild(enemyIcon);
		this->addChild(playerNum);
		this->addChild(enemyNum);


		bRet = true;
	}while(0);
	return bRet;
}

ControllerLayer::ControllerLayer() : gameLayer(NULL){

}

