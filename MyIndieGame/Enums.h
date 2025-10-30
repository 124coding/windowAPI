#pragma once

enum class eComponentType {
	Transform,
	Collider,
	Rigidbody,
	Script,
	SpriteRenderer,
	AutoSpriteFlipper,
	TileRenderer,
	Animator,
	Camera,
	AudioListner,
	AudioSource,
	End
};

enum class eLayerType {
	None,
	BackGround,
	Tile,
	Enemy,
	Player,
	Bullet,
	MeleeWeapon,
	RangedWeapon,
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

enum class eUIType {
	HPBar,
	EXPBar,
	StartButton,
	Button,
	End
};