#pragma once

enum class eComponentType {
	Transform,
	SpriteRenderer,
	Script,
	Camera,
	End
};

enum class eLayerType {
	None,
	BackGround,
	Player,
	MAX = 16
};

enum class eResourceType {
	Texture,
	// Mesh,
	// Shader
	AudioClip,
	Prefab,
	End
};