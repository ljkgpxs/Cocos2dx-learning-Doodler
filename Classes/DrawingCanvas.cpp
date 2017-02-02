#include "DrawingCanvas.h"

using namespace cocos2d;

bool DrawingCanvas::init()
{
	if(!Node::init())
		return false;
	drawNode = DrawNode::create();
	background = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(background);
	this->addChild(drawNode);
	return true;
}

void DrawingCanvas::onEnter()
{
	Node::onEnter();
	Size visibleSize = CCDirector::getInstance()->getVisibleSize();
	this->setContentSize(visibleSize);
	drawNode->setContentSize(visibleSize);
	this->setupTouchHandling();
}
void DrawingCanvas::setupTouchHandling()
{
	static Vec2 lastTouchPos;
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch *touch, Event *event)
	{
		lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
		return true;
	};

	touchListener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
	//	CCLOG("Touch Moved! %f %f %f %f", touchPos.x, touchPos.y, lastTouchPos.x, lastTouchPos.y);
		drawNode->drawSegment(lastTouchPos, touchPos, 4.0, Color4F::BLACK);
		lastTouchPos = touchPos;

	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

