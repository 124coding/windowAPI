#pragma once

enum class eComponentType {
	Transform,
	Collider,
	Script,
	SpriteRenderer,
	Animator,
	Camera,
	End
};

enum class eLayerType {
	None,
	BackGround,
	Animal,
	Player,
	Particle,
	MAX = 16
};

enum class eResourceType {
	Texture,
	// Mesh,
	// Shader
	AudioClip,
	Animation,
	Prefab,
	End
};

enum class eColliderType {
	Circle2D,
	Rect2D,
	End
};