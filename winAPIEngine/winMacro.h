#pragma once

#define windowWidth 1280
#define windowHeight 900

#define SAFE_DELETE(p) \
if (p != nullptr) \
{\
	delete p; \
	p = nullptr; \
}