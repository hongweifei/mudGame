






#include "game_ui.hpp"



namespace Fly
{
  
  namespace MUD
  {
    
    namespace UI
    {
      /**
       * 0 player information window
       * 1 other information window
       * 2 information window
       * 3 graphics window
       * 4 control window
       */
      static ::WINDOW *win[5] = {NULL,NULL,NULL,NULL,NULL};
      enum
	{
	  PLAYER_INFORMATION_WINDOW = 0,
	  OTHER_INFORMATION_WINDOW,
	  INFORMATION_WINDOW,
	  GRAPHICS_WINDOW,
	  CONTROL_WINDOW,
	  WINDOW_NUMBER
	};
      
      static ::PANEL *player_information = NULL; //玩家信息面板
      static ::PANEL *other_information = NULL;  //其他信息面板
      static ::PANEL *information = NULL;        //信息面板
      static ::PANEL *graphics = NULL;           //图像面板
      static ::PANEL *control = NULL;            //控制面板
      
      
      static int16_t stdscr_width = 0;
      static int16_t stdscr_height = 0;
      
      
      static Character *player = NULL;
		
      
      void init(Character *c)
      {
	Console::init();
	::curs_set(0);
		
	stdscr_width = Console::get_width();
	stdscr_height = Console::get_height();
	int width = stdscr_width;
	int height = stdscr_height;
	
	win[0] = ::newwin(height / 4, width / 4, 0, 0);
	win[1] = ::newwin(height / 4, width / 4, 0, width / 4);
	win[2] = ::newwin(height / 3, width, height / 3 * 2, 0);
	win[3] = ::newwin(height / 20 * 9.5, width / 2, height / 4, 0);
	win[4] = ::newwin(height / 3 * 2, width / 2, 0, width / 2);
	
	player_information = ::new_panel(win[PLAYER_INFORMATION_WINDOW]);
	other_information = ::new_panel(win[OTHER_INFORMATION_WINDOW]);
	information = ::new_panel(win[INFORMATION_WINDOW]);
	graphics = ::new_panel(win[GRAPHICS_WINDOW]);
	control = ::new_panel(win[CONTROL_WINDOW]);
		
	
	for (int i = 0;i < WINDOW_NUMBER;i++)
	  {
	    ::box(win[i],0,0);
	  }
	
	
	player = c;
      }
      
      
      
      void release()
      {
	::del_panel(player_information);
	::del_panel(other_information);
	::del_panel(information);
	::del_panel(graphics);
	::del_panel(control);

	for (int i = 0;i < WINDOW_NUMBER;i++)
	  ::delwin(win[i]);
	
	::endwin();
      }
      
      
      void updata()
      {
	int16_t width = stdscr_width;
	int16_t height = stdscr_height;
	Console::get_size(width, height);
	
	if (width == stdscr_width && height == stdscr_height)
	  return;
	
	stdscr_width = width;
	stdscr_height = height;
	
	::wresize(win[PLAYER_INFORMATION_WINDOW], height / 4, width / 4);
	::wresize(win[OTHER_INFORMATION_WINDOW], height / 4, width / 4);
	::wresize(win[INFORMATION_WINDOW], height / 3, width);
	::wresize(win[GRAPHICS_WINDOW], height / 20 * 9.5, width / 2);
	::wresize(win[CONTROL_WINDOW], height / 3 * 2, width / 2);
	
	::mvwin(win[PLAYER_INFORMATION_WINDOW], 0, 0);
	::mvwin(win[OTHER_INFORMATION_WINDOW], 0, width / 4);
	::mvwin(win[INFORMATION_WINDOW], height / 3 * 2, 0);
	::mvwin(win[GRAPHICS_WINDOW], height / 4, 0);
	::mvwin(win[CONTROL_WINDOW], 0, width / 2);

	//Console::clear();
	::wclear(stdscr);
	for (int i = 0;i < WINDOW_NUMBER;i++)
	  {
	    ::wclear(win[i]);
	    ::box(win[i],0,0);
	  }
      }
		
      
      void draw_main()
      {
	updata();
	
	int x = 2;
	int y = 2;
	if (stdscr_width <= 80)
	  x = 1;
	if (stdscr_height <= 40)
	  y = 1;
	
	
	::mvwprintw(win[PLAYER_INFORMATION_WINDOW],y,x,"Name:%s",player->get_name().c_str());
	::mvwprintw(win[PLAYER_INFORMATION_WINDOW],y * 2,x,"Level:%d",player->level);
	::mvwprintw(win[PLAYER_INFORMATION_WINDOW],y * 3,x,"HP:%d",player->get_hp());
	::mvwprintw(win[PLAYER_INFORMATION_WINDOW],y * 4,x,"ATK:%d",player->get_atk());
      }
      
      
      
    } // namepsace UI
    
    
  } // namespace MUD
  
  
} // namespace Fly



