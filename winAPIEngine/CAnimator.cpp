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
	// 생성된 애니메이션 및 이벤트 객체들의 메모리 해제
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
		// 현재 재생 중인 애니메이션의 프레임 진행
		mActiveAnimation->OnUpdate(tDeltaTime);

		SEvents* events = FindEvents(mActiveAnimation->GetName());

		// 애니메이션이 끝까지 재생되었는지 확인
		if (mActiveAnimation->IsComplete() == true) {
			// [Complete Event] 발생: 공격 애니메이션 종료 시점 등에 사용
			if (events) {
				events->completeEvent();
			}

			// 반복 재생 설정이면 초기화하여 다시 재생
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

// 1. 스프라이트 시트 기반 생성 (이미 합쳐진 이미지를 잘라서 사용)
void CAnimator::CreateAnimation(const std::wstring& tName, 
	CTexture* tSpriteSheet, 
	SVector2D tLeftTop, 
	SVector2D tSize, SVector2D tOffset, 
	UINT tSpriteLength, float tDuration)
{
	// 중복 생성 방지
	CAnimation* animation = nullptr;
	animation = FindAnimation(tName);
	
	if (animation != nullptr) return;

	// 새 애니메이션 객체 생성 및 설정
	animation = new CAnimation();
	animation->SetName(tName);
	animation->CreateAnimation(tName, tSpriteSheet, tLeftTop, tSize, tOffset, tSpriteLength, tDuration);

	animation->SetAnimator(this);

	// 이벤트 컨테이너도 같이 생성
	SEvents* events = new SEvents();
	mEvents.insert(std::make_pair(tName, events));

	mAnimations.insert(std::make_pair(tName, animation));
}

// 2. 폴더 기반 생성 (낱장 이미지들을 로딩해 하나의 시트로 합침)
void CAnimator::CreateAnimationByFolder(const std::wstring& tName, 
	const std::wstring& tPath, 
	SVector2D tOffset, 
	float tDuration)
{
	// 중복 확인
	CAnimation* animation = nullptr;
	animation = FindAnimation(tName);

	if (animation != nullptr) return;

	int fileCount = 0;

	std::filesystem::path fs(tPath);	// 해당 경로의 폴더 열기
	std::vector<CTexture*> images = {};

	// 폴더 내의 모든 파일 순회 (Run_00.png, Run_01.png ...)
	for (auto& p : std::filesystem::recursive_directory_iterator(fs)) {
		std::wstring fileName = p.path().filename();
		std::wstring fullName = p.path();

		// 리소스 매니저를 통해 낱장 이미지 로드
		CTexture* texture = CResourceMgr::Load<CTexture>(fileName, fullName);

		images.push_back(texture);
		fileCount++;
	}

	// 병합할 거대 텍스처(Sprite Sheet)의 크기 계산
	// 가로 길이 = 이미지 하나의 너비 * 이미지 개수
	UINT sheetWidth = images[0]->GetWidth() * fileCount;
	UINT sheetHeight = images[0]->GetHeight();

	// 빈 텍스처 생성
	CTexture* spriteSheet = CTexture::Create(tName, sheetWidth, sheetHeight);

	UINT imageWidth = images[0]->GetWidth();
	UINT imageHeight = images[0]->GetHeight();

	// GDI+ Graphics 객체를 사용하여 빈 텍스처에 낱장 이미지들을 그림
	Gdiplus::Graphics g(spriteSheet->GetImage());
	g.Clear(Gdiplus::Color(0, 0, 0, 0));

	for (size_t i = 0; i < images.size(); i++) {
		// 옆으로 이동하며(i * imageWidth) 하나씩 그리기
		g.DrawImage(images[i]->GetImage(),
			Gdiplus::Rect(i * imageWidth, 0, imageWidth, imageHeight),
			0, 0, imageWidth, imageHeight, Gdiplus::UnitPixel);
	}

	// GDI+로 그린 내용을 GDI용 HBITMAP에도 반영 (엔진이 둘 다 쓴다면 동기화 필요)
	spriteSheet->CreateHBitmapFromGdiPlus(false);

	// 합쳐진 텍스처를 이용해 일반 CreateAnimation 호출
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

// 애니메이션 변경 및 재생
void CAnimator::PlayAnimation(const std::wstring& tName, bool tLoop)
{
	CAnimation* animation = FindAnimation(tName);

	if (animation == nullptr) return;

	// 기존 애니메이션이 있었다면 [End Event] 호출
	if (mActiveAnimation) {
		SEvents* currentEvents = FindEvents(mActiveAnimation->GetName());
		if (currentEvents) {
			currentEvents->endEvent();
		}
	}

	// 새 애니메이션의 [Start Event] 호출
	SEvents* nextEvents = FindEvents(animation->GetName());
	if (nextEvents) {
		nextEvents->startEvent();
	}

	// 애니메이션 교체 및 리셋
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

// 이벤트 등록용 함수들
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
