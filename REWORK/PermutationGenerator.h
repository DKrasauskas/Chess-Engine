#include "Random.h"
#include "Table.h"
#include "MoveGenerator.h"

#define REMOVEBIT(bitboard, position) (bitboard &= ~((Bitboard)1 << position);
#define ADDBIT(bitboard, position)    bitboard |= ((Bitboard)1 << position);

using namespace Moves;

namespace Blockers {
	Bitboard variations[4096];
	int pos = 0;
	void Generate(Bitboard moves, Bitboard permutation) {
		unsigned long index;
		if (moves == 0) {
			variations[pos] = permutation;
			pos++;
			return;
		}
		while (_BitScanForward64(&index, moves)) {
			moves &= ~((Bitboard)1 << index);
			Generate(moves, permutation);
			permutation |= ((Bitboard)1 << index);
			Generate(moves, permutation);
			break;
		}
	}
	void Print(uint64_t variable) {
		for (int i = 0; i < 64; i++) {
			std::cout << ((variable >> (i)) & 1);
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55)std::cout << std::endl;
		}
		std::cout << "\n\n";
	}
	void GeneratePermutations(int type, int position) {
		unsigned long index;
		Bitboard moves;
		Bitboard permutation = 0;
		switch (type){
		case 0:
			moves = Gen_Bishop_notrail(position, 0, 0, 0, index, 0);
			break;

		case 1:
			moves = Gen_Rook_notrail(position, 0, 0, 0, index, 0);			
			break;

		case 2:
			moves = Gen_Forward_LEGAL_PAWNW(position, 0, 0, 0, index ,0);		
			break;
		case 3:
			moves = Gen_Forward_LEGAL_PAWNB(position, 0 ,0, 0, index, 0);
			
		    break;
		}	
		while (_BitScanForward64(&index, moves)) {
			moves &= ~((Bitboard)1 << index);
			Generate(moves, permutation);
			permutation |= ((Bitboard)1 << index);
			Generate(moves, permutation);
			break;
		}
	}
	void AllUnique() {
		for (int i = 0; i < pos; i++) {
			for (int j = 0; j < pos; j++) {
				if (variations[i] == variations[j] && i != j) {
					std::cout << "failed\n";
					break;
				}
			}
		}
		return;
	}
}
