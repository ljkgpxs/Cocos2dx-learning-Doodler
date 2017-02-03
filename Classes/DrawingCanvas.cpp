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
	static Vec2 lastTouchPos[MAX_TOUCH_POINTS];
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = [&](const std::vector<Touch *> &touches, Event *event)
	{
		for(auto touch : touches) {
			drawNode->drawDot(drawNode->convertTouchToNodeSpace(touch), 4.0f, Color4F::BLACK);
			lastTouchPos[touch->getID()] = drawNode->convertTouchToNodeSpace(touch);
		}
		return true;
	};

	touchListener->onTouchesMoved = [&](const std::vector<Touch *> &touches, Event *event)
	{
		Vec2 touchPos;
		for(auto touch : touches) {
			touchPos = drawNode->convertTouchToNodeSpace(touch);
			//CCLOG("Touch Moved! %f %f %f %f", touchPos.x, touchPos.y, lastTouchPos.x, lastTouchPos.y);
			drawNode->drawSegment(lastTouchPos[touch->getID()], touchPos, 4.0, Color4F::BLACK);
			lastTouchPos[touch->getID()] = touchPos;
		}

	};

	touchListener->onTouchesCancelled = [&](const std::vector<Touch *> &touches, Event *event)
	{

	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

