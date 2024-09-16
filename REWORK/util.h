#pragma once


namespace PositionObject {
	struct Move {
		int origin;
		Bitboard moves;
	};
	struct King {
		int pos;
		int attacker;
	};
	struct Iboard {
		Move* moves;
		int size;
		void Rescale() {

		}
		void Erase() {

		}
		
	};
}