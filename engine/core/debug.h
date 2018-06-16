#pragma once
/**
	@file		debug.h
	
	This file contains a number of useful debugging functions, print functions, etc.

	@copyright	See LICENSE file
*/

#ifdef __LINUX__
void _printf(const char*, ...) __attribute__((format(printf,1,2)));
void _error(const char*, ...) __attribute__((format(printf,1,2)));
void _warning(const char*, ...) __attribute__((format(printf,1,2)));
void _confirm(const char*, ...) __attribute__((format(printf,1,2)));
void _sleep(double);
void _exception(const char *, const char *, int);
void _exception2(const char*, const char*, const char*, int);
void _exception_fmt(const char *, const char *, const char *, int, ...);
void _break();
#else
void _printf(const char*, ...);
void _error(const char*, ...);
void _warning(const char*, ...);
void _confirm(const char*, ...);
void _sleep(double);
void _exception(const char *, const char *, int);
void _exception2(const char*, const char*, const char*, int);
void _exception_fmt(const char *, const char *, const char *, int, ...);
void _break();
#endif

#if __VORTEX_NO_ASSERT__
#define _assert(exp) if(!(exp)){}
#define _assert2(exp, msg) if(!(exp)){}
#define _assert_fmt(exp, msg, ...) if(!(exp)){}
#else
#define _assert(exp) { if (!(exp)) _exception(#exp,__FILE__,__LINE__); }
#define _assert2(exp, msg) { if (!(exp)) _exception2(#exp,msg,__FILE__,__LINE__); }
#define _assert_fmt(exp, msg, ...) { if (!(exp)) _exception_fmt(#exp,msg,__FILE__,__LINE__, __VA_ARGS__); }
#endif