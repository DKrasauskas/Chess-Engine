
Bitboard HashQueen( int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {
	return bishopMoves[position][(((occupied & diagonals) * bishopMagics[position]) >> (64 - rellevant_bits[0][position]))] | rookMoves[position][((occupied & rows) * rookMagics[position] >> (64 - rellevant_bits[1][position]))];
}
Bitboard HashRook(int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {
	return rookMoves[position][((occupied & rows) * rookMagics[position]) >> (64 - rellevant_bits[1][position])];
}

Bitboard HashBishop(int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {
	return bishopMoves[position][((occupied & diagonals) * bishopMagics[position]) >> (64 - rellevant_bits[0][position])];
}

Bitboard HashPawnW(int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {	
	//std::cout << ((((occupied & pawnW_forward[position]) * pawnWMagics[position]) >> (64 - rellevant_bits[2][position])))  << std::endl;
	return (pawnW_takes[position] & occupied) | pawnWMoves[position][((occupied & pawnW_forward[position]) * pawnWMagics[position]) >> (64 - rellevant_bits[2][position])];
}

Bitboard HashPawnB(int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {
	return (pawnB_takes[position] & occupied) | pawnBMoves[position][((occupied & pawnB_forward[position]) * pawnBMagics[position]) >> (64 - rellevant_bits[3][position])];
}
Bitboard HashKnight(int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {
	return knight[position];
}
Bitboard HashKing(int position, Bitboard occupied, Bitboard diagonals, Bitboard rows) {
	return king[position];
}
