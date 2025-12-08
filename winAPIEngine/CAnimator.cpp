#include "CAnimator.h"
#include "CResource.h"
#include "winMacro.h"
#include "CResourceMgr.h"

#include <filesystem>

void CAnimator::OnCreate()
{
}

void CAnimator::OnDestroy()
{
	for (auto& it : mAnimations) {
		SAFE_DELETE(it.second);
	}

	for (auto& it : mEvents) {
		SAFE_DELETE(it.second);
	}
}

void CAnimator::OnUpdate(float tDeltaTime)
{
	if (mActiveAnimation) {
		mActiveAnimation->OnUpdate(tDeltaTime);

		SEvents* events = FindEvents(mActiveAnimation->GetName());

		if (mActiveAnimation->IsComplete() == true) {
			if (events) {
				events->completeEvent();
			}

			if (mbLoop == true) {
				mActiveAnimation->Reset();
			}
		}
	}
}

void CAnimator::OnLateUpdate(float tDeltaTime)
{
}

void CAnimator::Render(HDC tHDC)
{
	if (mActiveAnimation) {
		mActiveAnimation->Render(tHDC);
	}
}

void CAnimator::CreateAnimation(const std::wstring& tName, 
	CTexture* tSpriteSheet, 
	SVector2D tLeftTop, 
	SVector2D tSize, SVector2D tOffset, 
	UINT tSpriteLength, float tDuration)
{
	CAnimation* animation = nullptr;
	animation = FindAnimation(tName);
	
	if (animation != nullptr) return;

	animation = new CAnimation();
	animation->SetName(tName);
	animation->CreateAnimation(tName, tSpriteSheet, tLeftTop, tSize, tOffset, tSpriteLength, tDuration);

	animation->SetAnimator(this);

	SEvents* events = new SEvents();
	mEvents.insert(std::make_pair(tName, events));

	mAnimations.insert(std::make_pair(tName, animation));
}

void CAnimator::CreateAnimationByFolder(const std::wstring& tName, 
	const std::wstring& tPath, 
	SVector2D tOffset, 
	float tDuration)
{
	CAnimation* animation = nullptr;
	animation = FindAnimation(tName);

	if (animation != nullptr) return;

	int fileCount = 0;

	std::filesystem::path fs(tPath);
	std::vector<CTexture*> images = {};

	for (auto& p : std::filesystem::recursive_directory_iterator(fs)) {
		std::wstring fileName = p.path().filename();
		std::wstring fullName = p.path();

		CTexture* texture = CResourceMgr::Load<CTexture>(fileName, fullName);

		images.push_back(texture);
		fileCount++;
	}

	UINT sheetWidth = images[0]->GetWidth() * fileCount;
	UINT sheetHeight = images[0]->GetHeight();

	CTexture* spriteSheet = CTexture::Create(tName, sheetWidth, sheetHeight);

	UINT imageWidth = images[0]->GetWidth();
	UINT imageHeight = images[0]->GetHeight();

	Gdiplus::Graphics g(spriteSheet->GetImage());
	g.Clear(Gdiplus::Color(0, 0, 0, 0));

	for (size_t i = 0; i < images.size(); i++) {
		g.DrawImage(images[i]->GetImage(),
			Gdiplus::Rect(i * imageWidth, 0, imageWidth, imageHeight),
			0, 0, imageWidth, imageHeight, Gdiplus::UnitPixel);
	}

	spriteSheet->CreateHBitmapFromGdiPlus(false);

	/*for (size_t i = 0; i < images.size(); i++) {
		BitBlt(spriteSheet->GetDCMem(), i * imageWidth, 0,
			imageWidth, imageHeight,
			images[i]->GetDCMem(), 0, 0, SRCCOPY);
	}*/

	CreateAnimation(tName, spriteSheet, SVector2D(), SVector2D(imageWidth, imageHeight), tOffset, fileCount, tDuration);
}

CAnimation* CAnimator::FindAnimation(const std::wstring& tName)
{
	auto iter = mAnimations.find(tName);

	if (iter == mAnimations.end()) {
		return nullptr;
	}

	return iter->second;
}

void CAnimator::PlayAnimation(const std::wstring& tName, bool tLoop)
{
	CAnimation* animation = FindAnimation(tName);

	if (animation == nullptr) return;

	if (mActiveAnimation) {
		SEvents* currentEvents = FindEvents(mActiveAnimation->GetName());
		if (currentEvents) {
			currentEvents->endEvent();
		}
	}

	SEvents* nextEvents = FindEvents(animation->GetName());
	if (nextEvents) {
		nextEvents->startEvent();
	}

	mActiveAnimation = animation;
	mActiveAnimation->Reset();
	mbLoop = tLoop;
}

CAnimator::SEvents* CAnimator::FindEvents(const std::wstring tName)
{
	auto iter = mEvents.find(tName);

	if (iter == mEvents.end()) {
		return nullptr;
	}

	return iter->second;
}

std::function<void()>& CAnimator::GetStartEvent(const std::wstring& tName)
{
	SEvents* events = FindEvents(tName);
	return events->startEvent.mEvent;
}

std::function<void()>& CAnimator::GetCompleteEvent(const std::wstring& tName)
{
	SEvents* events = FindEvents(tName);
	return events->completeEvent.mEvent;
}

std::function<void()>& CAnimator::GetEndEvent(const std::wstring& tName)
{
	SEvents* events = FindEvents(tName);
	return events->endEvent.mEvent;
}
