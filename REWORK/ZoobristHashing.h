


namespace TTable {
	struct Entry {
		int bestmove;
		int eval;
		bool tag;
		int depth;
		Bitboard hash;
		unsigned int HashFunc(int size) {
			return hash % size;
		}
	};
	struct Table {
		Entry* data;
		int size;
		int position = 0;
		unsigned int HashFunc(Bitboard key) {
			return key % size;
		}
	};
}

namespace Zobrist {

	Bitboard hashtable[12][64];
	Bitboard value = 0;
	unsigned long index;
	void _Create_() {
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 12; j++) {
				hashtable[j][i] = random_fewbits();
			}
		}
	}
	void _INIT_() {
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 12; j++) {
				hashtable[j][i] = 0;
			}
		}
	}
	Bitboard UpHash(int piece_type, int index) {
		return hashtable[piece_type][index];
	}
	TTable::Entry GetHashFull(BoardData::Board board){
		
		for (int i = 0; i < board.sizeB; i++) {
			value ^= hashtable[board.piecesB[i].type + 6][board.hashtable[i]];
		}
		for (int j = 0; j < board.sizeW; j++) {
			value ^= hashtable[board.piecesW[j].type][board.hashtable[j]];
		}
		TTable::Entry entry;
		entry.hash = value;
		return entry;
	}
}

