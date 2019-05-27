#ifndef HW4_SCOUT_H
#define HW4_SCOUT_H

#include"Player.h"

class Scout : public Player{
  /**
   * Attack damage 25
   * Heal power 0
   * Max HP 125
   * Goal Priorities -> {CHEST,TO_ALLY,ATTACK} in decreasing order
   * Class abbreviation -> "sc" or "SC"
   * Can move to all adjacent squares, including diagonals.
   * Can attack all adjacent squares, including diagonals.
   *
   */

public:

	Scout(unsigned id, int x, int y, Team team);

	int getAttackDamage() const override { return damage; }
	int getHealPower() const override { return healPower; }
	int getMaxHP() const override { return maxHP; }

	std::vector<Coordinate> getAttackableCoordinates() override;
	std::vector<Coordinate> getMoveableCoordinates() override;

};

#endif
