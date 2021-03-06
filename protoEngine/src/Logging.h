#ifndef LOGGING_H
#define LOGGING_H

#include <chrono>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

static std::string GetTimeString(){
    std::chrono::time_point<std::chrono::system_clock> now;
    now = std::chrono::system_clock::now();
 
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
 	
    std::string toReturn = std::ctime(&now_time);
    toReturn.pop_back();
    return toReturn;
}

#define REGISTERCLASS(c) static constexpr const char* _ClassName = #c;

#define PRINTCLASSNAME std::cout << "[" << _ClassName << "::" << __func__ << "] "

#define PRINTCLASSTIME std::cout << GetTimeString() << " : [" <<_ClassName<< "::" << __func__ << " (" << __LINE__ << ") ] "



#define PRINTPAIR(v) { PRINTCLASSTIME << #v << ": x=" << v.first << " y=" << v.second << "\n"; }

#define PRINTVEC2(v) { PRINTCLASSTIME << #v << ": x=" << v.x << " y=" << v.y << "\n"; }

#define PRINTVEC2VECTOR(v) { for (int _index_ = 0; _index_ < v.size(); _index_++) std::cout << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y << "\n"; }

#define PRINTVEC3(v) { PRINTCLASSTIME << #v << ": x=" << v.x << " y=" << v.y << " z=" << v.z << "\n"; }

#define PRINTVEC3VECTOR(v) { for (int _index_ = 0; _index_ < v.size(); _index_++) PRINTCLASSTIME << #v << "[" << _index_ << "]: x=" << v[_index_].x << " y=" << v[_index_].y  << v[_index_].z << "\n"; }

#define PRINTINTVECTOR(v) { for (int _index_ = 0; _index_ < v.size(); _index_++) PRINTCLASSTIME << #v << "[" << _index_ << "]: x=" << v[_index_] << "\n"; }

#define PRINT(f) { PRINTCLASSTIME << #f << ": " << f << "\n"; }

#define PRINTSTRING(f) { PRINTCLASSTIME << f << "\n"; }

#define PRINTBOOL(B) { string b = B==1?"TRUE":"FALSE"; std::cout << #B << " : " << b << "\n"; }

#define PRINTINT(i) { PRINTCLASSTIME << #i << ": x=" << i << "\n"; }

#define PRINTVECTOR(e) { std::cout << "Element Vector: " << #e << "\n"; \
                       for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_] << "\n"; }

#define PRINTELEMENT(e) { std::cout << #e << " " << e.toString() << "\n"; []

#define PRINTELEMENTPTR(e) { std::cout << #e << " " << e->toString() << "\n "; }

#define PRINTELEMENTVECTOR(e) { std::cout << "Element Vector: " << #e << "\n"; \
                              for (int _index_ = 0; _index_ < e.size(); _index_++) std::cout << _index_ << ": " << e[_index_]->toString() << "\n"; }

#endif