







#include "map.hpp"
#include "../console.hpp"


using namespace Fly;

namespace FlyMUD
{
  

  Node::Node(::std::string &name,Node::NodeType type,Node *up,Node *down,Node *left,Node *right)
  {
    this->name = name;
    this->type = type;
    this->up = up;
    this->down = down;
    this->left = left;
    this->right = right;
  }

  Node::~Node()
  {
    delete this->up;
    delete this->down;
    delete this->left;
    delete this->right;
  }

  
  void Node::print()
  {
    if (this == NULL)
      return;

    Console::print("-");                      // -
    Console::print(this->name.c_str());       // -name

    uint16_t x,y;
    Console::get_cursor_position(x,y);

    x = x - this->name.length() / 2;
    y--;                                      //    |
    Console::print(x,y,"|");                  // -name

    x--;
    y += 2;                                   //    |
    Console::print(x,y,"|");                  // -name
                                              //    |
    x += this->name.length() / 2;
    y++;                                      //    |
    Console::print("-");                      // -name-
  }                                           //    |

  
};//namespace FlyMUD






