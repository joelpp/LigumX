#pragma once

#if defined(_DEBUG)

	#define lxAssert(condition) if (!condition) __debugbreak();
	#define lxAssert0() lxAssert(false);

#else

	#define lxAssert(condition)
	#define lxAssert()

#endif