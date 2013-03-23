#ifndef BULLET_SPRITE_H
#define BULLET_SPRITE_H

#include "cocos2d.h"
USING_NS_CC;
class GameLayer;

class BulletSprite : public CCSprite{
public:
	static BulletSprite* createWithLayer(GameLayer* layer);
	CREATE_FUNC(BulletSprite);
};

#endif