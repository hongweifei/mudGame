




#include "console.hpp"
#include "game/character.hpp"
#include "game/game_ui.hpp"
#include <stdio.h>

using namespace Fly;
using namespace Fly::MUD;

int main()
{
  
  Character c1("C1",10,100,1);
  printf("Name:%s\nLevel:%d\nHP:%d\nATK:%d\n",c1.get_name().c_str(),c1.level,c1.get_hp(),c1.get_atk());
  
  ::UI::init(&c1);

  while(getch() != 'q')
    {
      ::UI::draw_main();
      update_panels();
      doupdate();
      //::Console::refresh();
    }
  
  ::UI::release();
  
  return 0;
}












