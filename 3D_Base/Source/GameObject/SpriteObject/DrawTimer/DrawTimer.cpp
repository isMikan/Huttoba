#include "DrawTimer.h"

DrawTimer::DrawTimer()
	: m_TimerImage	{ }
	, RemainingTimeImage	{}
{
	Init();
}

DrawTimer::~DrawTimer()
{
}

void DrawTimer::Init()
{
	//”š‚Ìİ’è
	RemainingTimeImage[Time::Ten] = std::make_unique<CUIObject>();
	RemainingTimeImage[Time::Ten]->AttachSprite(AssetManager::Sprite(Sprite2DList::TimeNumber));
	RemainingTimeImage[Time::Ten]->SetPatternNo(6, 0);
	RemainingTimeImage[Time::Ten]->SetPosition(635, 10, 0);

	RemainingTimeImage[Time::One] = std::make_unique<CUIObject>();
	RemainingTimeImage[Time::One]->AttachSprite(AssetManager::Sprite(Sprite2DList::TimeNumber));
	RemainingTimeImage[Time::One]->SetPatternNo(0, 0);
	RemainingTimeImage[Time::One]->SetPosition(675, 10, 0);

	//Œv‚Ìİ’è
	m_TimerImage = std::make_unique<CUIObject>();
	m_TimerImage->AttachSprite(AssetManager::Sprite(Sprite2DList::Clock));
	m_TimerImage->SetPatternNo(0, 0);
	m_TimerImage->SetPosition(560, 0, 0);
}

void DrawTimer::Update()
{
	//c‚èŠÔ‚ğæ“¾
	std::pair timeLimit = CTimeManager::GetInstance().GetTimeLimit();

	//ŠeˆÊ‚Éİ’è
	RemainingTimeImage[Time::One]->SetPatternNo(timeLimit.second, 0);
	RemainingTimeImage[Time::Ten]->SetPatternNo(timeLimit.first, 0);
}

void DrawTimer::Draw()
{
	m_TimerImage->Draw();

	for (auto& element : RemainingTimeImage)
	{
		element.second->Draw();
	}
}