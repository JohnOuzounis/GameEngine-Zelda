#pragma once
#include <functional>

class Spell {
   public:
	   using OnActivate = std::function<void(void)>;
	   using OnDeactivate = std::function<void(void)>;

	  private:
	   bool activated = false;
	   OnActivate activate;
	   OnDeactivate deactivate;

	   int cost = 0;

	  public:
	   void SetOnActivate(OnActivate a) { activate = a; }
	   void SetOnDeactivate(OnDeactivate d) { deactivate = d; }

	   int GetCost() const { return cost; }
	   void SetCost(int points) { cost = points; }

	   bool IsActivated() const { return activated; }

	   void Activate() {
		   activated = true;
		   if (activate)
			   (activate)();
	   }

	   void Deactivate() {
		   activated = false;
		   if (deactivate)
			   (deactivate)();
	   }
};