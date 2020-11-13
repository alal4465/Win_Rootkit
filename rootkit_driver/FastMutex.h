#pragma once
#include <Ntifs.h>

class FastMutex {
public:
	void Init();
	void Lock();
	void Unlock();

private:
	FAST_MUTEX m_mutex;
};
