#ifndef LOGGING_H
#define LOGGING_H
#pragma warning(disable : 4996)

#include <chrono>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

#define NOMINMAX
#include <windows.h>


static std::string GetTimeString(){
    std::chrono::time_point<std::chrono::system_clock> now;
    now = std::chrono::system_clock::now();
 
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
 	
    std::string toReturn = std::ctime(&now_time);

	//std::put_time(std::localtime(&now_c), "%F %T")
    toReturn.pop_back();
    return toReturn;
}
#define OUTPUT_NEWLINE() OutputDebugString("\n");

#define REGISTERCLASS(c) static constexpr const char* ClassName = #c;

#define PRINTCLASSNAME std::cout << "[" << ClassName << "::" << __func__ << "] "

#define PRINTCLASSTIME std::cout << GetTimeString() << " : [" << ClassName << "::" << __func__ << " (" << __LINE__ << ") ] "

#define OUTPUT_STRING(s) std::cout << GetTimeString() << " : " << s;

//#define OUTPUT_STRING_LINE(s) std::cout << GetTimeString() << " : " << s << std::endl;
#define OUTPUT_STRING_LINE(s) OutputDebugString(s.c_str()); OUTPUT_NEWLINE();

//#define PRINTPAIR(v) { PRINTCLASSTIME << #v << ": x=" << v.first << " y=" << v.second << "\n"; }
#define PRINTPAIR(v) OutputDebugString("Implement PRINTPAIR!"); OUTPUT_NEWLINE();

//#define PRINTVEC2(v) { PRINTCLASSTIME << #v << ": x=" << v.x << " y=" << v.y << "\n"; }
#define PRINTVEC2(v) OutputDebugString("Implement PRINTVEC2!"); OUTPUT_NEWLINE();

//#define PRINTVEC2VECTOR(v) { for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y << "\n"; }
#define PRINTVEC2VECTOR(v) OutputDebugString("Implement PRINTVEC2VECTOR!"); OUTPUT_NEWLINE();

//#define PRINTVEC3(v) { PRINTCLASSTIME << #v << ": x=" << v.x << " y=" << v.y << " z=" << v.z << "\n"; }
#define PRINTVEC3(v) OutputDebugString("Implement PRINTVEC3!"); OUTPUT_NEWLINE();

//#define PRINTVEC3VECTOR(v) { for (int _index_ = 0; _index_ < v.size(); _index_++) PRINTCLASSTIME << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y  << v[_index_].z << "\n"; }
#define PRINTVEC3VECTOR(v) OutputDebugString("Implement PRINTVEC3VECTOR!"); OUTPUT_NEWLINE();

//#define PRINTVEC4(v) { PRINTCLASSTIME << #v << ": x=" << v.x << " y=" << v.y << " z=" << v.z << " w=" << v.w << "\n"; }
#define PRINTVEC4(v) OutputDebugString("Implement PRINTVEC4!"); OUTPUT_NEWLINE();

//#define PRINTINTVECTOR(v) { for (int _index_ = 0; _index_ < v.size(); _index_++) PRINTCLASSTIME << #v << "[" << _index_ << "]: x=" << v[_index_] << "\n"; }
#define PRINTINTVECTOR(v) OutputDebugString("Implement PRINTINTVECTOR!"); OUTPUT_NEWLINE();

//#define PRINT(f) { PRINTCLASSTIME << #f << ": " << f << "\n"; }
#define PRINT(f) OutputDebugString(f); OUTPUT_NEWLINE();

//#define PRINTSTRING(f) { PRINTCLASSTIME << f << "\n"; }
#define PRINTSTRING(f) OutputDebugString(f.c_str()); OUTPUT_NEWLINE();

//#define PRINTBOOL(B) { string b = B==1?"TRUE":"FALSE"; std::cout << #B << " : " << b << "\n"; }
#define PRINTBOOL(v) OutputDebugString("Implement PRINTBOOL!"); OUTPUT_NEWLINE();

//#define PRINTINT(i) { PRINTCLASSTIME << #i << ": x=" << i << "\n"; }
#define PRINTINT(v) OutputDebugString("Implement PRINTINT!"); OUTPUT_NEWLINE();

//#define PRINTVECTOR(e) { std::cout << "Element Vector: " << #e << "\n"; \
//                       for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_] << "\n"; }
#define PRINTVECTOR(v) OutputDebugString("Implement PRINTVECTOR!"); OUTPUT_NEWLINE();

//#define PRINTELEMENT(e) { std::cout << #e << " " << e.toString() << "\n"; []
#define PRINTELEMENT(v) OutputDebugString("Implement PRINTELEMENT!"); OUTPUT_NEWLINE();

//#define PRINTELEMENTPTR(e) { std::cout << #e << " " << e->toString() << "\n "; }
#define PRINTELEMENTPTR(v) OutputDebugString("Implement PRINTELEMENTPTR!"); OUTPUT_NEWLINE();

//#define PRINTELEMENTVECTOR(e) { std::cout << "Element Vector: " << #e << "\n"; \
//                              for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_]->toString() << "\n"; }
#define PRINTELEMENTVECTOR(v) OutputDebugString("Implement PRINTELEMENTVECTOR!"); OUTPUT_NEWLINE();

#define lxLogMessage(f) { OutputDebugString(f); OUTPUT_NEWLINE(); }

#endif