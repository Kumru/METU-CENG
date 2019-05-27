#ifndef HW4_TANK_H
#define HW4_TANK_H

#include"Player.h"

class Tank : public Player{
  /**
   * Attack damage 25
   * Heal power 0
   * Max HP 1000
   * Goal Priorities -> {TO_ENEMY,ATTACK,CHEST} in decreasing order
   * Class abbreviation -> "ta" or "TA"
   * Can move to adjacent up, down, left or right square
   * Can attack to adjacent up, down, left or right square
   *
   */

public:

	Tank(unsigned id, int x, int y, Team team);

	int getAttackDamage() const override { return damage; }
	int getHealPower() const override { return healPower; }
	int getMaxHP() const override { return maxHP; }

	std::vector<Coordinate> getAttackableCoordinates() override;
	std::vector<Coordinate> getMoveableCoordinates() override;

};

#endif
