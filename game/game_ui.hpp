






#ifndef NCURSES
#define NCURSES
#endif // NCURSES


#ifndef __GAME_UI_HEAD__
#define __GAME_UI_HEAD__


#include "../console.hpp"
#include <panel.h>


namespace Fly
{

  namespace MUD
  {


    namespace UI
    {
      #define ERROR_TEXT_COLOR Console::FOREGROUND_red|Console::BACKGROUND_BLACK
      #define WARNING_TEXT_COLOR Console::FOREGROUND_YELLOW|Console::BACKGROUND_BLACK
		  

      void init();

      void draw_main();
      
      
    } // namespace UI

    
    
  } // namespace MUD
  
  
  
} // namespace Fly











#endif // __GAME_UI_HEAD__



