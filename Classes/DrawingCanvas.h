#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define MAX_TOUCH_POINTS	10

class DrawingCanvas : public cocos2d::Node
{
public:
	CREATE_FUNC(DrawingCanvas);
protected:
	cocos2d::DrawNode *drawNode;
	cocos2d::LayerColor *background;
	cocos2d::Sprite *check;
	cocos2d::Color4F selectedColor;

	bool init() override;
	void onEnter() override;
	void setupTouchHandling();
	void setupMenu();
	void clearPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType event);
	void backPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType event);
	void colorPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType event);
};

