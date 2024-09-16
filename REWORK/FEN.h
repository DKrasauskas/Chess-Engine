#include "Pieces.h"
#include "IBoard.h"
namespace FEN {
    struct BoardInfo {
        PartsWhite partsWhite = PartsWhite();       
        PartsBlack partsBlack = PartsBlack();
        uint64_t occupied = 0;
        uint64_t empty = 0;
        int _type[64];
    };
    struct Board {
        uint64_t occupied = 0;
        uint64_t empty = 0;

        //stores White Bitboards
        PartsWhite partsWhite = PartsWhite();
        //Stroes Black Bitboards
        PartsBlack partsBlack = PartsBlack();

        int _type[64] = { 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0 };
        //int movecount[12] = { -1, -1, -1, -1, -1 , -1 , -1, -1, -1, -1, -1, -1 };
        unsigned long index = 0;
        //Fen to Bitboards
        void _FenBinegold(const char* input) {
            char* line = (char*)malloc(sizeof(char) * 8);
            int index = 0;
            int row = 7;
            int count = 0;
            for (int i = 0; i < strlen(input); i++) {
                if (input[i] == ' ')break;
                if (input[i] == '/') {
                    int column = 0;
                    for (int j = 0; j < index; j++) {
                        if (column >= 8) {
                            count--;
                            break;
                        }
                        if (line[j] == '1' || line[j] == '2' || line[j] == '3' || line[j] == '4' || line[j] == '5' || line[j] == '6' || line[j] == '7' || line[j] == '8') {
                            column += line[j] - '0';
                            count += line[j] - '0';
                        }
                        else {
                            if (line[j] == 'p') {
                                partsBlack.pawns |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'P') {
                                partsWhite.pawns |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'n') {
                                partsBlack.knights |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'N') {
                                partsWhite.knights |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'K') {
                                partsWhite.king |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'k') {
                                partsBlack.king |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'Q') {
                                partsWhite.queen |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'q') {
                                partsBlack.queen |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'R') {
                                partsWhite.rook |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'r') {
                                partsBlack.rook |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'B') {
                                partsWhite.bishop |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            if (line[j] == 'b') {
                                partsBlack.bishop |= ((uint64_t)1 << count);
                                occupied |= ((uint64_t)1 << count);
                            }
                            column++;
                            count++;
                        }
                    }
                    free(line);
                    line = (char*)malloc(sizeof(char) * 8);
                    index = 0;
                    row--;
                }
                else {
                    line[index] = input[i];
                    index++;
                }
            }
            row = 0;
            int column = 0;
            for (int j = 0; j < index; j++) {
                if (column >= 8) break;

                if (line[j] == '1' || line[j] == '2' || line[j] == '3' || line[j] == '4' || line[j] == '5' || line[j] == '6' || line[j] == '7' || line[j] == '8') {
                    column += (int)line[j] - '0';
                    count += (int)line[j] - '0';
                }
                else {
                    if (line[j] == 'p') {
                        partsBlack.pawns |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'P') {
                        partsWhite.pawns |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'n') {
                        partsBlack.knights |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'N') {
                        partsWhite.knights |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);

                    }
                    if (line[j] == 'K') {
                        partsWhite.king |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'k') {
                        partsBlack.king |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'Q') {
                        partsWhite.queen |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'q') {
                        partsBlack.queen |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'R') {
                        partsWhite.rook |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'r') {
                        partsBlack.rook |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'B') {
                        partsWhite.bishop |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    if (line[j] == 'b') {
                        partsBlack.bishop |= ((uint64_t)1 << count);
                        occupied |= ((uint64_t)1 << count);
                    }
                    column++;
                    count++;
                }
            }

            //std::cout << count;
            empty = ~occupied;
            partsBlack.Init();
            partsWhite.Init();
            free(line);
        }
        // Visualize bitboard      
        BoardInfo GetBoardData(const char* input) {
            _FenBinegold(input);
            BoardInfo board;
            board.occupied = occupied;
            board.empty = empty;
            board.partsBlack = partsBlack;
            board.partsWhite = partsWhite;
            memcpy(&board._type, &_type, sizeof(_type));
            return board;
        }
    };
    void CreatePosition(BoardInfo board, BoardData::Board& out) {
        out.occupied = board.occupied;
        out.black = board.partsBlack.occupied;
        out.white = board.partsWhite.occupied;
        out.piecesW = (Pieces::Piece*)malloc(sizeof(Pieces::Piece) * 16);
        out.piecesB = (Pieces::Piece*)malloc(sizeof(Pieces::Piece) * 16);
        out.index = 0;
        out.sizeW = 0;
        out.sizeB = 0;
        unsigned long index;
        while (_BitScanForward64(&index, board.partsWhite.bishop)) {
            Pieces::Piece piece;
            piece.move_generation = HashBishop;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 2;
            out.piecesW[out.sizeW] = piece;
            out.hashtable[index] = out.sizeW;
            out.sizeW++;
            TOGGLEBIT(board.partsWhite.bishop, index);
        }
        while (_BitScanForward64(&index, board.partsWhite.rook)) {
            Pieces::Piece piece;
            piece.move_generation = HashRook;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 3;
            out.piecesW[out.sizeW] = piece;
            out.hashtable[index] = out.sizeW;
            out.sizeW++;
            TOGGLEBIT(board.partsWhite.rook, index);
        }
        while (_BitScanForward64(&index, board.partsWhite.queen)) {
            Pieces::Piece piece;
            piece.move_generation = HashQueen;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 4;
            out.piecesW[out.sizeW] = piece;
            out.hashtable[index] = out.sizeW;
            out.sizeW++;
            TOGGLEBIT(board.partsWhite.queen, index);
        }
        while (_BitScanForward64(&index, board.partsWhite.king)) {
            Pieces::Piece piece;
            piece.move_generation = HashKing;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 5;
            out.piecesW[out.sizeW] = piece;
            out.hashtable[index] = out.sizeW;
            out.whiteking = index;
            out.sizeW++;
            TOGGLEBIT(board.partsWhite.king, index);
        }
        while (_BitScanForward64(&index, board.partsWhite.knights)) {
            Pieces::Piece piece;
            piece.move_generation = HashKnight;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 1;
            out.piecesW[out.sizeW] = piece;
            out.hashtable[index] = out.sizeW;
            out.sizeW++;
            TOGGLEBIT(board.partsWhite.knights, index);
        }
        while (_BitScanForward64(&index, board.partsWhite.pawns)) {
            Pieces::Piece piece;
            piece.move_generation = HashPawnW;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 0;
            out.piecesW[out.sizeW] = piece;
            out.hashtable[index] = out.sizeW;
            out.sizeW++;
            TOGGLEBIT(board.partsWhite.pawns, index);
        }
        while (_BitScanForward64(&index, board.partsBlack.bishop)) {
            Pieces::Piece piece;
            piece.move_generation = HashBishop;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 2;
            out.piecesB[out.sizeB] = piece;
            out.hashtable[index] = out.sizeB;
            out.sizeB++;
            TOGGLEBIT(board.partsBlack.bishop, index);
        }
        while (_BitScanForward64(&index, board.partsBlack.rook)) {
            Pieces::Piece piece;
            piece.move_generation = HashRook;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 3;
            out.piecesB[out.sizeB] = piece;
            out.hashtable[index] = out.sizeB;
            out.sizeB++;
            TOGGLEBIT(board.partsBlack.rook, index);
        }
        while (_BitScanForward64(&index, board.partsBlack.queen)) {
            Pieces::Piece piece;
            piece.move_generation = HashQueen;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 4;
            out.piecesB[out.sizeB] = piece;
            out.hashtable[index] = out.sizeB;
            out.sizeB++;
            TOGGLEBIT(board.partsBlack.queen, index);
        }
        while (_BitScanForward64(&index, board.partsBlack.king)) {
            Pieces::Piece piece;
            piece.move_generation = HashKing;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 5;
            out.piecesB[out.sizeB] = piece;
            out.hashtable[index] = out.sizeB;
            out.blackking = index;
            out.sizeB++;
            TOGGLEBIT(board.partsBlack.king, index);
        }
        while (_BitScanForward64(&index, board.partsBlack.knights)) {
            Pieces::Piece piece;
            piece.move_generation = HashKnight;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 1;
            out.piecesB[out.sizeB] = piece;
            out.hashtable[index] = out.sizeB;
            out.sizeB++;
            TOGGLEBIT(board.partsBlack.knights, index);
        }
        while (_BitScanForward64(&index, board.partsBlack.pawns)) {
            Pieces::Piece piece;
            piece.move_generation = HashPawnB;
            piece.position = index;
            piece.Update(out.occupied);
            piece.type = 0;            
            out.piecesB[out.sizeB] = piece;
            out.hashtable[index] = out.sizeB;
            out.sizeB++;
            TOGGLEBIT(board.partsBlack.pawns, index);
        }
        out._INIT_captures();
        out._INIT_quiet();
        out._EvalFunc();
        out.HandleChecks();
    }
   
}
namespace DEBUG {
    void Print(uint64_t variable) {
        for (int i = 0; i < 64; i++) {
            std::cout << ((variable >> (i)) & 1);
            if (i == 7 || i == 15 || i == 23 || i == 31 || i == 39 || i == 47 || i == 55)std::cout << std::endl;
        }
        std::cout << "\n\n";
    }
}