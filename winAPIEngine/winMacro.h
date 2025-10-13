#pragma once

#define windowWidth 1600
#define windowHeight 800

#define SAFE_DELETE(p) \
if (p != nullptr) \
{\
	delete p; \
	p = nullptr; \
}