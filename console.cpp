





#include "console.hpp"
#include <memory.h>
#include <string.h>



namespace Fly
{
    
    namespace Console
    {

        static uint32_t width;
        static uint32_t height;

        #ifndef NCURSES

            #if WIN32

            static HANDLE backstage_buffer;            // 要修改的（后台）
            static HANDLE buffer1;
            static HANDLE buffer2;

            static CONSOLE_SCREEN_BUFFER_INFO *info;

            #endif // Win32

        #else

        static ::WINDOW *window;
        static ::std::map<short,short> color;
        static ::uint16_t color_count = 8;

        #endif // NCURSES


        

        void updata()
        {
            #ifndef NCURSES

                #if WIN32

                GetConsoleScreenBufferInfo(backstage_buffer,info);
                width = info->dwSize.X;
                height = info->dwSize.Y;

                #endif // WIN32

            #else

            getmaxyx(window, height, width);

            #endif // NCURSES
        }




        short get_ncurses_color(short color)
        {
            #ifdef NCURSES

            switch (color)
            {
            case FOREGROUND_BLACK:
                return COLOR_BLACK;
            case FOREGROUND_red:
                return COLOR_RED;
            case FOREGROUND_green:
                return COLOR_GREEN;
            case FOREGROUND_YELLOW:
                return COLOR_YELLOW;
            case FOREGROUND_blue:
                return COLOR_BLUE;
            case FOREGROUND_MAGENTA:
                return COLOR_MAGENTA;
            case FOREGROUND_CYAN:
                return COLOR_CYAN;
            case FOREGROUND_WHITE:
                return COLOR_WHITE;
            
            default:
                return 0;
            }
            
            #endif // NCURSES

            return 0;
        }



        void init()
        {
            #ifndef NCURSES

                #if WIN32

                system("chcp 65001");
                system("cls");

                buffer1 = ::GetStdHandle(STD_OUTPUT_HANDLE);
                buffer2 = ::CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,1,NULL);
                backstage_buffer = buffer1;

                #endif // WIN32

            #else
            

            window = ::initscr();
            ::keypad(window, TRUE);
            ::noecho();
            ::cbreak();                         //传回 'a' 'b' 之类的值

            ::start_color();

            #ifndef WIN32
            color_init();
            #endif // WIN32
            

            #endif // NCURSES

            //updata();
        }


        void exit()
        {
            #ifndef NCURSES

                #if WIN32

                ::CloseHandle(this->buffer1);
                ::CloseHandle(this->buffer2);
                //delete(this->info);

                #endif // WIN32

            #else

            ::endwin();

            #endif // NCURSES
        }



