#pragma once

#define PI 3.141592f

#define windowWidth 1280
#define windowHeight 900

#define mapWidth 1600
#define mapHeight 1600

#define tileSizeX 64.0f
#define tileSizeY 64.0f

#define DegToRad(p) ((p) * (PI / 180.0f))
#define RadToDeg(p) ((p) * (180.0f / PI))

#define SAFE_DELETE(p) \
if (p != nullptr) \
{\
	delete p; \
	p = nullptr; \
}