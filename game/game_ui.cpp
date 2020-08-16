






#include "game_ui.hpp"





namespace Fly
{

  namespace MUD
  {

    namespace UI
    {
      
      static PANEL *player_information; //玩家信息面板
      static PANEL *other_information;  //其他信息面板
      static PANEL *information;        //信息面板
      static PANEL *graphics;           //图像面板
      static PANEL *control;            //控制面板
      

      void init()
      {
	Console::init();

	int width = Console::get_width();
	int height = Console::get_height();
	
	WINDOW *player_information_window = newwin(height / 4, width / 4, 0, 0);
	WINDOW *other_information_window = newwin(height / 4, width / 4, 0, width / 4);
	WINDOW *information_window = newwin(height / 3, width, height / 3 * 2, 0);
	WINDOW *graphics_window = newwin(height / 20 * 9, width / 2, height / 4, 0);
	WINDOW *control_window = newwin(height / 3 * 2, width / 2, 0, width / 2);

	player_information = new_panel(player_information_window);
	other_information = new_panel(other_information_window);
	information = new_panel(information_window);
	graphics = new_panel(graphics_window);
	control = new_panel(control_window);
      }

      
      
      
    } // namepsace UI

    
  } // namespace MUD

  
} // namespace Fly



