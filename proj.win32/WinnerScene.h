#ifndef WINNER_SCENE_H
#define WINNER_SCENE_H

#include "cocos2d.h"

class WinnerScene : public cocos2d::CCLayerColor{
public:
	static cocos2d::CCScene* scene();
	virtual bool init();
	CREATE_FUNC(WinnerScene);
};

#endif