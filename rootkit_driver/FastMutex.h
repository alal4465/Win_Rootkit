#pragma once
#include <Ntifs.h>

class FastMutex {
public:
	void Init();
	void Lock();
	void Unlock();

private:
	FAST_MUTEX mutex_;
};
