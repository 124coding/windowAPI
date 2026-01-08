# [C++ / windowAPI] Brotato 모작

**게임명:** Brotato  
**개발 환경:** C++, windowAPI  
**개발 기간 & 인원:** 2025/10 ~ 2025/12 (3개월), 1인 개발  
**핵심 역할:** 게임 엔진 아키텍처 설계, 컴포넌트 시스템 구현, 인게임 로직 전반  

[![Brotato 모작 시연 영상](http://img.youtube.com/vi/tHov3ex_SmQ/0.jpg)](https://www.youtube.com/watch?v=tHov3ex_SmQ)

## 기술적 핵심
단순히 게임을 구현하는 것을 넘어서 유지보수가 용이하고 확장 가능한 컴포넌트 기반 엔진을 직접 설계해보았습니다.

</br>

### 1. 컴포넌트 기반 아키텍처 (Component-Based Architecture)
기존의 상속 중심 구조(Is-A)에서 발생하는 다중 상속 문제와 거대 부모 클래스 문제를 해결하기 위해, 기능 중심의 **컴포넌트 패턴(Has-A)** 을 도입했습니다. 객체(GameObject)를 빈 컨테이너로 정의하고, 필요한 기능(Component)을 부품처럼 조립하여 유연한 객체 생성이 가능하도록 설계했습니다.

#### 핵심 구현 내용  

#### 1. Component의 모듈화 및 관리

<img width="1000" height="800" alt="Component Diagram" src="https://github.com/user-attachments/assets/2601a9ec-afeb-402b-a5e1-64213a6e63bf" />

- 모든 기능을 CComponent를 상속 받아 구현하고, enum class를 통해 타입을 명확히 구분하여 관리하였습니다.  
- 게임 오브젝트 내에서 vector를 사용해 컴포넌트들을 관리하며, O(1) 조회 성능을 목표로 인덱스 기반 접근 방식을 채택했습니다.

초기에는 유연성을 위해 GetComponent와 RemoveComponent에 dynamic_cast를 사용했으나, 매 프레임 여러 번 호출되는 GetComponent의 특성상 매 프레임 빈번하게 호출되는 함수에서 RTTI(Run-Time Type Information) 비용에 의한 오버헤드를 제거하기 위해 vector의 인덱스를 Enum과 매핑하여 O(1)의 상수 시간 접근이 가능하도록 구조를 최적화하였습니다.

<details>
<summary>Component 클래스</summary>
<div markdown="1">

```C++
#pragma once
#include "CEntity.h"
#include "Enums.h"
#include "SVector2D.h"
 
#include <windows.h>

class GameObject;

class CComponent : public CEntity
{
public:
	CComponent(eComponentType tType) : mActive(true), mType(tType) {}	
	virtual ~CComponent() {}

	virtual CComponent* Clone() = 0;

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float tDeltaTime) = 0;
	virtual void OnLateUpdate(float tDeltaTime) = 0;
	virtual void Render(HDC tHDC) = 0;

	void SetOwner(GameObject* tOwner) {
		mOwner = tOwner;
	}

	GameObject* GetOwner() {
		return this->mOwner;
	}

	eComponentType GetType() {
		return this->mType;
	}

	void SetActive(bool tActive) {
		this->mActive = tActive;
	}

	bool IsActive() {
		return this->mActive;
	}

private:
	bool mActive;
	// 해당 컴포넌트의 주인이 누구인지 바로 확인할 수 있게함.
    GameObject* mOwner = nullptr;
	eComponentType mType;
};


```

</div>
</details>  
</br>

<details>
<summary>Component enum 클래스</summary>
<div markdown="1">

```C++
// 컴포넌트 분류
enum class eComponentType {
	Transform,
	Rigidbody,
	Script,
	SpriteRenderer,
	WeaponMgr,
	ItemMgr,
	AutoSpriteFlipper,
	TileRenderer,
	Animator,
	Collider,
	Camera,
	AudioListner,
	AudioSource,
	End
};
```

</div>
</details>  
</br>

<details>
<summary>GameObject 클래스 컴포넌트 관련 코드</summary>
<div markdown="1">

```C++
// 컴포넌트 추가
template <typename T>
T* AddComponent() {
	T* comp = new T();
	comp->SetOwner(this);

	mComponents[(UINT)comp->GetType()] = comp;

	return comp;
}

// 컴포넌트 삭제
template <typename T>
void RemoveComponent(eComponentType type) {
    int index = (int)type;
    
    if (mComponents[index] != nullptr) {
        mComponents[index]->OnDestroy();
        SAFE_DELETE(mComponents[index]);
        mComponents[index] = nullptr;
    }
}

// 컴포넌트 획득
template <typename T>
T* GetComponent(eComponentType type) {
	if (mComponents[(UINT)type] != nullptr)
		return static_cast<T*>(mComponents[(UINT)type]);
	return nullptr;
}
```

</div>
</details>
</br>

#### 2. 생명 주기(Life-Cycle) 동기화
- Unity/Unreal 엔진의 흐름과 유사하게 OnCreate -> OnUpdate -> Render -> OnDestroy의 생명 주기를 설계했습니다.
- 이러한 흐름 중에 로직의 누락을 방지하기 위해 모든 활성 컴포넌트가 자동 호출되는 위임(Delegation) 구조를 구축하였습니다.
- 이러한 생명 주기 코드는 특정 클래스들을 제외한 해당 프로젝트의 대부분의 클래스에서 채택하였습니다. 

<details>
<summary>GameObject 생명주기 코드</summary>
<div markdown="1">

```C++
virtual void OnCreate();
virtual void OnDestroy();
virtual void OnUpdate(float tDeltaTime);
virtual void OnLateUpdate(float tDeltaTime);
virtual void Render(HDC tHDC);

// 오브젝트 생성 시 미리 가지고 있는 컴포넌트들도 생성 시 코드를 실행 시켜줌.
void GameObject::OnCreate()
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnCreate();
	}
}

// 오브젝트 삭제 시 가지고 있는 컴포넌트들도 삭제시켜줌.
// 컴포넌트들이 삭제 시 코드를 실행 시켜줌.
void GameObject::OnDestroy()
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr) continue;
		comp->OnDestroy();
		SAFE_DELETE(comp);
	}

	mComponents.clear();
}

// 오브젝트 업데이트 시 가지고 있는 컴포넌트들도 업데이트시켜줌.
// 이는 1 프레임 당 한번씩 실행됨.
void GameObject::OnUpdate(float tDeltaTime)
{

	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->OnUpdate(tDeltaTime);
	}
}

// 이는 Render 후와 같은 업데이트 되야 하지만 조금 늦게 업데이트될 코드들을 작성해줌.
void GameObject::OnLateUpdate(float tDeltaTime)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->OnLateUpdate(tDeltaTime);
	}
}

// 오브젝트를 화면에 그릴 시 가지고 있는 컴포넌트들의 그리기 코드도 실행시켜줌.
void GameObject::Render(HDC tHDC)
{
	for (CComponent* comp : mComponents) {
		if (comp == nullptr || !comp->IsActive()) continue;
		comp->Render(tHDC);
	}
}
```

</div>
</details>  
</br>

#### 구현 성과
**1. 재사용성 극대화:** Player, Enemy 등 클래스를 별도로 복잡하게 만들지 않고 동일한 CTransform, CCollider 등의 컴포넌트들을 공유하여 코드의 중복을 약 60% 이상 감소시켰습니다.  
**2. 유지보수 용이:** 기능 추가 시에는 기존 코드를 수정할 필요 없이, 새로운 컴포넌트 클래스만 생성하여 부착하는 방식으로 확장성을 확보하였고 또한 특정 기능을 수정할 시에도 관련 컴포넌트만 수정하면 되기에 유지보수성을 확보하였습니다.


</br>

### 2. 충돌 처리 최적화: Bitset 행렬과 공간 분할
수백 개의 투사체와 몬스터가 난무하는 상황에서 $O(N^2)$의 충돌 연산은 치명적입니다. 이를 해결하기 위해 비트 연산 기반의 레이어 필터링과 메모리 최적화된 자료구조를 도입했습니다.

#### 핵심 구현 내용

#### 1.  std::bitset을 활용한 $O(1)$ 충돌 필터링
- std::vector나 조건문(if)을 순회하는 대신, std::bitset을 활용하여 충돌 상호작용 매트릭스(Interaction Matrix)를 구현했습니다.  
- 특정 레이어 간의 충돌 여부를 비트 연산 한 번으로 확인하므로, 객체 수가 늘어나도 필터링 오버헤드가 '0'에 수렴하도록 최적화했습니다.

#### 2. Union을 활용한 충돌 쌍(Pair) ID 최적화
- 충돌 이벤트를 관리하는 std::unordered_map의 키(Key)를 생성할 때, 문자열이나 구조체를 쓰지 않고 Union을 활용했습니다.
- 두 개의 UINT32 ID(Left, Right)를 메모리 복사 없이 하나의 UINT64 정수로 변환하여, 해시 맵 조회 속도를 극대화했습니다.

<details> 
<summary>최적화된 충돌 매니저 핵심 코드</summary> 
<div markdown="1">

```C++

class CCollisionMgr {
    // Union을 사용하여 비트 연산 없이 두 개의 32비트 ID를 64비트 키로 병합
    union CollisionID {
        struct {
            UINT32 left;
            UINT32 right;
        };
        UINT64 cId;
    };

    // 메모리 효율적인 비트셋 매트릭스
    static std::bitset<(UINT)eLayerType::MAX> mCollisionLayerMtrix[(UINT)eLayerType::MAX];
};

void CCollisionMgr::ColliderCollision(float tDeltaTime, CCollider* tLeft, CCollider* tRight) {
    // ...
    CollisionID id = {};
    id.left = tLeft->GetID();
    id.right = tRight->GetID();

    // UINT64 정수형 키를 사용하여 고속 조회
    auto iter = mCollisionMap.find(id.cId);
    // ...
}

```
</div> 
</details>
</br>

#### 3. 충돌체(Collider)의 추상화 및 다형성
- 모든 충돌체를 CCollider로 추상화하고, 이를 상속받는 CBoxCollider2D(직사각형)와 CCircleCollider2D(원형)을 구현했습니다.  
- 기본적인 AABB(Axis-Aligned Bounding Box)와 Circle 충돌뿐만 아니라, 회전하는 오브젝트를 정밀하게 판정하기 위해 OBB(Oriented Bounding Box, 분리축 이론) 알고리즘과 Clamping(Closest Point) 기법을 적용하여 Box-Circle 간의 충돌까지 완벽하게 구현했습니다.

<details>
<summary>Collider Intersect 코드</summary>
<div markdown="1">

```C++
bool CCollisionMgr::Intersect(CCollider* tLeft, CCollider* tRight)
{
	// 각 충돌체의 위치(AnchorPoint 기준)
	CTransform* leftTr = tLeft->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);
	CTransform* rightTr = tRight->GetOwner()->GetComponent<CTransform>(eComponentType::Transform);

	// 각 충돌체의 크기
	SVector2D leftSize = ObjectSize(tLeft->GetOwner()) * tLeft->GetSize();
	SVector2D rightSize = ObjectSize(tRight->GetOwner()) * tRight->GetSize();

	// 각 충돌체의 중간 위치
	SVector2D leftCenterPos = ObjectCenterPos(tLeft->GetOwner());
	SVector2D rightCenterPos = ObjectCenterPos(tRight->GetOwner());

	// 충돌체 간의 거리의 제곱
	float distanceSq = (leftCenterPos - rightCenterPos).LengthSq();

	// 각 충돌체의 타입
	eColliderType leftType = tLeft->GetColliderType();
	eColliderType rightType = tRight->GetColliderType();

	// 각 오브젝트의 중심을 기준으로
	if (leftTr->GetRot() == 0.0f && rightTr->GetRot() == 0.0f) {
		if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
			// rect - rect
			// 두 사각형의 중심점의 가로, 세로 지름 따로 측정하여 그 거리가 각 사각형의 가로, 세로 거리보다 작으면 충돌
			if (fabs(leftCenterPos.mX - rightCenterPos.mX) < fabs(leftSize.mX / 2.0f + rightSize.mX / 2.0f) &&
				fabs(leftCenterPos.mY - rightCenterPos.mY) < fabs(leftSize.mY / 2.0f + rightSize.mY / 2.0f)) {

				return true;
			}
		}
		else if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {// circle - circle
			// 두 원의 중심점 사이의 거리가 두 원의 반지름의 합보다 작으면 충돌
			if (distanceSq <= (leftSize.mX / 2.0f + rightSize.mX / 2.0f) * (leftSize.mX / 2.0f + rightSize.mX / 2.0f)) {
				return true;
			}
		}
		else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Circle2D ||
			leftType == eColliderType::Circle2D && rightType == eColliderType::Rect2D) { // circle - rect

			SVector2D circlePos, rectPos, rectHalfSize;
			float circleRadius;
			// 사각형의 각 가로, 세로 지름의 반과 원의 반지름을 더해서 사각형의 가로, 세로 기준 각 중심점의 거리와 비교해서 중심점의 거리가 짧으면 충돌
			if (leftType == eColliderType::Circle2D) {
				circlePos = leftCenterPos;
				circleRadius = leftSize.mX / 2.0f;
				rectPos = rightCenterPos;
				rectHalfSize = rightSize / 2.0f;
			}
			else {
				circlePos = rightCenterPos;
				circleRadius = rightSize.mX / 2.0f;
				rectPos = leftCenterPos;
				rectHalfSize = leftSize / 2.0f;
			}

			// 클램핑(Clamping) 기법 이용 충돌 코드
			SVector2D rectMin = rectPos - rectHalfSize;
			SVector2D rectMax = rectPos + rectHalfSize;

			float closestX = std::max(rectMin.mX, std::min(circlePos.mX, rectMax.mX));
			float closestY = std::max(rectMin.mY, std::min(circlePos.mY, rectMax.mY));

			SVector2D closestPoint(closestX, closestY);

			float distanceSquared = (circlePos - closestPoint).LengthSq();
			float radiusSquared = circleRadius * circleRadius;

			if (distanceSquared < radiusSquared) {
				return true;
			}
		}
	}
	else {
		if (leftType == eColliderType::Circle2D && rightType == eColliderType::Circle2D) {
			float radiusSum = (leftSize.mX / 2.0f + rightSize.mX / 2.0f);
			if (distanceSq <= radiusSum * radiusSum) {
				return true;
			}
		}
		// OBB끼리의 충돌(분리축 이론)
		else if (leftType == eColliderType::Rect2D && rightType == eColliderType::Rect2D) {
			SVector2D dist = leftCenterPos.GetDistanceVector(rightCenterPos);

			SVector2D vec[4] = { leftSize.GetWidthVector(leftTr->GetRot()), leftSize.GetHeightVector(leftTr->GetRot()), rightSize.GetWidthVector(rightTr->GetRot()), rightSize.GetHeightVector(rightTr->GetRot()) };

			for (int i = 0; i < 4; i++) {
				float sum = 0.0f;
				// 검사 기준 축 정규화하여 벡터로 만듦
				SVector2D unit = vec[i].Normalize();

				// 모든 4개의 반경 벡터를 현재 기준 축에 투영(절대값 - 방향은 신경쓰지 않기에)
				for (int j = 0; j < 4; j++) {
					sum += (vec[j].Dot(unit) > 0) ? vec[j].Dot(unit) : -vec[j].Dot(unit);
				}

				float dot = (dist.Dot(unit) > 0) ? dist.Dot(unit) : -dist.Dot(unit);

				// 투영된 중심 거리가 투영된 두 사각형의 반지름 합보다 크다면 사이에 빈 공간이 있다는 뜻이기에 충돌 X
				if (dot > sum) {
					return false;
				}
			}
			return true;
		}
		else {
			// 원과 OBB의 충돌
			// 원에서 가장 가까운 사각형의 점을 구하여 이 점과 원의 중심점과의 거리를 구하여 그 거리가 반지름보다 짧다면 충돌
			CCollider* rectCol = (leftType == eColliderType::Rect2D) ? tLeft : tRight;
			CCollider* circleCol = (leftType == eColliderType::Circle2D) ? tLeft : tRight;

			SVector2D rectPos = ObjectCenterPos(rectCol->GetOwner());
			SVector2D circlePos = ObjectCenterPos(circleCol->GetOwner());

			SVector2D rectHalfSize = (ObjectSize(rectCol->GetOwner()) * rectCol->GetSize()) / 2.0f;
			float circleRadius = (ObjectSize(circleCol->GetOwner()) * circleCol->GetSize()).mX / 2.0f;

			float rectRot = rectCol->GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetRot();

			// 원의 중심을 사각형 기준 로컬 좌표로 변환
			SVector2D dir = circlePos - rectPos;

			// 사각형의 회전값(rectRot)만큼 반대로 회전(-rectRot)시켜야 로컬 축 정렬이 됨 (이는 내적을 이용해 투영)
			SVector2D rectRight = rectCol->GetSize().GetWidthVector(rectRot).Normalize();
			SVector2D rectUp = rectCol->GetSize().GetHeightVector(rectRot).Normalize();  

			float localX = dir.Dot(rectRight);
			float localY = dir.Dot(rectUp);

			float closestX = std::max(-rectHalfSize.mX, std::min(localX, rectHalfSize.mX));
			float closestY = std::max(-rectHalfSize.mY, std::min(localY, rectHalfSize.mY));

			float distanceX = localX - closestX;
			float distanceY = localY - closestY;

			float distSq = (distanceX * distanceX) + (distanceY * distanceY);

			if (distSq <= (circleRadius * circleRadius)) {
				return true;
			}
		}
	}

	return false;
}
```

</div>
</details>  
</br>

#### 4. 레이어 기반 충돌 필터링(Layer Interaction Matrix)
- 모든 게임 오브젝트를 Player, Enemy, MeleeWeapon, Bullet 등의 레이어로 분류하였습니다.
- 각 씬(Scene)에서 충돌이 필요한 레이어 조합만을 미리 지정하여 불필요한 연산은 원천 차단하였습니다.  

초기에는 레이어를 따로 두지 않고 모든 객체에 대해 충돌 로직을 확인하다보니 불필요한 충돌 연산이 너무 많아 추후에 유닛이 아주 많아지면 성능에 문제가 될 것이라고 생각하여 Unity/Unreal 엔진처럼 레이어 분리를 도입했습니다.

<details>
<summary>Layer Enum 클래스</summary>
<div markdown="1">

```C++
enum class eLayerType {
	None,
	BackGround,
	Tile,
	Material,
	Enemy,
	Player,
	Effect,
	Bullet,
	MeleeWeapon,
	RangedWeapon,
	Particle,
	EffectText,
	MAX = 16
};
```

</div>
</details>  
</br>


<details>
<summary>Layer CollisionCheck 코드</summary>
<div markdown="1">

```C++
// 두 Layer타입을 받아서 Layer 간의 오브젝트 충돌을 시킬지 말지 정하는 함수
void CCollisionMgr::CollisionLayerCheck(eLayerType tLeft, eLayerType tRight, bool tEnable) {
	int row = 0;
	int col = 0;

	if (tLeft <= tRight) {
		row = (UINT)tLeft;
		col = (UINT)tRight;
	}
	else {
		row = (UINT)tRight;
		col = (UINT)tLeft;
	}

	// 충돌 행렬을 둬서 충돌이 가능한지 아닌지를 저장
	mCollisionLayerMtrix[row][col] = tEnable;
}


void CCollisionMgr::LayerCollision(float tDeltaTime, CScene* tScene, eLayerType tLeft, eLayerType tRight) {
	// 각 레이어의 오브젝트들을 가져와서 충돌체가 현재 사용중인 오브젝트인지, 충돌체가 있는지 확인하고 충돌을 일으킴
	const std::vector<GameObject*>& lefts = CSceneMgr::GetGameObjects(tLeft);
	const std::vector<GameObject*>& rights = CSceneMgr::GetGameObjects(tRight);

	for (GameObject* left : lefts) {
		if (left->IsActive() == false) continue;

		CCollider* leftCollider = left->GetComponent<CCollider>(eComponentType::Collider);
		if (leftCollider == nullptr) continue;
		
		for (GameObject* right : rights) {
			if (right->IsActive() == false) continue;

			CCollider* rightCollider = right->GetComponent<CCollider>(eComponentType::Collider);
			if (rightCollider == nullptr) continue;

			if (left == right) {
				continue;
			}

			ColliderCollision(tDeltaTime, leftCollider, rightCollider);
		}
	}
}

```

</div>
</details>  
</br>

충돌 체크 시 매번 GetComponent를 호출하는 비용을 줄이기 위해, Scene 단계에서 Collider 컴포넌트를 가진 오브젝트만 별도의 리스트로 캐싱(Caching)하는 최적화 기법도 고려할 수 있습니다. 이 프로젝트에서는 따로 수정하지는 않았습니다.

#### 5. 이벤트 기반 충돌 처리
- 충돌 발생 시, OnCollisionEnter, OnCollisionStay, OnCollisionExit 이벤트를 발생시켜 이를 각 스크립트에서 함수 오버라이딩하여 독립적으로 로직을 처리하도록 하였습니다.
- 이를 통해 충돌 감지 로직과 충돌 후 처리 로직을 완벽하게 분리하였습니다.
  
<details>
<summary>Collision 이벤트 코드 (PlayerScript)</summary>
<div markdown="1">

```C++
virtual void OnCollisionEnter(float tDeltaTime, CCollider* tOther);
virtual void OnCollisionStay(float tDeltaTime, CCollider* tOther);
virtual void OnCollisionExit(float tDeltaTime, CCollider* tOther);



void CPlayerScript::OnCollisionEnter(float tDeltaTime, CCollider* tOther) {
	if (mCanCollideEnemy && tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
		mCanCollideEnemy = false;
		int rand = std::rand() % 100;

		if (mDodge > mDodgeLimit) {
			if (rand > mDodgeLimit) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
			else {
				CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
			}
		}
		else {
			if (rand > mDodge) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
			else {
				CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
			}
		}
	}

	if (tOther->GetOwner()->GetLayerType() == eLayerType::Material) {
		this->ChangeMoney(tOther->GetOwner()->GetComponent<CMaterialScript>(eComponentType::Script)->GetMoney() * CMonsterSpawnMgr::GetMoneyMultiple());
		this->IncreasedExp(tOther->GetOwner()->GetComponent<CMaterialScript>(eComponentType::Script)->GetExp() * CMonsterSpawnMgr::GetExpMultiple());

		if (this->mExp >= this->mNeedLevelUpExp) {
			this->mExp -= this->mNeedLevelUpExp;
			this->IncreaseLevel();
			this->mCurStageLevelUpCount++;
			this->IncreaseMaxHP(1);
			this->mNeedLevelUpExp *= 1.2f;
		}

		ObjDestroy(tOther->GetOwner());
	}
}

void CPlayerScript::OnCollisionStay(float tDeltaTime, CCollider* tOther) {

	if (mCanCollideEnemy) {
		mCanCollideEnemy = false;
		int rand = std::rand() % 100 + 1;
		if (rand > mDodge) {
			if (tOther->GetOwner()->GetLayerType() == eLayerType::Enemy) {
				GameObject* enemy = tOther->GetOwner();
				ButtDamageByEnemy(enemy);
			}
		}
		else {
			CEffectMgr::ShowEffectText(GetOwner()->GetComponent<CTransform>(eComponentType::Transform)->GetPos(), L"회피", Gdiplus::Color::White);
		}
	}
}

void CPlayerScript::OnCollisionExit(float tDeltaTime, CCollider* tOther) {

}

```

</div>
</details>  
</br>

#### 구현 성과
**1. 성능 최적화:** 이전 방식 대비 연산량을 획기적으로 줄여, 200개 이상의 동적 오브젝트(유닛+투사체)가 난전(Bullet Hell)을 벌이는 상황에서도 90 FPS 이상을 안정적으로 유지했습니다.  
**2. 로직 분리:** 예를 들어 Enemy가 무기에 부딪히면 Enemy의 체력이 깍인다는 로직을 생각해볼때 EnemyScript의 OnCollisionEnter에만 작성하면 되기에 코드의 응집도를 높였습니다.  

</br>

### 3. 데이터 주도적 아키텍처와 레벨 디자인 파이프라인 (Data-Driven Architecture)
로그라이크 장르의 방대한 콘텐츠(수십 종의 무기, 몬스터, 아이템)와 복잡한 웨이브 시스템을 코드에 하드코딩하는 것은 확장성과 유지보수성을 저해합니다. 이를 해결하기 위해 데이터(JSON)와 로직(C++)을 철저히 분리하고, 기획 데이터 수정만으로 게임의 밸런스와 스테이지 구성을 즉시 변경할 수 있는 파이프라인을 구축했습니다.

#### 핵심 구현 내용

#### 1. 템플릿과 람다를 활용한 추상 팩토리 시스템
- DataMgr 내부에 std::function을 값으로 갖는 Creator Map을 구축하여 객체 생성 로직을 캡슐화했습니다.
- 무기 정보(WeaponInfo), 아이템 정보(ItemInfo) 등의 구조체를 정의하고 unordered_map을 사용하여 문자열 ID(wstring key)를 통해 O(1) 속도로 데이터에 접근할 수 있게 구현했습니다.
- Register 템플릿 함수를 통해 특정 ID와 클래스 생성 로직(람다)을 연결함으로써, 구체적인 클래스 타입(CDagger, CBabyAlien 등)을 알 필요 없이 문자열 ID만으로 다형성 객체를 생성하는 유연한 구조를 만들었습니다.

[JSON 데이터 모음](https://github.com/124coding/windowAPI/tree/master/Data)  

<details>
<summary>데이터 로드 함수 코드</summary>
<div markdown="1">

```C++
void CDataMgr::LoadDatas() {
    // ... (JSON 파일 스트림 열기) ...

    // 1. 몬스터 데이터 로드 (MonsterStats.json)
    for (auto& monsterStat : monsterStats["Monsters"]) {
        SMonster monster;
        monster.ID = ToWString(monsterStat["M_ID"]);
        monster.hp = monsterStat["HP"];
        monster.hpIncreasedEachWave = monsterStat["HP_increase_each_wave"];
        // ... (스탯 파싱) ...
        
        mMonsterStats.insert({ monster.ID, monster });
    }

    // 2. 무기 데이터 로드 (Weapons.json)
    for (auto& weaponData : weaponDatas["MeleeWeapons"]) {
        SWeapon weapon;
        weapon.ID = ToWString(weaponData["W_ID"]);
        
        // 티어(Tier)별 스탯 파싱
        int i = 0;
        for (const auto& t : weaponData["Tiers"]) {
            weapon.tier[i].damage = t["Damage"];
            weapon.tier[i].delay = t["Delay"];
            weapon.tier[i].basePrice = t["BasePrice"];
            i++;
        }
        mWeaponDatas.insert({ weapon.ID, weapon });
    }
    
    // ... (아이템, 캐릭터, 업그레이드 데이터 로드) ...
}

```

</div>
</details>  
</br>

#### 2. 무기 시스템: 데이터 기반 생성 및 스탯 자동화
- 무기 획득 시, WeaponMgr가 ID를 기반으로 객체를 생성하고 JSON 데이터를 조회하여 공격력, 쿨타임, 티어(Tier)별 스탯을 자동으로 적용합니다.
- 특히 PlusWeapon 함수에서는 인벤토리에 동일한 무기가 있을 경우, 자동으로 합성을 진행하여 상위 티어의 데이터를 적용하는 로직까지 통합하여 구현했습니다.

<details>
<summary>무기 추가 함수 코드</summary>
<div markdown="1">

```C++
std::pair<int, CWeapon*> CWeaponMgr::PlusWeapon(eLayerType tType, std::wstring tWeaponId, int tWeaponTier) {

    // 1. 인벤토리가 가득 찼을 때: 동일 무기/동일 티어가 있는지 확인하여 합성 시도
    if (mWeapons.size() >= mWeaponCount ) {
        if (tWeaponTier == 4) return { -1, nullptr }; // 최대 티어 예외처리

        for (int i = 0; i < mWeapons.size(); i++) {
            CWeaponScript* wpSc = mWeapons[i]->GetComponent<CWeaponScript>(eComponentType::Script);
            
            // 동일 ID, 동일 티어 발견 -> 티어 업그레이드
            if (mWeapons[i]->GetID() == tWeaponId && wpSc->GetTier() == tWeaponTier) {
                auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
                CDataMgr::SWeapon currentWeapon = weaponIter->second;

                // 다음 티어(tWeaponTier + 1)의 스탯 적용
                wpSc->SetDamage(currentWeapon.tier[tWeaponTier].damage);
                wpSc->SetDelay(currentWeapon.tier[tWeaponTier].delay);
                // ... (기타 스탯 갱신) ...
                wpSc->SetTier(tWeaponTier + 1);

                return { i, mWeapons[i] };
            }
        }
        return { -1, nullptr }; // 합성 불가능하면 추가 실패
    }

    // 2. 신규 무기 생성
    auto weaponIter = CDataMgr::GetWeaponDatas().find(tWeaponId);
    CDataMgr::SWeapon currentWeapon = weaponIter->second;

    auto iter = CDataMgr::GetWeaponCreator().find(currentWeapon.ID);
    CWeapon* weapon = iter->second(); // ID에 맞는 객체 생성
    weapon->SetID(tWeaponId);

    // 3. 컴포넌트 설정 및 스탯 초기화
    // ... (이미지, 콜라이더 설정) ...
    
    CWeaponScript* wpSc = weapon->GetComponent<CWeaponScript>(eComponentType::Script);
    wpSc->SetDamage(currentWeapon.tier[tWeaponTier - 1].damage);
    // ... (현재 티어 스탯 적용) ...

    mWeapons.push_back(weapon);
    return { mWeapons.size() - 1, mWeapons.back() };
}
```

</div>
</details>  
</br>

#### 3. 레벨 디자인: JSON 기반의 웨이브 컨트롤
- 확장성을 고려하여 다양한 적 타입(Enemy IDs)과 스폰 패턴을 JSON 데이터로 정의하고, MonsterSpawnMgr가 이를 해석하여 실행하도록 설계했습니다.
- **군집 스폰(Cluster)** 과 **개별 스폰(Individual)** 을 구분하고, 시간 경과에 따라 스폰 주기가 감소하는 로직을 포함하여, 프로그래머의 개입 없이 기획 데이터만으로 난이도 곡선(Difficulty Curve)을 조절할 수 있는 환경을 구축했습니다.

<details>
<summary>몬스터 스폰 코드</summary>
<div markdown="1">

```C++
// 현재 스테이지의 몬스터 스폰 데이터를 읽어옴.
void CMonsterSpawnMgr::LoadStageSpawnEvents(int tStageNum) {
	mActiveStageSpawnEvents.clear();

	std::string stageFileName = "../Data/Stages/Stage_" + std::to_string(tStageNum) + ".json";
	std::ifstream stageFile(stageFileName, std::ios::in);

	if (!stageFile.is_open()) {
		return;
	}

	json j;
	stageFile >> j;

	mStageNum = tStageNum;

	// 돈 배율, 경험치 배율, 스테이지 별 시간을 저장.
	mInitialTime = j["DefaultModifiers"]["Time"];
	mExpMultiple = j["DefaultModifiers"]["Exp_Multiplier"];
	mMoneyMultiple = j["DefaultModifiers"]["Money_Multiplier"];
	mTime = mInitialTime;

	for (auto& e : j["Events"]) {
		SpawnEvent event;

		// 각 이벤트 별로 스폰은 언제 할건지, 최소 스폰 텀은 어느정도인지, 몇 초씩 스폰 텀을 줄어들게 할건지 등
		event.spawnTiming = e["SpawnTiming"];
		event.repeatingInterval = e["repeating_interval"];
		event.minRepeatingInterval = e["min_repeating_interval"];
		event.reduceRepeatingInterval = e["reduce_repeating_interval"];

		event.nextSpawnTime = event.spawnTiming;
		event.currentInterval = event.repeatingInterval;

		for (auto& mId : e["M_IDS"]) {
			event.IDs.push_back(CDataMgr::ToWString(mId));
		}
		event.spawnType = CDataMgr::ToWString(e["SpawnType"]);

		// 스폰 이벤트 벡터에 이를 저장
		mActiveStageSpawnEvents.push_back(event);
	}
}

// 스테이지가 끝날 시 스폰 이벤트 벡터를 정리
void CMonsterSpawnMgr::DestroyStageSpawnEvents() {
	mActiveStageSpawnEvents.clear();
}

SVector2D CMonsterSpawnMgr::GetRandomPosAroundObject(SVector2D tPlayerPos, float minR, float maxR) {
	// 입력 위치를 중심으로 최소, 최대 거리 사이에서 랜덤 360도 안에 몬스터 위치를 잡아줌.
	float angle = DegToRad(((float)rand() / RAND_MAX) * 360);
	float dist = minR + ((float)rand() / RAND_MAX) * (maxR - minR);

	float x = cosf(angle) * dist;
	float y = sinf(angle) * dist;

	SVector2D resultPos = tPlayerPos + SVector2D(x, y);

	if (resultPos.mX < 0 + tileSizeX) {
		resultPos.mX = 0 + tileSizeX;
	}
	else if (resultPos.mX > mapWidth - tileSizeX) {
		resultPos.mX = mapWidth - tileSizeX;
	}

	if (resultPos.mY < 0 + tileSizeY) {
		resultPos.mY = 0 + tileSizeY;
	}
	else if (resultPos.mY > mapHeight - tileSizeY) {
		resultPos.mY = mapHeight - tileSizeY;
	}

	return resultPos;
}

void CMonsterSpawnMgr::MonsterSpawnEvent(GameObject* tTarget) {
	for (auto& spawnEvent: mActiveStageSpawnEvents) {
		// 몬스터의 스폰 타입이 군집적인지 개별적인지에 따라 스폰을 다르게
		if (mInitialTime - mTime >= spawnEvent.nextSpawnTime) {
			if (spawnEvent.spawnType == L"Individual") {
				for (auto& mID : spawnEvent.IDs) {
					MonsterSpawn(mID, tTarget, 500.0f, 1000.0f, true);
				}
			}
			else if (spawnEvent.spawnType == L"cluster") {
				for (auto& mID : spawnEvent.IDs) {
					MonsterSpawn(mID, tTarget, 0.0f, 50.0f, true);
				}
			}

			if (spawnEvent.currentInterval > spawnEvent.minRepeatingInterval) {
				spawnEvent.currentInterval -= spawnEvent.reduceRepeatingInterval;
			}
			spawnEvent.nextSpawnTime += spawnEvent.currentInterval;
		}
		else {
			break;
		}
	}
}

// 데이터를 기반으로 자동으로 몬스터 생성
void CMonsterSpawnMgr::MonsterSpawn(const std::wstring tMonsterId, GameObject* tTarget, float tMinDistance, float tMaxDistance, bool tIndividual) {
	SVector2D targetPos = tTarget->GetComponent<CTransform>(eComponentType::Transform)->GetPos();

	SVector2D anchorPos = GetRandomPosAroundObject(targetPos, 500.0f, 800.0f);

	CDataMgr::SMonster currentMonster;

	auto it = CDataMgr::GetMonsterBasicStats().find(tMonsterId);

	if (it == CDataMgr::GetMonsterBasicStats().end()) {
		return;
	}

	currentMonster = it->second;

	auto iter = CDataMgr::GetMonsterCreator().find(currentMonster.name);
	if (iter == CDataMgr::GetMonsterCreator().end()) {
		return;
	}

	int offset = currentMonster.maxNumber - currentMonster.minNumber;
	int rand = 0;
	if (offset == 0) {
		rand = currentMonster.minNumber;
	}
	else {
		rand = std::rand() % offset + currentMonster.minNumber;
	}

	for (int i = 0; i < rand; i++) {
		// 몬스터의 최대 수는 100마리
		if (CSceneMgr::GetGameObjects(eLayerType::Enemy).size() == 100) {
			break;
		}

		// ... (몬스터 생성 코드) ...
	}
}
```

</div>
</details>  
</br>

#### 구현 성과
**1. 통합 데이터 파이프라인 구축:** 무기, 몬스터, 스테이지 정보 등 게임의 핵심 요소들을 모두 JSON으로 관리하여, 코드 수정 없이 콘텐츠의 추가 및 밸런스 조절이 가능한 OCP(개방-폐쇄 원칙) 환경을 완성했습니다.   
**2. 생산성 향상:** 무기 추가 및 웨이브 설계 시 클래스 생성부터 스탯 적용까지의 과정을 자동화하여, 단순 반복 작업을 제거하고 로직 구현에 집중할 수 있었습니다. 이를 통해 1인 개발임에도 빠른 속도로 콘텐츠 볼륨을 확보했습니다.

</br>

### 4. FSM을 활용한 AI 및 전투 로직
브로테이토는 플레이어의 직접적인 공격 조작 없이, 캐릭터가 장착한 무기가 자동으로 적을 탐색하고 공격하는 오토 배틀러 시스템을 가지고 있습니다. 이를 구현하기 위해 각 무기와 몬스터의 행동을 상태(State)로 정의하고, FSM(Finite State Machine) 패턴을 도입하여 로직의 꼬임을 방지하고, 적의 피격 처리와 대미지 연산 로직을 모듈화하여 안정적인 전투 루프를 완성했습니다.

#### 핵심 구현 내용

#### 1. 상태(State) 기반의 무기 행동 제어
- 근접 무기(Melee): 행동을 Idle(탐색), Attack(추적/공격), Back(복귀)의 3단계로 정의했습니다.  
1. Attack: 적을 향해 가속도(Velocity)를 주어 이동하고, 공격 범위 밖으로 나가면 공격을 중단합니다.
2. Back: 공격이 끝나면 벡터 연산을 통해 플레이어의 원래 위치로 부드럽게 복귀합니다.

- 원거리 무기(Ranged): 별도의 이동 없이 Idle 상태에서 적을 감지하면 즉시 투사체(CBullet)를 생성(Instantiate)하고 쿨타임을 적용하는 방식으로 구현했습니다.

<details>
<summary>근접 무기 FSM 및 원거리 무기 FSM</summary>
<div markdown="1">

```C++
void CMeleeWeaponScript::OnUpdate(float tDeltaTime) {
    switch (mState) {
    case eState::Idle:
		// 적을 바라보며 플레이어 옆에 계속 위치
		SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		CalculatePosNextToTarget();
        // 적 탐색 및 공격 가능 여부 체크
        CanAttackCheck(CSceneMgr::GetGameObjects(eLayerType::Enemy));
        break;
    case eState::Attack:
        // 공격 범위 이탈 체크
        AttackEndCheck();
        break;
    case eState::Back:
        // 플레이어 위치로 복귀
        BackToPlayer();
        break;
    }
}

void CRangedWeaponScript::OnUpdate(float tDeltaTime)
{
	switch (mState) {
	case eState::Idle:
		SetRotForClosedEnemyWatch(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		CalculatePosNextToTarget();
		CanAttackCheck(CSceneMgr::GetGameObjects(eLayerType::Enemy));
		break;
	default:
		break;
	}
}
```

</div>
</details>  
</br>

#### 2. 수학적 원리를 적용한 거리 계산 최적화
- 적과의 거리를 계산할 때 SetRotForClosedEnemyWatch, CanAttackCheck 등 빈번하게 호출되는 함수에서는 LengthSq() (거리의 제곱)을 비교하는 방식을 사용하여 값비싼 제곱근 연산을 제외하였기에 연산 성능을 최적화 했습니다.  

#### 3. 다형성을 활용한 피격 및 데미지 처리
- 적이 충돌했을 때, 충돌 대상이 근접 무기인지 총알(투사체)인지에 따라 데미지 연산 로직을 분기 처리했습니다.
- DamageByWeapon 함수에서 플레이어의 스탯(근거리/원거리 대미지, 치명타 확률)과 무기 고유 스탯을 통해 각 무기만의 데미지 합산 로직을 이용하여 최종 대미지를 산출하고, 크리티컬 발생 시 텍스트 색상을 변경하는 등 타격감을 위한 연출 로직을 포함했습니다.
  
<details>
<summary>대미지 연산 및 피격 처리 (EnemyScript)</summary>
<div markdown="1">

```C++
void CEnemyScript::DamageByWeapon(GameObject* tWeapon) {
    CPlayerScript* plSc = CPlayScene::GetPlayer()->GetComponent<CPlayerScript>(eComponentType::Script);
    CWeaponScript::SDamageInfo dmgInfo = { 0.0f, false };

    // 무기 타입에 따른 대미지 계산 분기
    if (tWeapon->GetLayerType() == eLayerType::MeleeWeapon) {
        CWeaponScript* wpSc = tWeapon->GetComponent<CWeaponScript>(eComponentType::Script);
        // 플레이어 근거리 대미지 + 무기 대미지 합산
        wpSc->SetDamage(wpSc->GetDamage() + plSc->GetMeleeDamage());
        dmgInfo = wpSc->GetFinalDamage();
    }
    else if (tWeapon->GetLayerType() == eLayerType::Bullet) {
        CBulletScript* blSc = tWeapon->GetComponent<CBulletScript>(eComponentType::Script);
        dmgInfo = blSc->GetFinalDamage(plSc->GetRangedDamage());
    }

    // 체력 감소 및 피격 이펙트 재생
    DecreaseHP(dmgInfo.damage);
    CEffectMgr::PlayEffect(L"EnemyHit", GetOwner()->GetPos(), GetOwner());

    // 크리티컬 여부에 따른 텍스트 색상 변경 (노란색/흰색)
    Gdiplus::Color textColor = dmgInfo.isCritical ? Gdiplus::Color(255, 255, 0) : Gdiplus::Color(255, 255, 255);
    CEffectMgr::ShowEffectText(textPos, std::to_wstring((int)dmgInfo.damage), textColor);
}
```

</div>
</details>
</br>

#### 구현 성과
**1. 안정적인 오토 배틀 구현:** 무기의 상태(State)가 명확히 나뉘어 있어, "공격하러 가는 도중에 복귀해버리는" 등의 로직 충돌 없이 수백 번의 공격 사이클이 안정적으로 돌아가는 것을 확인했습니다.  
**2. 확장 용이한 전투 로직:** DamageByWeapon 함수에서 충돌체의 LayerType만 확인하면 되므로, 추후 새로운 공격 수단이 추가되어도 적 스크립트의 구조 변경을 최소화할 수 있습니다.

## 트러블 슈팅 및 성능 최적화 (Troubleshooting & Optimization)

### 1. 렌더링 파이프라인 최적화: GDI+에서 GDI(HBITMAP)로의 전환

- 문제 상황 (Problem): 초기에는 편리한 GDI+의 DrawImage 함수를 사용하여 모든 오브젝트를 렌더링했습니다. 하지만 투사체와 몬스터 수가 증가하자 CPU 연산량이 급증하며 프레임이 20 FPS 이하로 떨어지는 심각한 병목 현상이 발생했습니다. 프로파일링 결과, DrawImage 내부의 복잡한 연산(포맷 변환, 보간 등)이 주원인임을 파악했습니다.

- 해결 과정 (Solution):

1. GDI 비트맵 캐싱 (HBITMAP Caching): 렌더링 속도가 훨씬 빠른 GDI(BitBlt, TransparentBlt)를 사용하기 위해, 리소스 로딩 단계에서 GDI+ 이미지를 GDI 호환 비트맵(HBITMAP)으로 미리 변환하여 메모리에 캐싱하는 CreateHBitmapFromGdiPlus 함수를 구현했습니다.

2. 픽셀 직접 접근 (LockBits) 및 컬러 키 처리: 반투명 연산 비용을 줄이기 위해 LockBits로 픽셀 데이터에 직접 접근하여, 알파 값이 낮은 픽셀을 특정 색상(Magenta: 255, 0, 255)으로 치환했습니다. 이를 통해 무거운 알파 블렌딩 대신 고속 TransparentBlt을 사용할 수 있게 최적화했습니다.

3. 공간과 시간의 트레이드오프 (Space-Time Tradeoff): 게임 내에서 빈번한 좌우 반전(Flip) 연산을 렌더링 타임에 매번 계산하지 않고, 변환 단계에서 미리 Left/Right 두 가지 비트맵을 생성해두고 상황에 맞춰 교체(SelectObject)하는 방식으로 최적화했습니다.

<details> 
<summary>GDI+ -> HBITMAP 변환 및 최적화 코드</summary> 
<div markdown="1">

```C++
void CTexture::CreateHBitmapFromGdiPlus(bool tbAlpha) {
    // ... (DC 생성 및 초기화) ...

    if (tbAlpha) {
        // 알파 블렌딩이 꼭 필요한 경우 (그대로 HBITMAP 변환)
        Gdiplus::Bitmap* bitmap = (Gdiplus::Bitmap*)mImage;
        bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &mhRightBitmap);
        // 좌우 반전 미리 생성 (Pre-computation)
        bitmap->RotateFlip(Gdiplus::RotateNoneFlipX); 
        bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0), &mhLeftBitmap);
    }
    else {
        // 일반 스프라이트 최적화 (LockBits 활용)
        // ... (Bitmap 생성 및 LockBits) ...

        // 픽셀 순회: 경계선 문제 해결 및 컬러 키(Magenta) 적용
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int idx = (i * stride) + (j * 4);
                BYTE a = srcBits[idx + 3];

                // 알파값이 낮으면 투명색(Magenta)으로 치환 -> TransparentBlt 활용 가능
                if (a < 128) {
                    destBits[idx + 0] = 255; // B
                    destBits[idx + 1] = 0;   // G
                    destBits[idx + 2] = 255; // R
                    destBits[idx + 3] = 255; // A
                }
                else {
                    // 원본 색상 복사
                    destBits[idx + 0] = srcBits[idx + 0];
                    destBits[idx + 1] = srcBits[idx + 1];
                    destBits[idx + 2] = srcBits[idx + 2];
                    destBits[idx + 3] = 255;
                }
            }
        }
        // ... (UnlockBits 및 좌우 반전 비트맵 생성) ...
    }
}
```
</div> 
</details> 
</br>

- 결과 (Result): 렌더링 방식을 전환한 후, 100개 이상의 오브젝트가 동시에 등장하는 상황에서도 90 FPS를 안정적으로 방어했습니다. 특히 DrawImage 호출 횟수를 줄이고, BitBlt 계열 함수를 사용하여 렌더링 부하를 최소화한 것이 성능 향상의 핵심이었습니다.

### 2. 씬(Scene) 생명주기를 활용한 리소스 관리

- 문제 상황 (Problem): 게임 규모가 커지면서 캐릭터 선택창, 상점 등 특정 상황에서만 쓰이는 무거운 리소스들이 씬 전환 후에도 메모리에 남아 불필요한 메모리 점유율을 높이는 문제가 있었습니다.

- 해결 과정 (Solution): 모든 씬(CScene)에 OnEnter(진입)와 OnExit(퇴장) 가상 함수를 설계하여 씬의 생명주기를 관리했습니다. OnExit 호출 시 해당 씬 전용 텍스처와 UI 오브젝트를 식별하여 vector에서 제거하고 소멸자를 호출, 즉시 메모리에서 해제(Release)하도록 구조화했습니다.

- 결과 (Result): 씬을 여러 번 오가도 메모리 사용량이 누적되지 않고 일정하게 유지(Stable Memory Footprint)되어, 장시간 플레이 시에도 안정성을 확보했습니다.

### 3. 구조적 안정성: 지연 처리를 통한 크래시 방지

- 문제 상황 (Problem): 게임 로직(OnUpdate) 실행 도중 오브젝트가 파괴(Destroy)되거나 UI가 닫히면서, 순회 중이던 vector나 list의 Iterator가 무효화(Invalidation) 되어 메모리 접근 위반(Crash)이 발생하는 문제가 있었습니다.

- 해결 과정 (Solution): 이벤트 큐(Event Queue) 패턴을 도입하여 삭제 로직을 분리했습니다.

1. OnUpdate 도중에는 mDeleteQueue에 삭제 요청만 등록합니다.

2. 모든 로직 업데이트가 끝난 후, 프레임의 마지막 단계에서 큐를 비우며 실제 메모리 해제와 컨테이너 제거를 일괄 처리합니다.

- 결과 (Result): 객체의 생명주기 관리 로직이 명확해졌으며, 복잡한 상호작용(예: 몬스터가 죽으면서 자신을 때린 투사체를 삭제) 상황에서도 크래시 없는 안정적인 엔진 구동을 확보했습니다.

### 4. 렌더링 최적화: 타일맵 베이킹(Baking)을 통한 드로우 콜 최소화
- 문제 상황 (Problem): 맵의 크기가 커짐에 따라 바닥을 구성하는 타일 오브젝트가 수백 개 이상으로 늘어났습니다. 이를 매 프레임 개별적으로 렌더링(DrawImage 호출)하다 보니, 오브젝트 순회와 함수 호출 오버헤드(Draw Call)가 발생하여 프레임 드랍의 원인이 되었습니다.

- 해결 과정 (Solution): 타일맵 베이킹(Tilemap Baking) 기법을 도입했습니다.

1. 맵 로딩 시점(OnEnter)에 맵 크기만한 거대한 빈 비트맵을 메모리에 생성합니다.

2. 파일에서 읽어온 수백 개의 타일 정보를 이용해, GDI+ Graphics 객체로 빈 비트맵 위에 타일들을 한 번에 그려 넣습니다(Merge).

3. 인게임 루프(OnUpdate)에서는 수백 개의 타일 객체를 순회하는 대신, 미리 완성된(Baked) 단 한 장의 배경 이미지만 렌더링합니다.

- 결과 (Result): 수백 번 발생하던 타일 렌더링 연산을 단 1회로 줄여 렌더링 부하를 획기적으로 감소시켰으며, 맵 스크롤 시에도 끊김 없는 부드러운 화면 전환을 구현했습니다.


<details> 
<summary>타일맵 베이킹(LoadBakedMap, RandomBakedMap) 코드</summary> 
<div markdown="1">

```C++
// [Map Baking] 파일에서 읽어온 타일 정보를 하나의 비트맵으로 병합하여 드로우 콜 최소화
void CPlayScene::LoadBakedMap(const wchar_t* tPath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, tPath, L"rb");

    // 1. 맵 전체 크기에 맞는 거대한 빈 텍스처(캔버스) 생성
    CTexture* mBakedMapImg = CTexture::Create(L"BakedBG", mapWidth, mapHeight);
    
    // 2. 텍스처에 그리기 위한 GDI+ Graphics 객체 생성
    Gdiplus::Graphics graphics(mBakedMapImg->GetImage());
    CTexture* tileTex = CToolScene::GetMapTileTexture();

    // 3. 파일에서 타일 데이터를 읽어와 캔버스에 '굽기(Bake)'
    while (true) {
        int idxX = 0, idxY = 0, posX = 0, posY = 0;
        
        if (fread(&idxX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&idxY, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posX, sizeof(int), 1, pFile) == NULL) break;
		if (fread(&posY, sizeof(int), 1, pFile) == NULL) break;

        int srcX = idxX * tileSizeX;
        int srcY = idxY * tileSizeY;

        // 개별 객체 생성 없이, 이미지에 직접 그려 넣음
        graphics.DrawImage(tileTex->GetImage(), 
            Gdiplus::Rect(posX, posY, tileSizeX, tileSizeY), 
            srcX, srcY, tileSizeX, tileSizeY, 
            Gdiplus::UnitPixel);
    }
    fclose(pFile);

    // 4. 외곽선 처리 및 최적화를 위한 HBITMAP 변환
    OutLineFill(&graphics, tileSizeX, tileSizeY);
    mBakedMapImg->CreateHBitmapFromGdiPlus(false);
    
    // 완성된 배경 텍스처 적용
    mBakedMap->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer)->SetTexture(mBakedMapImg);
}

// [Procedural Generation] 런타임에 랜덤 맵 생성 및 베이킹
void CPlayScene::RandomBakedMap()
{
    // 1. 랜덤 타일셋 텍스처 로드
    int randTile = std::rand() % 5 + 1;
    std::wstring tileName = L"Tile" + std::to_wstring(randTile);
    CTexture* randomMapTex = CResourceMgr::Find<CTexture>(tileName);

    // 2. 빈 텍스처 및 Graphics 객체 준비
    CTexture* mBakedMapImg = CTexture::Create(L"BakedBG", mapWidth, mapHeight);
    Gdiplus::Graphics graphics(mBakedMapImg->GetImage());

    int tileCountWidth = mapWidth / tileSizeX;
    int tileCountHeight = mapHeight / tileSizeY;

    // 3. 절차적 생성 알고리즘 (Procedural Logic)
    for (int i = 0; i < tileCountWidth; i++) {
        for (int j = 0; j < tileCountHeight; j++) {
            
            // 90% 확률로 기본 타일, 10% 확률로 장식 타일 배치
            int randNum = std::rand() % 10;
            int srcX = 0, srcY = 0;

            if (randNum > 0) {
                // 기본 타일 좌표 설정
                srcX = (tileMapWidth - 1) * tileSizeX;
                srcY = (tileMapHeight - 1) * tileSizeY;
            }
            else {
                // 장식 타일 랜덤 선택 로직
                // ... (while문으로 유효한 장식 타일 인덱스 탐색) ...
            }
            
            // 계산된 타일을 캔버스에 그리기
            graphics.DrawImage(randomMapTex->GetImage(),
                Gdiplus::Rect(i * tileSizeX, j * tileSizeY, tileSizeX, tileSizeY),
                srcX, srcY, tileSizeX, tileSizeY,
                Gdiplus::UnitPixel);
        }
    }

    // 4. 마무리 처리 (외곽선 및 HBITMAP 변환)
    OutLineFill(&graphics, tileSizeX, tileSizeY);
    mBakedMapImg->CreateHBitmapFromGdiPlus(false);
    
    mBakedMap->GetComponent<CSpriteRenderer>(eComponentType::SpriteRenderer)->SetTexture(mBakedMapImg);
}
```

</div> 
</details> 
</br>

## 회고
### 1. "작동하는 코드"에서 "설계된 아키텍처"로의 전환
개발 초기에는 빠른 기능 구현에 집중하여 하드코딩을 하기도 했으나, 프로젝트 규모가 커질수록 유지보수가 어려워지는 기술적 부채(Technical Debt)를 경험했습니다. 이를 해결하기 위해 개발 중간에 클래스 다이어그램을 재설계하고 컴포넌트 패턴으로 리팩토링하는 과정을 거쳤습니다. 이 경험을 통해 "좋은 설계가 곧 개발 속도와 직결된다"는 것을 체감했으며, 확장 가능한 엔진 아키텍처를 설계하는 능력을 길렀습니다.

### 2. 메모리 관리와 C++의 깊이 있는 이해
C++ WinAPI 환경에서 직접 엔진을 바닥부터 구현하며, 가비지 컬렉터(GC) 없는 환경에서의 메모리 관리 중요성을 뼈저리게 느꼈습니다. 단순한 nullptr 체크를 넘어, 씬 전환 시의 리소스 해제 시점 관리, vector의 재할당 문제, 스마트 포인터의 필요성 등을 고민하며 안정성(Stability) 높은 프로그램을 만드는 데 집중했습니다.

### 3. 프로파일링 기반의 문제 해결 능력
프레임 저하 문제를 겪었을 때 막연히 코드를 수정하는 것이 아니라, 병목 지점이 '렌더링 파이프라인'임을 분석하고 GDI+에서 GDI로 전환하거나 **자료구조(Vector → Map/Enum)** 를 변경하는 등 논리적인 접근으로 문제를 해결했습니다. 이러한 최적화 경험은 앞으로 마주할 더 복잡한 성능 이슈들도 해결할 수 있다는 자신감을 주었습니다.

## 마치며
이번 브로테이토 모작 프로젝트는 단순히 기능을 구현하는 것을 넘어, **"수많은 오브젝트를 어떻게 효율적으로 관리할 것인가"** 에 대한 해답을 찾아가는 과정이었습니다.

- 컴포넌트 패턴을 통해 객체 지향적인 설계의 유연함을 배웠고,

- 데이터 주도 설계를 통해 기획 데이터와 로직을 분리하는 생산성 높은 파이프라인을 구축했으며,

- FSM과 렌더링 최적화를 통해 실제 게임 플레이가 가능한 수준의 퍼포먼스를 확보했습니다.

이 과정에서 마주친 수많은 LNK 오류와 메모리 누수, 프레임 저하 문제를 끈기 있게 해결하며 **"문제를 해결하는 프로그래머"** 로서 한 단계 성장할 수 있었습니다. 앞으로도 더 깊이 있는 최적화와 탄탄한 설계를 고민하는 개발자가 되겠습니다.