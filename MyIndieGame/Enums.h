#pragma once

enum class eComponentType {
	Transform,
	SpriteRenderer,
	Animator,
	Script,
	Camera,
	End
};

enum class eLayerType {
	None,
	BackGround,
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