#pragma once

#define windowWidth 672
#define windowHeight 846

#define SAFE_DELETE(p) \
if (p != nullptr) \
{\
	delete p; \
	p = nullptr; \
}