


#include "console.hpp"
#include <memory.h>
#include <string.h>


namespace FlyConsole
{
    

    Console::Console()
    {
        #ifndef NCURSES

            #if WIN32

            system("chcp 65001");
            system("cls");

            this->buffer1 = ::GetStdHandle(STD_OUTPUT_HANDLE);
            this->buffer2 = ::CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,1,NULL);
            this->backstage_buffer = this->buffer1;

            #endif // WIN32

        #else
        

        this->window = ::initscr();
        ::keypad(this->window, TRUE);
        ::noecho();
        ::cbreak();                         //传回 'a' 'b' 之类的值

        ::start_color();

        #ifndef WIN32
        this->color_init();
        #endif // WIN32
        

        #endif // NCURSES

        //this->updata();
    }


    Console::~Console()
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


    void Console::color_init()
    {
        #ifdef NCURSES

        for (short i = 0; i < 8; i++)
        {
            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_BLACK,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_BLACK);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_red,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_RED);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_green,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_GREEN);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_YELLOW,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_YELLOW);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_blue,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_BLUE);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_MAGENTA,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_MAGENTA);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_CYAN,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_CYAN);

            this->color_count++;
            this->color.insert(::std::map<short,short>::value_type(i|BACKGROUND_WHITE,this->color_count));
            ::init_pair(this->color_count,get_ncurses_color(i),COLOR_WHITE);
        }

        #endif // NCURSES
    }


    uint32_t Console::get_width()
    {
        this->updata();
        return this->width;
    }


    uint32_t Console::get_height()
    {
        this->updata();
        return this->height;
    }


    void Console::get_size(int16_t &w,int16_t &h)
    {
        this->updata();
        w = this->width;
        h = this->height;
    }


    void Console::get_cursor_position(int16_t &x,int16_t &y)
    {
        this->updata();

        #ifndef NCURSES

            #if WIN32

            x = this->info->dwCursorPosition.X;
            y = this->info->dwCursorPosition.Y;

            #endif // WIN32

        #else

        getyx(this->window,y,x);

        #endif // NCURSES
    }



    void Console::set_size(int16_t w,int16_t h)
    {
        #ifndef NCURSES

            #if WIN32

            ::COORD size = {w,h};
            ::SetConsoleScreenBufferSize(this->buffer1,size);
            ::SetConsoleScreenBufferSize(this->buffer2,size);

            #endif // WIN32

        #else

        #endif // NCURSES
    }

    void Console::set_cursor_position(int16_t x,int16_t y)
    {
        #ifndef NCURSES

            #if WIN32

            ::COORD position = {x,y};
            ::SetConsoleCursorPosition(this->backstage_buffer,position);

            #endif // WIN32

        #else

        ::wmove(this->window, y, x);

        #endif // NCURSES
    }

    void Console::move_cursor(int16_t x,int16_t y)
    {
        #ifndef NCURSES

            #if WIN32

            ::COORD position = {x,y};
            ::SetConsoleCursorPosition(this->backstage_buffer,position);

            #endif // WIN32

        #else

        ::wmove(this->window, y, x);

        #endif // NCURSES
    }

    void Console::clear()
    {
        #ifndef NCURSES

            #if WIN32

            char text[this->width];
            char *clear_text = (char*)memset(text,' ',this->width * sizeof(char));

            this->move_cursor(0,0);
            for (size_t i = 0; i < this->height; i++)
            {
                ::WriteConsoleA(this->backstage_buffer, clear_text, this->width, NULL, NULL);
            }
            this->move_cursor(0,0);

            /** 释放内存 */
            delete clear_text;

            #endif // WIN32

        #else

        

        #endif // NCURSES

        this->updata();
    }

    void Console::refresh()
    {
        #ifndef NCURSES

            #if WIN32

            // 后台是buffer1
            if (this->backstage_buffer == this->buffer1)
            {
                ::SetConsoleActiveScreenBuffer(this->buffer1);    // 将buffer1换到前台
                this->backstage_buffer = this->buffer2;         // 将后台改为buffer2
            }

            // 后台是buffer2
            else
            {
                ::SetConsoleActiveScreenBuffer(this->buffer2);    // 将buffer2换到前台
                this->backstage_buffer = this->buffer1;         // 将后台改为buffer1
            }
            
            #endif // WIN32

        #else

        ::wrefresh(this->window);

        #endif // NCURSES
        
        this->updata();
    }


    void Console::echo(bool b)
    {
        #ifdef NCURSES

        if (b)
            ::echo();
        else
            ::noecho();

        #endif // NCURSES
    }


    void Console::set_getch_way(enum __getch_way__ way)
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


    int Console::get_ch()
    {
        #ifndef NCURSES

            #if WIN32

            return ::_getch();

            #endif // WIN32

        #else

        return ::getch();

        #endif // NCURSES
    }


    void Console::set_text_color(short color)
    {
        #ifndef NCURSES

            #if WIN32

            ::SetConsoleTextAttribute(this->backstage_buffer,color);

            #endif // WIN32

        #else

        ::attron(COLOR_PAIR(this->color[color]));

        #endif // NCURSES
    }


    void Console::print(const char *text)
    {
        #ifndef NCURSES

            #if WIN32

            ::WriteConsoleA(this->backstage_buffer,text,strlen(text) * sizeof(char),NULL,NULL);

            #endif // WIN32

        #else

        wprintw(this->window, text);

        #endif // NCURSES
    }


    void Console::print(int16_t x,int16_t y,const char *text)
    {
        #ifndef NCURSES

            #if WIN32

            this->move_cursor(x,y);
            ::WriteConsoleA(this->backstage_buffer,text,strlen(text) * sizeof(char),NULL,NULL);

            #endif // WIN32

        #else

        mvwprintw(this->window, y, x, text);

        #endif // NCURSES
    }


} // namespace FlyConsole










