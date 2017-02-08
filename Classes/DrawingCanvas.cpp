#include "DrawingCanvas.h"
#include "Constants.h"
#include "Lobby.h"

using namespace cocos2d;

bool DrawingCanvas::init()
{
	if(!Node::init())
		return false;
	drawNode = DrawNode::create();
	background = LayerColor::create(Color4B(COLOR_WHITE));
	selectedColor = COLOR_GREEN;
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
	this->setupMenu();
}
void DrawingCanvas::setupTouchHandling()
{
	static Vec2 lastTouchPos[MAX_TOUCH_POINTS];
	auto touchListener = EventListenerTouchAllAtOnce::create();
	static std::vector<float> lastRadius(MAX_TOUCH_POINTS, INITIAL_RADIUS);
	touchListener->onTouchesBegan = [&](const std::vector<Touch *> &touches, Event *event)
	{
		for(auto touch : touches) {
			drawNode->drawDot(drawNode->convertTouchToNodeSpace(touch), lastRadius[touch->getID()], selectedColor);
			lastTouchPos[touch->getID()] = drawNode->convertTouchToNodeSpace(touch);
		}
		return true;
	};

	touchListener->onTouchesMoved = [&](const std::vector<Touch *> &touches, Event *event)
	{
		Vec2 touchPos;
		for(auto touch : touches) {
			float dt = 1.0f / 100.0f, rc = 1.0f, alpha = dt / (rc + dt), radius;
			touchPos = drawNode->convertTouchToNodeSpace(touch);
			float distance = lastTouchPos[touch->getID()].distance(touchPos);
			radius = (alpha * distance) + (1.0f - alpha) * lastRadius[touch->getID()];
			//CCLOG("Touch Moved! %f %f %f %f", touchPos.x, touchPos.y, lastTouchPos.x, lastTouchPos.y);
			drawNode->drawSegment(lastTouchPos[touch->getID()], touchPos, radius > 1.0f ? radius : 1.0f, selectedColor);
			lastTouchPos[touch->getID()] = touchPos;
			lastRadius[touch->getID()] = radius;
		}

	};

	touchListener->onTouchesCancelled = [&](const std::vector<Touch *> &touches, Event *event)
	{

	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}


void DrawingCanvas::setupMenu()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	ui::Button *backButton = ui::Button::create();
	backButton->setAnchorPoint(Vec2(0.0f, 1.0f));
	backButton->setPosition(Vec2(0.0f, visibleSize.height));
	backButton->loadTextures("backButton.png", "backButtonPressed.png");
	backButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::backPressed, this));
	ui::Button *clearButton = ui::Button::create();
	clearButton->setAnchorPoint(Vec2(1.0f, 1.0f));
	clearButton->setPosition(Vec2(visibleSize.width, visibleSize.height));
	clearButton->loadTextures("clearButton.png", "clearButtonPressed.png");
	clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::clearPressed, this));

	check = Sprite::create("checkMark.png");
	check->setAnchorPoint(Vec2(0.5f, 0.5f));
	check->setNormalizedPosition(Vec2(0.5f, 0.5f));

	Node *colorButtonLayer = Node::create();
	colorButtonLayer->setContentSize(Size(visibleSize.width, visibleSize.height * 0.2f));
	colorButtonLayer->setAnchorPoint(Vec2(0.5f, 0.0f));
	colorButtonLayer->setPosition(Vec2(visibleSize.width / 2.0f, 0.0f));

	for(int i = 1; i <= 5; i++) {
		Color4F color;
		ui::Button *colorButton = ui::Button::create();
		colorButton->loadTextures("colorSwatch.png", "colorSwatch.png");
		colorButton->setAnchorPoint(Vec2(0.5f, 0.0f));
		colorButton->setPosition(Vec2(visibleSize.width * double(i) / 6.0f, 0.0f));
		colorButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::colorPressed, this));
		switch(i) {
		case 1 : color = COLOR_PURPLE; break;
		case 2 : color = COLOR_BLUE; break;
		case 3 : color = COLOR_GREEN; colorButton->addChild(check); break;
		case 4 : color = COLOR_RED; break;
		case 5 : color = COLOR_YELLOW; break;
		}
		colorButton->setColor(Color3B(color));
		colorButtonLayer->addChild(colorButton);
	}

	this->addChild(colorButtonLayer);
	this->addChild(backButton);
	this->addChild(clearButton);
}

void DrawingCanvas::clearPressed(Ref *pSender, ui::Widget::TouchEventType event)
{
	if(event == ui::Widget::TouchEventType::ENDED) {
		drawNode->clear();
	}
}

void DrawingCanvas::backPressed(Ref *pSender, ui::Widget::TouchEventType event)
{
	if(event == ui::Widget::TouchEventType::ENDED) {
		Scene *scene = Scene::create();
		scene->addChild(Lobby::create());
		Director::getInstance()->replaceScene(TransitionSlideInT::create(0.6f, scene));
	}
}

void DrawingCanvas::colorPressed(Ref *pSender, ui::Widget::TouchEventType event)
{
	ui::Button *pressedButton = static_cast<ui::Button*>(pSender);
	if(event == ui::Widget::TouchEventType::ENDED) {
		selectedColor = Color4F(pressedButton->getColor());
		check->retain();
		check->removeFromParent();
		pressedButton->addChild(check);
		check->release();
	}
}
