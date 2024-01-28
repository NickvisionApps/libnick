#ifndef ENUMFLAGS_H
#define ENUMFLAGS_H

#include <type_traits>

#define DEFINE_ENUM_FLAG_OPERATORS(T) \
static_assert(std::is_enum_v<T> == true); \
inline T operator~(T a) \
{ \
	return (T)~(int)a; \
} \
inline T operator|(T a, T b) \
{ \
	return (T)((int)a | (int)b); \
} \
inline T operator&(T a, T b) \
{ \
	return (T)((int)a & (int)b); \
} \
inline T operator^(T a, T b) \
{ \
	return (T)((int)a ^ (int)b); \
} \
inline T& operator|=(T& a, T b) \
{ \
	return (T&)((int&)a |= (int)b); \
} \
inline T& operator&=(T& a, T b) \
{ \
	return (T&)((int&)a &= (int)b); \
} \
inline T& operator^=(T& a, T b) \
{ \
	return (T&)((int&)a ^= (int)b); \
}

#endif //ENUMFLAGS_H