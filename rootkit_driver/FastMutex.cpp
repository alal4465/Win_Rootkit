#include "FastMutex.h"

void FastMutex::Init() {
	ExInitializeFastMutex(&mutex_);
}

void FastMutex::Lock() {
	ExAcquireFastMutex(&mutex_);
}

void FastMutex::Unlock() {
	ExReleaseFastMutex(&mutex_);
}
