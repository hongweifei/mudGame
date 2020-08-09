




#ifndef __MUD_CHARACTER_HEAD__
#define __MUD_CHARACTER_HEAD__


#include <stdint.h>
#include <string>




namespace Fly
{

  //品质
  typedef enum __quality__ : uint8_t
    {
     QUALITY_GREEN = 1,		//绿色
     QUALITY_BLUE,		//蓝色
     QUALITY_PURPLE,		//紫色
     QUALITY_ORANGE,		//橙色
     QUALITY_RED,		//红色
     QUALITY_GOLD		//金色
    } Quality;

  
  //装备类型
  typedef enum __equipment_type__ : uint8_t
    {
     EQUIPMENT_WEAPON,
     EQUIPMENT_ARMOR,
     EQUIPMENT_SHOES,
    } EquipmentType;


  //装备
  class Equipment
  {
  private:
    EquipmentType type;//装备的类型
    Quality quality;//装备的品质
    ::std::string name;//装备的名字

    uint32_t atk;//初始攻击力
    uint32_t hp;//初始生命值
  public:
    uint32_t level;//装备的等级

    uint32_t(*atk_formula)(Equipment*);
    uint32_t(*hp_formula)(Equipment*);

    
    Equipment(EquipmentType, Quality, ::std::string, uint32_t atk = 0, uint32_t hp = 0, uint32_t level = 1);
    ~Equipment();


    ::std::string get_name() {return this->name;};
    EquipmentType get_type() {return this->type;};
    Quality get_quality() {return this->quality;};


    /**
     *  默认获取计算后的
     * @return false获取计算后的,true获取原始的
     */
    uint32_t get_atk(bool initial = false)
    {
      if (initial)
	      return this->atk;
      return this->atk_formula(this);
    };

    
    /**
     *  默认获取计算后的
     * @return false获取计算后的,true获取原始的
     */
    uint32_t get_hp(bool initial = false)
    {
      if(initial)
	      return this->hp;
      return this->hp_formula(this);
    };
    
    
    /**
     * 设置atk(攻击力)的计算式
     */
    void set_atk_formula(uint32_t(*atk)(Equipment*))
    {
      this->atk_formula = atk;
    };

			 
    /**
     * 设置hp(生命值)的计算式
     */
    void set_hp_formula(uint32_t(*hp)(Equipment*))
    {
      this->hp_formula = hp;
    };

    
  };

  

  //人物
  class Character
  {
  private:
    ::std::string name;//名字

    uint32_t atk;//攻击力(一级时)
    uint32_t hp;//生命值(一级)

  public:
    uint32_t level;//等级
    Quality quality;//品质
    

    Equipment *weapon = NULL;//武器
    Equipment *armor = NULL;//防具
    Equipment *shoes = NULL;//鞋子


    uint32_t (*atk_formula)(Character*);
    uint32_t (*hp_formula)(Character*);
    

    Character(::std::string, uint32_t atk, uint32_t hp, uint32_t level = 1);
    ~Character();


    ::std::string get_name() { return this->name; };
    
    uint32_t get_atk(bool initial = false)
    {
      if (initial)
	      return this->atk;
      return this->atk_formula(this);
    };
    
    uint32_t get_hp(bool initial = false)
    {
      if (initial)
        return this->hp;
      return this->hp_formula(this);
    };
  };




}




#endif



