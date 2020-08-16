





#ifndef __MAP_HEAD__
#define __MAP_HEAD__


#include <string>
#include <vector>


namespace Fly
{

	namespace MUD
	{


	  class Node
	  {
	  public:
	    typedef enum __node_type__
	      {
	       NONE,
	       BUILDING,
	      } NodeType;

	    typedef enum __node_print_way__
	      {
	       UP,
	       DOWN,
	       LEFT,
	       RIGHT
	      } NodePrintWay;

	  private:
	    ::std::string name;
	    NodeType type = NONE;
	    
	  public:
	    
	    Node *up;
	    Node *down;
	    Node *left;
	    Node *right;

	    void(*arrive_event)(Node*) = [](Node *node){return;};    //到达事件
	    void(*keyboard_event)(Node*) = [](Node *node){return;};  //键盘事件
	    void(*enter_event)(Node*) = [](Node *node){return;};     //回车事件
	    
	    Node(::std::string &name,Node::NodeType type,
		 Node *up = NULL,Node *down = NULL,Node *left = NULL,Node *right = NULL);
	    ~Node();

	    ::std::string get_name(){return this->name;};

	    void print();
	  };

	  

	  class Map
	  {
	  private:
	    
	  public:
	    Node *center;
	    
	    
	    Map();
	    ~Map();

	    void print();
	  };


	} // namespace MUD

  


    
  
  
};// namespace Fly






#endif // __MAP_HEAD__