        void color_init()
        {
            #ifdef NCURSES

            for (short i = 0; i < 8; i++)
            {
                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_BLACK,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_BLACK);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_red,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_RED);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_green,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_GREEN);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_YELLOW,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_YELLOW);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_blue,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_BLUE);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_MAGENTA,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_MAGENTA);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_CYAN,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_CYAN);

                color_count++;
                color.insert(::std::map<short,short>::value_type(i|BACKGROUND_WHITE,color_count));
                ::init_pair(color_count,get_ncurses_color(i),COLOR_WHITE);
            }

            #endif // NCURSES
        }


        uint32_t get_width()
        {
            updata();
            return width;
        }


        uint32_t get_height()
        {
            updata();
            return height;
        }


        void get_size(int16_t &w,int16_t &h)
        {
            updata();
            w = width;
            h = height;
        }


        void get_cursor_position(int16_t x,int16_t y)
        {
            updata();

            #ifndef NCURSES

                #if WIN32

                x = info->dwCursorPosition.X;
                y = info->dwCursorPosition.Y;

                #endif // WIN32

            #else

            getyx(window,y,x);

            #endif // NCURSES
        }



        void set_size(int16_t w,int16_t h)
        {
            #ifndef NCURSES

                #if WIN32

                ::COORD size = {w,h};
                ::SetConsoleScreenBufferSize(buffer1,size);
                ::SetConsoleScreenBufferSize(buffer2,size);

                #endif // WIN32

            #else

            #endif // NCURSES
        }

        void set_cursor_position(int16_t x,int16_t y)
        {
            #ifndef NCURSES

                #if WIN32

                ::COORD position = {x,y};
                ::SetConsoleCursorPosition(backstage_buffer,position);

                #endif // WIN32

            #else

            ::wmove(window, y, x);

            #endif // NCURSES
        }

        void move_cursor(int16_t x,int16_t y)
        {
            #ifndef NCURSES

                #if WIN32

                ::COORD position = {x,y};
                ::SetConsoleCursorPosition(backstage_buffer,position);

                #endif // WIN32

            #else

            ::wmove(window, y, x);

            #endif // NCURSES
        }

        void clear()
        {
            #ifndef NCURSES

                #if WIN32

                char text[width];
                char *clear_text = (char*)memset(text,' ',width * sizeof(char));

                move_cursor(0,0);
                for (size_t i = 0; i < height; i++)
                {
                    ::WriteConsoleA(backstage_buffer, clear_text, width, NULL, NULL);
                }
                move_cursor(0,0);

                /** 释放内存 */
                delete clear_text;

                #endif // WIN32

            #else

            

            #endif // NCURSES

            updata();
        }

        void refresh()
        {
            #ifndef NCURSES

                #if WIN32

                // 后台是buffer1
                if (backstage_buffer == buffer1)
                {
                    ::SetConsoleActiveScreenBuffer(buffer1);    // 将buffer1换到前台
                    backstage_buffer = buffer2;         // 将后台改为buffer2
                }

                // 后台是buffer2
                else
                {
                    ::SetConsoleActiveScreenBuffer(buffer2);    // 将buffer2换到前台
                    backstage_buffer = buffer1;         // 将后台改为buffer1
                }
                
                #endif // WIN32

            #else

            ::wrefresh(window);

            #endif // NCURSES
            
            updata();
        }


        void echo(bool b)
        {
            #ifdef NCURSES

            if (b)
                ::echo();
            else
                ::noecho();

            #endif // NCURSES
        }


        void set_getch_way(enum __getch_way__ way)
        {
            #ifdef NCURSES

            switch (way)
            {
            case RAW:
                ::raw();            //传回的是 KEY_F(n) KEY_DOWN 之类;不会产生阻塞
                break;
            case CBREAK:
                ::cbreak();         //传回 'a' 'b' 之类的值
                break;
            
            default:
                ::cbreak();         //传回 'a' 'b' 之类的值
                break;
            }

            #endif // NCURSES
        }


        int get_ch()
        {
            #ifndef NCURSES

                #if WIN32

                return ::_getch();

                #endif // WIN32

            #else

            return ::getch();

            #endif // NCURSES
        }


        void set_text_color(short color)
        {
            #ifndef NCURSES

                #if WIN32

                ::SetConsoleTextAttribute(backstage_buffer,color);

                #endif // WIN32

            #else

            ::attron(COLOR_PAIR(Console::color[color]));

            #endif // NCURSES
        }


        void print(const char *text)
        {
            #ifndef NCURSES

                #if WIN32

                ::WriteConsoleA(backstage_buffer,text,strlen(text) * sizeof(char),NULL,NULL);

                #endif // WIN32

            #else

            wprintw(window, text);

            #endif // NCURSES
        }


        void print(int16_t x,int16_t y,const char *text)
        {
            #ifndef NCURSES

                #if WIN32

                move_cursor(x,y);
                ::WriteConsoleA(backstage_buffer,text,strlen(text) * sizeof(char),NULL,NULL);

                #endif // WIN32

            #else

            mvwprintw(window, y, x, text);

            #endif // NCURSES
        }

    };


    

} // namespace FlyConsole










