#pragma once
#include <vector>
#include <GameEngine/Math.h>
#include <cassert>

class Health {
   private:
	std::vector<int> health;
	int healthPerBar = 0;

   public:
	Health() {}
	Health(int healthBars, int healthPerBar) : healthPerBar(healthPerBar) {
		SetHealth(healthBars, healthPerBar);
	}
	~Health() { health.clear(); }

	void SetHealth(int healthBars, int healthPerBar) {
		this->healthPerBar = healthPerBar;

		health.clear();
		for (int i = 0; i < healthBars; i++) {
			health.push_back(healthPerBar);
		}
	}

	void TakeDamage(int damage) {
		for (int i = (int)health.size()-1; i >= 0 && damage > 0; i--) {
			if (health[i] > 0) {
				int damageTaken = GameEngine::Math::Min(health[i], damage);
				health[i] -= damageTaken;
				damage -= damageTaken;
			}
		}
	}

	void Heal(int healing) {
		for (int i = 0; i < health.size() && healing > 0; i++) {
			int spaceInBar = healthPerBar - health[i];

			if (spaceInBar > 0) {
				int heal = GameEngine::Math::Min(spaceInBar, healing);
				health[i] += heal;
				healing -= heal;
			}
		}
	}

	void AddHealthBar() {
		health.push_back(0);
		Heal(healthPerBar);
	}

	int GetTotalHealth() const {
		int totalHealth = 0;

		for (int i = 0; i < health.size(); i++) {
			totalHealth += health[i];
		}
		return totalHealth;
	}

	int GetHealthBars() const {
		int bars = 0;
		for (int i = 0; i < health.size() && health[i] > 0; i++, bars++) {}
		return bars;
	}

	int GetHealthPerBar() const { return healthPerBar; }

	bool IsEmpty() { return health.size() > 0 && health[0] == 0; }
	bool IsFull() {
		return health.size() > 0 && health[health.size() - 1] == healthPerBar;
	}
};