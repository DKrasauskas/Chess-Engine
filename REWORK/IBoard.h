

#define WRITEBIT(bitboard, index) bitboard |= ((Bitboard)1 << index)
#define CLEARBIT(bitboard, index) bitboard &= ~((Bitboard)1 << index)
#define TOGGLEBIT(bitboard, index) bitboard ^= ((Bitboard)1 << index)


namespace Pieces {
	struct Piece {
		Bitboard moves;
		Bitboard(*move_generation)(int, Bitboard, Bitboard, Bitboard);
		int position;
		int type; // corresponds to value
		bool white;
		bool captured = 0;
		void Update(Bitboard occupied) {				
			moves = move_generation(position, occupied, bishop_hash[position], rook_hash[position]);
		}
	};
}


namespace BoardData {
	struct Board {
		Bitboard ID;
		int values[6] = { 71, 293, 300, 456, 905, 90000 };
		unsigned long index;
		unsigned long indexs;
		int eval = 0;
		bool tag = 1;
		bool white_in_check = 0;
		bool black_in_check = 0;
		int whiteking;
		int blackking; //it is worth tracking their squares
		bool castlingW;
		bool castlingB;
		//piece data
		Pieces::Piece* piecesW;
		Pieces::Piece* piecesB;
		//captures and quiet moves
		Bitboard* capturesW;
		Bitboard* capturesB;
		Bitboard* quietW;
		Bitboard* quietB;
		int sizeW, sizeB;
		int hashtable[64];
		//occupied pieces
		Bitboard occupied;
		Bitboard white;
		Bitboard black;


