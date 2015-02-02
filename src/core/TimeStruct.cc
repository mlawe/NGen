#include "TimeStruct.h"
Time_Struct::Time_Struct()
{
  seconds=0;
  nanoseconds=0;
}

Time_Struct::Time_Struct(int64_t s,int64_t n)
{
  seconds=s;
  nanoseconds=n;
  
  Check();

}

void Time_Struct::Zero()
{
  seconds=0;
  nanoseconds=0;

}

void Time_Struct::Check()
{
  //  std::cout<<"Check"<<std::endl;
  //std::cout<<"seconds "<<seconds<<" nanoseconds "<<nanoseconds<<std::endl;
  while(nanoseconds>=1000000000)
    {
      seconds+=1;
      nanoseconds-=1000000000;
      
    }

  //now handle negatives
  while(nanoseconds<=-1000000000)
    {
      seconds-=1;
      nanoseconds+=1000000000;

    }
  if(nanoseconds<0 and seconds>0)
    {
      seconds-=1;
      nanoseconds=1000000000+nanoseconds;
      Check();
    }
  if(nanoseconds>0 and seconds<0)
    {
      seconds+=1;
      nanoseconds=-1000000000+nanoseconds;
      Check();
    }
}

Time_Struct& Time_Struct::operator+=(const Time_Struct & t)
{
  seconds+=t.seconds;
  nanoseconds+=t.nanoseconds;

  Check();

  return *this;

}


Time_Struct& Time_Struct::operator-=(const Time_Struct &t)
{
  seconds-=t.seconds;
  nanoseconds-=t.nanoseconds;

  Check();

  return *this;
}


Time_Struct Time_Struct::operator+(const Time_Struct & t)
{
  Time_Struct result=*this;
  result+=t;

  return result;



}


Time_Struct Time_Struct::operator-(const Time_Struct & t)
{
  Time_Struct result=*this;
  result-=t;


  return result;

}


bool Time_Struct::operator==(const Time_Struct& t)
{

  return (seconds==t.seconds and nanoseconds==t.nanoseconds);

}

bool Time_Struct::operator!=(const Time_Struct& t)
{
  return !(*this==t);


}

bool Time_Struct::operator>(const Time_Struct&t)
{
  if(seconds!=t.seconds)
    return seconds>t.seconds;
  
  return nanoseconds>t.nanoseconds;
}

bool Time_Struct::operator<(const Time_Struct &t)
{
  return !(*this>t) and !(*this==t);

}
