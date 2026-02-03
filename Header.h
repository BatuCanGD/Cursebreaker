#pragma once

class GameMaster { // to keep track of the player's actions.
public:
	static int turn_amount;
	static void increment_turn();
};