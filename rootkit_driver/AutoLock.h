#pragma once
#include <Ntifs.h>

template<typename TLock>
struct AutoLock {
	AutoLock(TLock& lock) : lock_(lock) {
		lock_.Lock();
	}

	~AutoLock() {
		lock_.Unlock();
	}

private:
	TLock& lock_;
};
