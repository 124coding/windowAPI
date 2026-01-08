#include "CSpriteRenderer.h"
#include "GameObject.h"

#include "CSceneMgr.h"

#include "CTransform.h"

#include "CRenderer.h"

#include "winMacro.h"

#include <assert.h>

void CSpriteRenderer::OnCreate()
{
}

void CSpriteRenderer::OnDestroy()
{

}

void CSpriteRenderer::OnUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::OnLateUpdate(float tDeltaTime)
{
}

void CSpriteRenderer::Render(HDC tHDC)
{
	if (mTexture == nullptr) {
		return;
	}

	// 1. Transform 정보 가져오기
	CTransform* tr = GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	SVector2D pos = tr->GetPos();
	float rot = tr->GetRot();
	SVector2D scale = tr->GetScale();

	// 최종 렌더링 크기 계산 (오브젝트 기본 크기 * 스케일)
	float fScaleX = GetOwner()->GetSize().mX * scale.mX;
	float fScaleY = GetOwner()->GetSize().mY * scale.mY;

	/*if (mbFlipY) {
		fScaleY *= -1.0f;
	}*/

	// 2. 카메라 변환 (World Space -> Screen Space)
	// 메인 카메라가 보고 있는 위치를 기준으로 내 좌표를 보정
	pos = mainCamera->CalculatePosition(pos);

	// 3. 최적화: 배경(BackGround) 레이어 전용 처리
	// 배경은 보통 회전하지 않고 화면을 가득 채우므로 가장 빠른 BitBlt 사용
	if (GetOwner()->GetLayerType() == eLayerType::BackGround) {
		BitBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
			mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
			mTexture->GetDCMem(),
			0, 0,
			SRCCOPY);

		return;
	}

	// 4. 프러스텀 컬링 (Frustum Culling)
	// 화면 밖으로 나간 오브젝트는 그리지 않음 (최적화 핵심)

	// 현재 그려질 영역(Rect) 계산
	float left = pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX;
	float top = pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY;
	float right = left + mTexture->GetWidth() * fScaleX;
	float bottom = top + mTexture->GetHeight() * fScaleY;

	// 회전 시 이미지가 대각선으로 길어져 잘리는 것을 방지하기 위한 여유분(Margin) 계산
	float margin = 0.0f;
	if (rot != 0.0f) margin = (mTexture->GetWidth() * fScaleX + mTexture->GetHeight() * fScaleY) * 0.5f;

	// 화면 범위를 벗어났는지 체크
	if (right + margin < 0 ||
		left - margin > windowWidth ||
		bottom + margin < 0||
		top - margin > windowHeight)
	{
		return;
	}

	// 5. 렌더링 수행 (GDI vs GDI+)

	// Flip(좌우반전) 상태에 맞는 GDI 비트맵 핸들 가져오기
	HBITMAP hBm = mTexture->GetHBitmap(mbFlipX);

	// CASE A: GDI 방식 (빠름, 회전 불가, 단순 투명도)
	if (hBm != nullptr && !mbGdiplusDraw) {
		HDC srcDC = mTexture->GetDCMem();
		HBITMAP oldBm = (HBITMAP)SelectObject(srcDC, hBm);	// DC에 비트맵 장착

		// 알파 블렌딩 (반투명 지원)
		if (mTexture->GetbAlpha()) {
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;	// 32bit 비트맵의 알파 채널 사용

			// 전체 투명도(AlphaMultiplier) 적용
			func.SourceConstantAlpha = (BYTE)(mAlphaMultiplier * 255.0f);
			AlphaBlend(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				srcDC,
				0, 0,
				mTexture->GetBaseWidth(), mTexture->GetBaseHeight(), func);
		}
		// 트랜스패런트 블릿 (특정 색상만 투명 처리 - 크로마키)
		else {
			TransparentBlt(tHDC, pos.mX - GetOwner()->GetAnchorPoint().mX * fScaleX, pos.mY - GetOwner()->GetAnchorPoint().mY * fScaleY,
				mTexture->GetWidth() * fScaleX, mTexture->GetHeight() * fScaleY,
				srcDC,
				0, 0,
				mTexture->GetBaseWidth(), mTexture->GetBaseHeight(),
				RGB(255, 0, 255));	// 마젠타 색상을 투명하게
		}

		SelectObject(srcDC, oldBm);	// DC 복구
	}
	// CASE B: GDI+ 방식 (느림, 회전 가능, 정교한 투명도)
	else {
		Gdiplus::ImageAttributes imgAtt;
		// 1. 키 컬러 설정 (마젠타 투명화)
		imgAtt.SetColorKey(Gdiplus::Color(245, 0, 245), Gdiplus::Color(255, 0, 255));

		// 2. 컬러 매트릭스를 이용한 전체 투명도 조절
		if (mAlphaMultiplier < 1.0f) {
			Gdiplus::ColorMatrix colorMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	// [3][3] 위치가 Alpha 값
				0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			};
			colorMatrix.m[3][3] = mAlphaMultiplier;
			imgAtt.SetColorMatrix(&colorMatrix);
		}

		Gdiplus::Graphics graphics(tHDC);
		
		// 3. 행렬 변환 (순서 중요: 이동 -> 회전 -> 스케일)
		// 3-1. 기준점(Pivot)을 오브젝트의 위치로 이동
		graphics.TranslateTransform(pos.mX, pos.mY);

		// 3-2. 기준점을 중심으로 회전
		graphics.RotateTransform(rot);

		// 3-3. 좌우 반전 처리 (스케일을 음수로)
		if (mbFlipX) {
			fScaleX *= -1.0f;
		}

		// 3-4. 확대/축소 적용
		graphics.ScaleTransform(fScaleX, fScaleY);

		float originalWidth = mTexture->GetBaseWidth();
		float originalHeight = mTexture->GetBaseHeight();

		float drawWidth = mTexture->GetWidth();
		float drawHeight = mTexture->GetHeight();

		// 이미 행렬 변환으로 원점을 (pos.mX, pos.mY)로 옮겨두었으므로,
		// 여기서는 앵커 포인트를 고려하여 로컬 좌표 기준으로 그리면 됨
		Gdiplus::RectF destRect(
			-GetOwner()->GetAnchorPoint().mX,
			-GetOwner()->GetAnchorPoint().mY,
			drawWidth,
			drawHeight
		);

		graphics.DrawImage(mTexture->GetImage(),
			destRect,               
			0.0f, 0.0f,             
			originalWidth,          
			originalHeight,         
			Gdiplus::UnitPixel,     
			&imgAtt                 
		);
	}
}
