






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
      static WINDOW *win[5] = {NULL,NULL,NULL,NULL,NULL};
      enum
	{
	  PLAYER_INFORMATION_WINDOW = 0,
	  OTHER_INFORMATION_WINDOW,
	  INFORMATION_WINDOW,
	  GRAPHICS_WINDOW,
	  CONTROL_WINDOW,
	  WINDOW_NUMBER
	};
      
      static PANEL *player_information = NULL; //玩家信息面板
      static PANEL *other_information = NULL;  //其他信息面板
      static PANEL *information = NULL;        //信息面板
      static PANEL *graphics = NULL;           //图像面板
      static PANEL *control = NULL;            //控制面板


      static Character *player = NULL;

      
      void init(Character *c)
      {
	Console::init();
	
	int width = Console::get_width();
	int height = Console::get_height();

	
	win[0] = newwin(height / 4, width / 4, 0, 0);
	win[1] = newwin(height / 4, width / 4, 0, width / 4);
	win[2] = newwin(height / 3, width, height / 3 * 2, 0);
	win[3] = newwin(height / 20 * 9.5, width / 2, height / 4, 0);
	win[4] = newwin(height / 3 * 2, width / 2, 0, width / 2);
	
	player_information = new_panel(win[PLAYER_INFORMATION_WINDOW]);
	other_information = new_panel(win[OTHER_INFORMATION_WINDOW]);
	information = new_panel(win[INFORMATION_WINDOW]);
	graphics = new_panel(win[GRAPHICS_WINDOW]);
	control = new_panel(win[CONTROL_WINDOW]);


	for (int i = 0;i < WINDOW_NUMBER;i++)
	  {
	    box(win[i],0,0);
	  }
	

	player = c;
      }



      void release()
      {
	for (int i = 0;i < WINDOW_NUMBER;i++)
	  delwin(win[i]);
	
	del_panel(player_information);
	del_panel(other_information);
	del_panel(information);
	del_panel(graphics);
	del_panel(control);
	endwin();
      }


      void draw_main()
      {

	mvwprintw(win[PLAYER_INFORMATION_WINDOW],2,2,"Name:%s",player->get_name().c_str());
	mvwprintw(win[PLAYER_INFORMATION_WINDOW],4,2,"Level:%d",player->level);
	mvwprintw(win[PLAYER_INFORMATION_WINDOW],6,2,"HP:%d",player->get_hp());
	mvwprintw(win[PLAYER_INFORMATION_WINDOW],8,2,"ATK:%d",player->get_atk());
      }


      
    } // namepsace UI
    
    
  } // namespace MUD
  
  
} // namespace Fly



