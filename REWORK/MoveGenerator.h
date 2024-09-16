#pragma once
#define Bitboard uint64_t

namespace Moves {
    Bitboard Gen_Rook(uint8_t position, Bitboard occupied_white, Bitboard occupied, Bitboard moves, unsigned long& index, Bitboard attacked_by_black) {

        if (_BitScanReverse64(&index, occupied & rookU[position])) {
            moves |= rookU[position] ^ rookU[index];
        }
        else    moves |= rookU[position];
        if (_BitScanForward64(&index, occupied & rookD[position])) {
            moves |= rookD[position] ^ rookD[index];
        }
        else  moves |= rookD[position];
        if (_BitScanForward64(&index, occupied & rookR[position])) {
            moves |= rookR[position] ^ rookR[index];
        }
        else moves |= rookR[position];
        if (_BitScanReverse64(&index, occupied & rookL[position])) {
            moves |= rookL[position] ^ rookL[index];
        }
        else  moves |= rookL[position];
        attacked_by_black |= (moves ^ moves & occupied_white);
        return  moves ^ moves & occupied_white;
    }
    Bitboard Gen_Bishop(uint8_t position, Bitboard occupied_white, Bitboard occupied, Bitboard moves, unsigned long& index, Bitboard attacked_by_black) {

        if (_BitScanReverse64(&index, occupied &bishopUL[position])) {
            moves |= bishopUL[position] ^ bishopUL[index];
        }
        else    moves |= bishopUL[position];
        if (_BitScanReverse64(&index, occupied & bishopUR[position])) {
            moves |= bishopUR[position] ^ bishopUR[index];
        }
        else    moves |= bishopUR[position];
        if (_BitScanForward64(&index, occupied & bishopDR[position])) {
            moves |= bishopDR[position] ^ bishopDR[index];
        }
        else  moves |= bishopDR[position];
        if (_BitScanForward64(&index, occupied & bishopDL[position])) {
            moves |= bishopDL[position] ^ bishopDL[index];
        }
        else  moves |= bishopDL[position];
        attacked_by_black |= moves ^ moves & occupied_white;
        return  moves ^ moves & occupied_white;
    }
    Bitboard Gen_Queen(uint8_t position, Bitboard occupied_white, Bitboard occupied, Bitboard moves, unsigned long& index, Bitboard attacked_by_black) {

        if (_BitScanReverse64(&index, occupied & bishopUL[position])) {
            moves |= bishopUL[position] ^ bishopUL[index];
        }
        else    moves |= bishopUL[position];
        if (_BitScanReverse64(&index, occupied & bishopUR[position])) {
            moves |= bishopUR[position] ^ bishopUR[index];
        }
        else    moves |= bishopUR[position];
        if (_BitScanForward64(&index, occupied & bishopDR[position])) {
            moves |= bishopDR[position] ^ bishopDR[index];
        }
        else  moves |= bishopDR[position];
        if (_BitScanForward64(&index, occupied & bishopDL[position])) {
            moves |= bishopDL[position] ^ bishopDL[index];
        }
        else  moves |= bishopDL[position];
        if (_BitScanReverse64(&index, occupied & rookU[position])) {
            moves |= rookU[position] ^ rookU[index];
        }
        else    moves |= rookU[position];
        if (_BitScanForward64(&index, occupied & rookD[position])) {
            moves |= rookD[position] ^ rookD[index];
        }
        else  moves |= rookD[position];
        if (_BitScanForward64(&index, occupied & rookR[position])) {
            moves |= rookR[position] ^ rookR[index];
        }
        else moves |= rookR[position];
        if (_BitScanReverse64(&index, occupied & rookL[position])) {
            moves |= rookL[position] ^ rookL[index];
        }
        else  moves |= rookL[position];
        return  moves ^ (moves & occupied_white);
    }
    Bitboard Gen_Knight(uint8_t position, Bitboard occupied_black, Bitboard occupied, Bitboard moves) {

        return moves | (knight[position] ^ (occupied_black & knight[position]));
    }
    Bitboard Gen_Rook_notrail(uint8_t position, Bitboard occupied_white, Bitboard occupied, Bitboard moves, unsigned long& index, Bitboard attacked_by_black) {
        if (_BitScanReverse64(&index, occupied & rookU[position])) {
            moves |= rookU[position] ^ rookU[index];
        }
        else    moves |= rookU[position];
        if (_BitScanForward64(&index, occupied & rookD[position])) {
            moves |= rookD[position] ^ rookD[index];
        }
        else  moves |= rookD[position];
        if (_BitScanForward64(&index, occupied & rookR[position])) {
            moves |= rookR[position] ^ rookR[index];
        }
        else moves |= rookR[position];
        if (_BitScanReverse64(&index, occupied & rookL[position])) {
            moves |= rookL[position] ^ rookL[index];
        }
        else  moves |= rookL[position];
        attacked_by_black |= (moves ^ moves & occupied_white);
        moves = moves ^ moves & occupied_white;
        moves &= ~((Bitboard)1 << (position - (position / 8) * 8));
        moves &= ~((Bitboard)1 << (position + ((63 - position) / 8) * 8));

        moves &= ~((Bitboard)1 << (position / 8) * 8);
        moves &= ~((Bitboard)1 << (position / 8) * 8 + 7);
        return moves;
    }
    Bitboard Gen_Bishop_notrail(uint8_t position, Bitboard occupied_white, Bitboard occupied, Bitboard moves, unsigned long& index, Bitboard attacked_by_black) {

        if (_BitScanReverse64(&index, occupied & bishopUL[position])) {
            moves |= bishopUL[position] ^ bishopUL[index];
        }
        else    moves |= bishopUL[position];
        if (_BitScanReverse64(&index, occupied & bishopUR[position])) {
            moves |= bishopUR[position] ^ bishopUR[index];
        }
        else    moves |= bishopUR[position];
        if (_BitScanForward64(&index, occupied & bishopDR[position])) {
            moves |= bishopDR[position] ^ bishopDR[index];
        }
        else  moves |= bishopDR[position];
        if (_BitScanForward64(&index, occupied & bishopDL[position])) {
            moves |= bishopDL[position] ^ bishopDL[index];
        }
        else  moves |= bishopDL[position];
        attacked_by_black |= moves ^ moves & occupied_white;
        moves = moves ^ moves & occupied_white;
       
        return (moves&0xFF818181818181FF)^moves; // there was a bug here so I just did some black magic

    }
    Bitboard Gen_Forward_LEGAL_PAWNW(uint8_t position, Bitboard unused, Bitboard occupied, Bitboard moves, unsigned long& i, Bitboard attacked_by_black) {
        if ((occupied& pawnW_forward[position]) == 0) return pawnW_forward[position];
        unsigned long index;
        unsigned long indexofmove;
        _BitScanReverse64(&index, occupied & pawnW_forward[position]);
        _BitScanForward64(&indexofmove, pawnW_forward[position]);
    
        if (index == indexofmove) {
            return (pawnW_forward[position] & (~((uint64_t)1 << indexofmove)));
        }
        else {
            return (uint64_t)0;
        }

    }
    Bitboard Gen_Forward_LEGAL_PAWNB(uint8_t position,  Bitboard unused, Bitboard occupied, Bitboard moves, unsigned long& i, Bitboard attacked_by_black) {
        if ((occupied & pawnB_forward[position]) == 0) return pawnB_forward[position];
        unsigned long index;
        unsigned long indexofmove;
        _BitScanForward64(&index, occupied & pawnB_forward[position]);
        _BitScanReverse64(&indexofmove, pawnB_forward[position]);
        if (index == indexofmove) {
            return (pawnB_forward[position] & (~((uint64_t)1 << indexofmove)));
        }
        else {
            return (uint64_t)0;         
        }
    }
}