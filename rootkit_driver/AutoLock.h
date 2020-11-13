#pragma once
#include <Ntifs.h>
template<typename TLock>
struct AutoLock {
	AutoLock(TLock& lock) : m_lock(lock) {
		m_lock.Lock();
	}

	~AutoLock() {
		m_lock.Unlock();
	}

private:
	TLock& m_lock;
};
