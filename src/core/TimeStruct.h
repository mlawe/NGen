#ifndef __TIMESTRUCT__
#define __TIMESTRUCT__
#include <iostream>

struct Time_Struct{
  int64_t seconds;
  int64_t nanoseconds;

  Time_Struct();
  Time_Struct(int64_t,int64_t);
  void Zero();
  //void SetTime(double t);
  void Check();

  
  Time_Struct& operator+=(const Time_Struct & t);
  Time_Struct& operator-=(const Time_Struct &t);
  Time_Struct operator+(const Time_Struct & t);
  Time_Struct operator-(const Time_Struct & t);
  bool operator==(const Time_Struct& t);
  bool operator!=(const Time_Struct& t);
  bool operator>(const Time_Struct&t);  
  bool operator<(const Time_Struct &t);
};

#endif

