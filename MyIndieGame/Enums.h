#pragma once

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

enum class eLayerType {
	None,
	BackGround,
	Tile,
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
	None,

	// TitleScene
	TitleSceneUI,

	// SettingScene
	CharacterSelectUI,
	WeaponSelectUI,
	ShopUI,

	// PlayScene
	PlaySceneUI,

	// EndingScene,
	EndingSceneUI,

	// Commonness
	OptionUI,

	End
};