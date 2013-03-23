#ifndef LOSER_SCENE_H
#define LOSER_SCENE_H

#include "cocos2d.h"

class LoserScene : public cocos2d::CCLayerColor{
public:
	static cocos2d::CCScene* scene();
	virtual bool init();
	CREATE_FUNC(LoserScene);

private:
	void loadNewGame(CCObject* sender);
};

#endif