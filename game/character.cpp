




#include "character.hpp"






namespace Fly
{

	namespace MUD
	{

	  Equipment::Equipment(EquipmentType type, Quality quality, ::std::string name, uint32_t atk, uint32_t hp, uint32_t level)
	  {
	    this->type = type;
	    this->quality = quality;
	    this->name = name;
	    this->atk = atk;
	    this->hp = hp;
	    this->level = level;


	    
	    

	    
	    this->atk_formula = [](Equipment *e){

				  uint32_t atk = e->get_atk(true);
				  switch(e->get_type())
				    {
				    case EQUIPMENT_WEAPON:
				      return atk * e->get_quality() + e->get_quality() * (e->level - 1);
				    case EQUIPMENT_ARMOR:
				      return (atk * e->get_quality() + e->get_quality() * (e->level - 1)) / 2;
				    case EQUIPMENT_SHOES:
				      return (atk * e->get_quality() + e->get_quality() * (e->level - 1)) / 3;
				    }
					return atk;
				};
	    
	    this->hp_formula = [](Equipment *e){
				 uint32_t hp = e->get_hp(true);
				 
				 switch(e->get_type())
				   {
				   case EQUIPMENT_WEAPON:
				     return hp * e->get_quality() + e->get_quality() * (e->level - 1) / 3;
				   case EQUIPMENT_ARMOR:
				     return hp * e->get_quality() + e->get_quality() * (e->level - 1);
				   case EQUIPMENT_SHOES:
				     return hp * e->get_quality() + e->get_quality() * (e->level - 1) / 2;
				   }
				   return hp;
			    };

	  }
	  

	  Equipment::~Equipment()
	  {
	    
	  }




	  


	  

	  Character::Character(::std::string name, uint32_t atk, uint32_t hp, uint32_t level)
	  {
	    this->name = name;
	    this->atk = atk;
	    this->hp = hp;
	    this->level = level;

	    this->atk_formula = [](Character *c){
				  uint32_t equipment_atk = 0;
				  if (c->weapon != NULL)
				    equipment_atk += c->weapon->get_atk(false);
				  if (c->armor != NULL)
				    equipment_atk += c->armor->get_atk(false);
				  if (c->shoes != NULL)
				    equipment_atk += c->shoes->get_atk(false);

				  return c->get_atk(true) + (c->quality + c->level);
				};

	    this->hp_formula = [](Character *c){
				 uint32_t equipment_hp = 0;
				 if (c->weapon != NULL)
				   equipment_hp += c->weapon->get_hp(false);
				 if (c->armor != NULL)
				   equipment_hp += c->armor->get_hp(false);
				 if (c->shoes != NULL)
				   equipment_hp += c->shoes->get_hp(false);

				 return c->get_hp(true) + (c->quality + c->level);
			    };
	  }


	  Character::~Character()
	  {
	    
	  }

	} // namespace MUD

  
  
} // namespace Fly




