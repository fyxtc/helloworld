#include "BulletSprite.h"
#include "GameLayer.h"
USING_NS_CC;

BulletSprite* BulletSprite::createWithLayer(GameLayer* layer){
	BulletSprite* sprite = BulletSprite::create();
	if(sprite && sprite->initWithFile("bullet.png", CCRectMake(0, 0, 16, 16))){
		layer->addChild(sprite, 2);
		sprite->setVisible(false);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}