		void _Print(uint64_t variable) {
			for (int i = 0; i < 64; i++) {
				std::cout << ((variable >> (i)) & 1);
				if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55)std::cout << std::endl;
			}
			std::cout << "\n\n";
		}
		void do_move_capture(int piece_index, int destination, int& captured_index) {
			Bitboard recalc;
			if (tag) {
				CLEARBIT(occupied, piecesW[piece_index].position);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesW[piece_index].position, occupied, bishop_hash[piecesW[piece_index].position], rook_hash[piecesW[piece_index].position]) | knight[piecesW[piece_index].position]);
				TOGGLEBIT(white, destination);
				TOGGLEBIT(black, destination);
				TOGGLEBIT(white, piecesW[piece_index].position);
				captured_index = hashtable[destination];
				piecesB[captured_index].captured = 1;
				eval += values[piecesB[captured_index].type]; // this is for eval
				if (piecesW[piece_index].type == 5) {
					whiteking = destination;
				}
				piecesW[piece_index].position = destination;
				piecesW[piece_index].Update(occupied);
				capturesW[piece_index] = piecesW[piece_index].moves & black;
				hashtable[destination] = piece_index;
			}
			else {
				CLEARBIT(occupied, piecesB[piece_index].position);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesB[piece_index].position, occupied, bishop_hash[piecesB[piece_index].position], rook_hash[piecesB[piece_index].position]) | knight[piecesB[piece_index].position]);
				TOGGLEBIT(white, destination);
				TOGGLEBIT(black, destination);
				TOGGLEBIT(black, piecesB[piece_index].position);
				captured_index = hashtable[destination];
				piecesW[captured_index].captured = 1;
				eval -= values[piecesW[captured_index].type]; // this is for eval
				if (piecesB[piece_index].type == 5) {
					blackking = destination;
				}
				piecesB[piece_index].position = destination;
				piecesB[piece_index].Update(occupied);
				capturesB[piece_index] = piecesB[piece_index].moves & white;
				hashtable[destination] = piece_index;
			}	
			bool update = 0;
			Bitboard data = recalc & white;
			while (_BitScanForward64(&indexs, data)) {
				piecesW[hashtable[indexs]].Update(occupied);
				if (piecesW[hashtable[indexs]].type == 5)update = 1;
				capturesW[hashtable[indexs]] = piecesW[hashtable[indexs]].moves & black;
				quietW[hashtable[indexs]] = (white & piecesW[hashtable[indexs]].moves) ^ (piecesW[hashtable[indexs]].moves ^ capturesW[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			data = recalc & black;
			while (_BitScanForward64(&indexs, data)) {
				piecesB[hashtable[indexs]].Update(occupied);
				if (piecesB[hashtable[indexs]].type == 5)update = 1;
				capturesB[hashtable[indexs]] = piecesB[hashtable[indexs]].moves & white;
				quietB[hashtable[indexs]] = (black & piecesB[hashtable[indexs]].moves) ^ (piecesB[hashtable[indexs]].moves ^ capturesB[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			if (update)HandleChecks();
		}
		void undo_move_capture(int piece_index, int destination, int captured_index) {
			Bitboard recalc;	
			
			if (tag) {			
				TOGGLEBIT(occupied, destination);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesW[piece_index].position, occupied, bishop_hash[piecesW[piece_index].position], rook_hash[piecesW[piece_index].position]) | knight[piecesW[piece_index].position]);
				TOGGLEBIT(white, destination);
				TOGGLEBIT(black, piecesW[piece_index].position);
				TOGGLEBIT(white, piecesW[piece_index].position);	
				hashtable[piecesW[piece_index].position] = captured_index;
				piecesB[captured_index].captured = 0;
				eval -= values[piecesB[captured_index].type]; // this is for eval
				piecesW[piece_index].position = destination;
				if (piecesW[piece_index].type == 5) {
					whiteking = destination;
				}
				piecesW[piece_index].Update(occupied);
				capturesW[piece_index] = piecesW[piece_index].moves & black;
				hashtable[destination] = piece_index;
			}
			else {
				//std::cout <<destination <<  "toggle\n";
				TOGGLEBIT(occupied, destination);
				//_Print(occupied);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesB[piece_index].position, occupied, bishop_hash[piecesB[piece_index].position], rook_hash[piecesB[piece_index].position]) | knight[piecesB[piece_index].position]);
				TOGGLEBIT(white, piecesB[piece_index].position);
				TOGGLEBIT(black, destination);
				TOGGLEBIT(black, piecesB[piece_index].position);
				hashtable[piecesB[piece_index].position] = captured_index;
				piecesW[captured_index].captured = 0;
				eval += values[piecesW[captured_index].type];
				if (piecesB[piece_index].type == 5) {
					blackking = destination;
				}
				piecesB[piece_index].position = destination;
				piecesB[piece_index].Update(occupied);
				capturesB[piece_index] = piecesB[piece_index].moves & white;
				hashtable[destination] = piece_index;
				//_Print(occupied);
				//_Print(recalc & black);
			}		
			bool update = 0;
			Bitboard data = recalc & white;
			while (_BitScanForward64(&indexs, data)) {
				piecesW[hashtable[indexs]].Update(occupied);
				if (piecesW[hashtable[indexs]].type == 5)update = 1;
				capturesW[hashtable[indexs]] = piecesW[hashtable[indexs]].moves & black;
				quietW[hashtable[indexs]] = (white & piecesW[hashtable[indexs]].moves) ^ (piecesW[hashtable[indexs]].moves ^ capturesW[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			data = recalc & black;
			while (_BitScanForward64(&indexs, data)) {
				piecesB[hashtable[indexs]].Update(occupied);
				if (piecesB[hashtable[indexs]].type == 5)update = 1;
				capturesB[hashtable[indexs]] = piecesB[hashtable[indexs]].moves & white;
				quietB[hashtable[indexs]] = (black & piecesB[hashtable[indexs]].moves) ^ (piecesB[hashtable[indexs]].moves ^ capturesB[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			if (update)HandleChecks();
		}
		void do_move_quiet(int piece_index, int destination) {
			Bitboard recalc;
			if (tag) {
				//std::cout << " white\n";
				CLEARBIT(occupied, piecesW[piece_index].position);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesW[piece_index].position, occupied, bishop_hash[piecesW[piece_index].position], rook_hash[piecesW[piece_index].position]) | knight[piecesW[piece_index].position]);
				TOGGLEBIT(occupied, destination);
				TOGGLEBIT(white, destination);
				TOGGLEBIT(white, piecesW[piece_index].position);
				if (piecesW[piece_index].type == 5) {
					whiteking = destination;
				}
				piecesW[piece_index].position = destination;
				piecesW[piece_index].Update(occupied);
				capturesW[piece_index] = piecesW[piece_index].moves & black;
				hashtable[destination] = piece_index;
			}
			else {
				//std::cout << " black\n";
				CLEARBIT(occupied, piecesB[piece_index].position);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesB[piece_index].position, occupied, bishop_hash[piecesB[piece_index].position], rook_hash[piecesB[piece_index].position]) | knight[piecesB[piece_index].position]);
				TOGGLEBIT(occupied, destination);
				TOGGLEBIT(black, destination);
				TOGGLEBIT(black, piecesB[piece_index].position);
				if (piecesB[piece_index].type == 5) {
					blackking = destination;
				}
				piecesB[piece_index].position = destination;
				piecesB[piece_index].Update(occupied);
				capturesB[piece_index] = piecesB[piece_index].moves & white;
				hashtable[destination] = piece_index;
			}

			// after changing this it is needed to recalculate some moves		
			bool update = 0;
			Bitboard data = recalc & white;
			while (_BitScanForward64(&indexs, data)) {
				piecesW[hashtable[indexs]].Update(occupied);
				if (piecesW[hashtable[indexs]].type == 5)update = 1;
				capturesW[hashtable[indexs]] = piecesW[hashtable[indexs]].moves & black;
				quietW[hashtable[indexs]] = (white & piecesW[hashtable[indexs]].moves) ^ (piecesW[hashtable[indexs]].moves ^ capturesW[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			data = recalc & black;
			while (_BitScanForward64(&indexs, data)) {
				piecesB[hashtable[indexs]].Update(occupied);
				if (piecesB[hashtable[indexs]].type == 5)update = 1;
				capturesB[hashtable[indexs]] = piecesB[hashtable[indexs]].moves & white;
				quietB[hashtable[indexs]] = (black & piecesB[hashtable[indexs]].moves) ^ (piecesB[hashtable[indexs]].moves ^ capturesB[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			if(update)HandleChecks();
		}
		void undo_move_quiet(int piece_index, int destination) {
			Bitboard recalc;
			if (tag) {
				CLEARBIT(occupied, piecesW[piece_index].position);
				recalc = ((HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesW[piece_index].position, occupied, bishop_hash[piecesW[piece_index].position], rook_hash[piecesW[piece_index].position]) | knight[piecesW[piece_index].position]));
				TOGGLEBIT(occupied, destination);
				TOGGLEBIT(white, destination);
				TOGGLEBIT(white, piecesW[piece_index].position);
				if (piecesW[piece_index].type == 5) {
					whiteking = destination;
				}
				piecesW[piece_index].position = destination;
				piecesW[piece_index].Update(occupied);
				capturesW[piece_index] = piecesW[piece_index].moves & black;
			}
			else {			
				//std::cout << piecesB[piece_index].position << " fef ef" << destination << "\n";
				//_Print(HashQueen(piecesB[piece_index].position, occupied, bishop_hash[destination], rook_hash[destination]));
				unsigned long index;
				//_Print(Moves::Gen_Queen(piecesB[piece_index].position, 0, occupied, 0, index, 0));
				CLEARBIT(occupied, piecesB[piece_index].position);
				recalc = (HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]) | knight[destination] | HashQueen(piecesB[piece_index].position, occupied, bishop_hash[piecesB[piece_index].position], rook_hash[piecesB[piece_index].position]) | knight[piecesB[piece_index].position]);
				TOGGLEBIT(occupied, destination);
				TOGGLEBIT(black, destination);
				TOGGLEBIT(black, piecesB[piece_index].position);
				if (piecesB[piece_index].type == 5) {
					blackking = destination;
				}
				piecesB[piece_index].position = destination;
				piecesB[piece_index].Update(occupied);
				capturesB[piece_index] = piecesB[piece_index].moves & black;
			}

			hashtable[destination] = piece_index;
			// after changing this it is needed to recalculate some moves	
			//std::cout << "recalculating\n";
			//_Print(HashQueen(destination, occupied, bishop_hash[destination], rook_hash[destination]));
			//_Print(HashQueen(piecesB[piece_index].position, occupied, bishop_hash[destination], rook_hash[destination]));
			//_Print(black);
			//_Print(occupied);
			bool update = 0;
			Bitboard data = recalc & white;
			while (_BitScanForward64(&indexs, data)) {
				piecesW[hashtable[indexs]].Update(occupied);
				if (piecesW[hashtable[indexs]].type == 5)update = 1;
				capturesW[hashtable[indexs]] = piecesW[hashtable[indexs]].moves & black;
				quietW[hashtable[indexs]] = (white & piecesW[hashtable[indexs]].moves) ^ (piecesW[hashtable[indexs]].moves ^ capturesW[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			data = recalc & black;
			while (_BitScanForward64(&indexs, data)) {
				piecesB[hashtable[indexs]].Update(occupied);
				if (piecesB[hashtable[indexs]].type == 5)update = 1;
				capturesB[hashtable[indexs]] = piecesB[hashtable[indexs]].moves & white;
				quietB[hashtable[indexs]] = (black & piecesB[hashtable[indexs]].moves) ^ (piecesB[hashtable[indexs]].moves ^ capturesB[hashtable[indexs]]);
				TOGGLEBIT(data, indexs);
			}
			if (update)HandleChecks();
			// after changing this it is needed to recalculate some moves
		}
		void HandleChecks() {	
			white_in_check = 0;
			black_in_check = 0;
			for (int i = 0; i < sizeB; i++) {		
				if (capturesB[i] & ((Bitboard)1 << whiteking)) {
					if (piecesB[i].captured)continue;
					white_in_check = 1;
						break;
				}
			}
			for (int i = 0; i < sizeW; i++) {
				if (capturesW[i] & ((Bitboard)1 <<blackking)) {
					if (piecesW[i].captured)continue;
					black_in_check = 1;
					break;
				}
			}
			
			
			
		}
		void _INIT_captures() {
			capturesW = (Bitboard*)malloc(sizeof(Bitboard) * sizeW);
			for (int i = 0; i < sizeW; i++) {
				capturesW[i] = (piecesW[i].moves & black);
			}
			capturesB = (Bitboard*)malloc(sizeof(Bitboard) * sizeB);
			for (int i = 0; i < sizeB; i++) {
				capturesB[i] = (piecesB[i].moves & white);
			}
		}
		void _INIT_quiet() {
			quietW = (Bitboard*)malloc(sizeof(Bitboard) * sizeW);
			for (int i = 0; i < sizeW; i++) {
				quietW[i] = (piecesW[i].moves&white)^(piecesW[i].moves ^ capturesW[i]);
			}
			quietB = (Bitboard*)malloc(sizeof(Bitboard) * sizeB);
			for (int i = 0; i < sizeB; i++) {
				quietB[i] = (piecesB[i].moves & black)^(piecesB[i].moves ^ capturesB[i]);
			}
		}
		void _EvalFunc() {
			for (int i = 0; i < sizeW; i++) {
				eval += values[piecesW[i].type];
			}
			for (int i = 0; i < sizeB; i++) {
				eval -= values[piecesB[i].type];
			}
		}
	};
	


}

namespace DEBUGDATA {
	void _Print(uint64_t variable) {
		for (int i = 0; i < 64; i++) {
			std::cout << ((variable >> (i)) & 1);
			if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55)std::
				cout << std::endl;
		}
		std::cout << "\n\n";
	}
	void GetInfo(BoardData::Board b) {
		
		std::cout << "occupied ->  " << "\n\n";
		_Print(b.occupied);
		std::cout << "occupied  W->  " << "\n\n";
		_Print(b.white);
		std::cout << "occupied  B->  " << "\n\n";
		_Print(b.black);
		for (int i = 0; i < b.sizeW; i++) {
			if (b.piecesW[i].captured)continue;
			if (b.capturesW[i] == 0)continue;
			_Print(b.capturesW[i]);
			std::cout << "this was for" << b.piecesW[i].position << " " << b.piecesW[i].type << "\n";
		}
		std::cout << "occupied  B->  " << "\n\n";
		_Print(b.black);
		for (int i = 0; i < b.sizeB; i++) {
			if (b.piecesB[i].captured)continue;
			if (b.capturesB[i] == 0)continue;
			_Print(b.capturesB[i]);
		}
		/*
		std::cout << "piececount ->  " << "\n\n";
		std::cout << b.index << "\n";
		std::cout << "pieces (Black) ->  " << "\n";
		for (int i = 0; i < b.sizeB; i++) {			
			if (b.piecesB[i].captured)continue;
			std::cout << "position ->  " <<b.piecesB[i].position << "type -> "<< b.piecesB[i].type << "\n";
			
		}
		std::cout << "pieces (WHITE) ->  " << "\n";
		for (int i = 0; i < b.sizeW; i++) {
			if (b.piecesW[i].captured)continue;
			std::cout << "position ->  " << b.piecesW[i].position << "type -> " << b.piecesW[i].type << "\n";
		}
		
		std::cout << "moves (WHITE) ->  " << "\n";
		for (int i = 0; i < b.sizeW; i++) {
			if (b.piecesW[i].captured)continue;
			_Print(b.quietW[i]);
		}
		/*
		std::cout << "moves (BLACK) ->  " << "\n";
		for (int i = 0; i < b.sizeB; i++) {
			if (b.piecesB[i].captured)continue;
			_Print(b.piecesB[i].moves);
		}
		std::cout << "captures (BLACK) ->  " << "\n";
		for (int i = 0; i < b.sizeB; i++) {
			if (b.piecesB[i].captured)continue;
			_Print(b.capturesB[i]);
		}
		std::cout << "captures (WHITE) ->  " << "\n";
		for (int i = 0; i < b.sizeW; i++) {
			if (b.piecesW[i].captured)continue;
			_Print(b.capturesW[i]);
		}
		std::cout << "static evaluation  ->  " << b.eval << "\n";
		std::cout << "Checks -> "   << "\n";
		std::cout << "White -> " << b.white_in_check <<  "\n";
		std::cout << "Black -> "<< b.black_in_check << "\n";
		*/
		
	}
}

