#include "CCamera.h"
#include "GameObject.h"

#include "CCollider.h"
#include "CTransform.h"

#include "CTilemapRenderer.h"

void CCamera::OnCreate()
{
}

void CCamera::OnDestroy()
{
}

void CCamera::OnUpdate(float tDeltaTime)
{
	// 1. 추적할 타겟(주로 플레이어)이 있는 경우
	if (mTarget != nullptr) {
		CTransform* targetTr = mTarget->GetComponent<CTransform>(eComponentType::Transform);
		CCollider* targetCollider = mTarget->GetComponent<CCollider>(eComponentType::Collider);

		// 타겟의 중심점을 바라보게 설정
		mLookPosition = targetTr->GetPos() + targetCollider->GetOffset();

		// ==========================================
		// Map Boundary Clamping (맵 밖으로 카메라 나가지 않게 제한)
		// ==========================================
		if (mLookPosition.mX < windowWidth / 2 - tileSizeX) {
			mLookPosition.mX = windowWidth / 2 - tileSizeX + 1;
		}

		if (mLookPosition.mX > mapWidth - windowWidth / 2 + tileSizeX) {
			mLookPosition.mX = mapWidth - windowWidth / 2 + tileSizeX - 1;
		}

		if (mLookPosition.mY < windowHeight / 2 - tileSizeY * 2) {
			mLookPosition.mY = windowHeight / 2 - tileSizeY * 2 + 1;
		}

		if (mLookPosition.mY > mapHeight - windowHeight / 2 + tileSizeY) {
			mLookPosition.mY = mapHeight - windowHeight / 2 + tileSizeY - 1;
		}
	}
	// 2. 타겟이 없는 경우 (자유 시점 카메라 or 연출용)
	else {
		CTransform* cameraTr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
		mLookPosition = cameraTr->GetPos();
	}

	// 렌더링 오프셋(mDistance) 계산
	// mLookPosition은 "카메라가 바라보는 중앙 지점(월드 좌표)"입니다.
	// 렌더링할 때는 "화면 좌상단(0,0)에 해당하는 월드 좌표"를 모든 물체의 좌표에서 빼줘야 합니다.
	// 따라서 (바라보는 점 - 화면 해상도의 절반)을 하여 카메라 사각형의 Left-Top 좌표를 구합니다.
	mDistance = mLookPosition - (mResolution / 2.0f);
}

void CCamera::OnLateUpdate(float tDeltaTime)
{
}

void CCamera::Render(HDC tHDC)
{
}
