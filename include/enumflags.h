#ifndef ENUMFLAGS_H
#define ENUMFLAGS_H

#define DEFINE_ENUM_FLAG_OPERATORS(T) \
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
} \

#endif //ENUMFLAGS_H