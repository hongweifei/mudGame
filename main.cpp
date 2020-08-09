





#include "game/character.hpp"

#include <stdio.h>

using namespace Fly;

int main()
{
  
  Character c1("C1",10,100,1);
  
  printf("Name:%s\nLevel:%d\nHP:%d\nATK:%d\n",c1.get_name().c_str(),c1.level,c1.get_hp(),c1.get_atk());
  return 0;
}












