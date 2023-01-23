#pragma once

#include <mutex>

enum class EThreadMode
{
	NotThreadSafe,
	ThreadSafe,
};

template
<
	typename object,
	EThreadMode mode
>
class SharedPtr
{
public:
	FORCEINLINE SharedPtr();


private:

};