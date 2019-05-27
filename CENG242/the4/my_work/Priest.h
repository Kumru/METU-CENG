#ifndef HW4_PRIEST_H
#define HW4_PRIEST_H

#include"Player.h"

class Priest : public Player{
  /**
   * Attack damage 0
   * Heal power 50
   * Max HP 150
   * Goal Priorities -> {HEAL,TO_ALLY,CHEST} in decreasing order
   * Class abbreviation -> "pr" or "PR"
   * Can move to all adjacent squares, including diagonals.
   * Can heal all adjacent squares, including diagonals.
   *
   */

public:

	Priest(unsigned id, int x, int y, Team team);

	int getAttackDamage() const override { return damage; }
	int getHealPower() const override { return healPower; }
	int getMaxHP() const override { return maxHP; }

	std::vector<Coordinate> getMoveableCoordinates() override;
	std::vector<Coordinate> getHealableCoordinates() override;

};

#endif
