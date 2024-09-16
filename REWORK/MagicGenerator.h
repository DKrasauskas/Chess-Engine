//#include "PermutationGenerator.h"
//#include "Random.h"
#include "PermutationGenerator.h"
#include <fstream>
#include "Magics.h"
#include "Lookup.h"
#include "Hashing.h"
#include "FEN.h"

using namespace std;
namespace MagicGeneration {
    void TestMagics(Bitboard moves, int position, int permutation_count, Bitboard* permutations, std::ofstream& os, std::ofstream& lookup, int type) {
        uint64_t(*func_ptr)(uint8_t, Bitboard, Bitboard, Bitboard, unsigned long&, Bitboard);
        int size;
        func_ptr = Gen_Bishop;
        switch (type) {
        case 0:
            size = 512;
            func_ptr = Gen_Bishop;
            break;
        case 1:
            size = 4096;
            func_ptr = Gen_Rook;
            break;
        case 2:
            size = 4;
            func_ptr = Gen_Forward_LEGAL_PAWNW;
            break;
        case 3:
            size = 4;
            func_ptr = Gen_Forward_LEGAL_PAWNB;
            break;
        }
        int index_count;
        lookup << "{ ";
        unsigned long reference = 0;
        Bitboard* blockers = (Bitboard*)malloc(sizeof(Bitboard) * size);
        Bitboard* visitedS = (Bitboard*)malloc(sizeof(Bitboard) * size);
        for (int i = 0; i < size; i++) {
            visitedS[i] = 0;
        }
        for (int i = 0; i < permutation_count; i++) {
            blockers[i] = func_ptr(position, (uint64_t)0, permutations[i] & moves, (uint64_t)0, reference, 0);         
        }
        for (int i = 0; i < 0xFFFFFF; i++) {
            uint64_t magic = random_fewbits();
            bool failed = false;
            if (type < 2) {
                if (count_bits((moves * magic) & 0xFF00000000000000ULL) < 6) continue;
            }
            else {
                //if (count_bits((moves * magic) & 0xFF00000000000000ULL) < rellevant_bits[type][position]) continue;
            }          
            Bitboard* visited = (Bitboard*)malloc(sizeof(Bitboard) * size);
            Bitboard* counted = (Bitboard*)malloc(sizeof(Bitboard) * size);
            memcpy(visited, visitedS, sizeof(Bitboard) * size);
            memcpy(counted, visitedS, sizeof(Bitboard) * size);
            for (int j = 0; j < permutation_count; j++) {
                int index = ((moves & permutations[j]) * magic) >> (64 - rellevant_bits[type][position]);
                if (counted[index] == 1) {
                    if (visited[index] != blockers[j]) {
                        failed = true;
                        break;
                    }
                }
                else {
                    counted[index] = 1;
                    visited[index] = blockers[j];
                }
            }
            if (!failed) {
                os << "0x" << std::hex << magic << ",\n ";
                for (int i = 0; i < size; i++) {
                    if (i == size - 1) {
                        lookup << "0x" << std::hex << (uint64_t)visited[i] << " ";
                        break;
                    }
                    lookup << "0x" << std::hex << (uint64_t)visited[i] << ", ";

                }
                free(counted);
                free(visited);
                break;

            }
            else {
                failed = false;
                free(counted);
                free(visited);
            }

        }
        free(blockers);
        free(visitedS);
        lookup << "}, ";

    }
    
    bool ConfirmNoHashCollisions(int  type) {
        unsigned long reference = 0;
        bool failed = 0;
        switch (type) {
        case 0:
            for (int i = 0; i < 64; i++) {
                Blockers::pos = 0;
                Blockers::GeneratePermutations(type, i);
                for (int j = 0; j < Blockers::pos; j++) {
                    if (bishopMoves[i][((Blockers::variations[j] & bishop[i]) * bishopMagics[i]) >> (64 - rellevant_bits[type][i])] == Gen_Bishop(i, 0, Blockers::variations[j], 0, reference, 0)) {
                    }
                    else {
                        failed = 1;
                    }
                }
                Blockers::pos = 0;
            }
            if (failed) {
                cout << "FailedGeneratingMagics\n";
            }
            else {
                cout << "Success\n";
            }
            break;
        case 1:

            for (int i = 0; i < 64; i++) {
                Blockers::pos = 0;
                Blockers::GeneratePermutations(type, i);
                for (int j = 0; j < Blockers::pos; j++) {                  
                    if (rookMoves[i][((Blockers::variations[j] & rook[i]) * rookMagics[i]) >> (64 - rellevant_bits[type][i])] == Gen_Rook(i, 0, Blockers::variations[j], 0, reference, 0)) {
                    }
                    else {
                        failed = 1;
                    }
                }
                Blockers::pos = 0;
            }
            if (failed) {
                cout << "FailedGeneratingMagics\n";
            }
            else {
                cout << "Success\n";
            }
            break;
        case 2:
            for (int i = 8; i < 56; i++) {
                Blockers::pos = 0;
                Blockers::GeneratePermutations(type, i);
                for (int j = 0; j < Blockers::pos; j++) {
                    if (pawnWMoves[i][((Blockers::variations[j] & pawnW_forward[i]) * pawnWMagics[i]) >> (64 - rellevant_bits[type][i])] == Gen_Forward_LEGAL_PAWNW(i, 0, Blockers::variations[j], 0, reference, 0)) {
                    }
                    else {             
                        failed = 1;
                        break;
                    }
                }
                Blockers::pos = 0;
            }
            if (failed) {
                cout << "FailedGeneratingMagics\n";
            }
            else {
                cout << "Success\n";
            }
            break;
        case 3:    
            for (int i = 8; i < 56; i++) {
                Blockers::pos = 0;
                Blockers::GeneratePermutations(type, i);
                for (int j = 0; j < Blockers::pos; j++) {
                    if (pawnBMoves[i][((Blockers::variations[j] & pawnB_forward[i]) * pawnBMagics[i]) >> (64 - rellevant_bits[type][i])] == Gen_Forward_LEGAL_PAWNB(i, 0, Blockers::variations[j], 0, reference, 0)) {
                    }
                    else {
                        failed = 1;
                    }
                }
                Blockers::pos = 0;
            }
            if (failed) {
                cout << "FailedGeneratingMagics\n";
            }
            else {
                cout << "Success\n";
            }
        }
        return failed;
    }
    
    void ConfirmNoCollisions() {
        ConfirmNoHashCollisions(0);
        ConfirmNoHashCollisions(1);
        ConfirmNoHashCollisions(2);
        ConfirmNoHashCollisions(3);
    }

    void FindPermutation(int num, Bitboard nums) {
        Blockers::pos = 0;
        Blockers::GeneratePermutations(1, num);
        
        DEBUG::Print(nums);
        DEBUG::Print(Blockers::variations[3]);
        for (int i = 0; i < Blockers::pos; i++) {
            if (Blockers::variations[i] == nums) {
            std:cout << "found" << std::endl;
            }
           

        }
    }
    
    void CreateLookup() {
        ofstream out("Lookup.h");
        ofstream outf("Magics.h");
        //ConfirmNoCollisions();    
        out << "Bitboard rookMoves[64][4096] = {";
        outf << "Bitboard rookMagics[64] = {";
        int position = 0;
        for (int i = 0; i < 64; i++) {
            Blockers::GeneratePermutations(1, i);
            TestMagics(rook[i], i, Blockers::pos, Blockers::variations, outf, out, 1);
            cout << "generated Rook at" << i << endl;
            Blockers::pos = 0;
        }
        out << "};\n\n";
        outf << "};\n\n";
        out << "Bitboard bishopMoves[64][512] = {";
        outf << "Bitboard bishopMagics[64] = {";
        for (int i = 0; i < 64; i++) {
            Blockers::GeneratePermutations(0, i);
            TestMagics(bishop[i], i, Blockers::pos, Blockers::variations, outf, out, 0);
            cout << "generated Bishop at" << i << endl;
            Blockers::pos = 0;
        }
        out << "};\n\n";
        outf << "};\n\n";
        out << "Bitboard pawnWMoves[64][4] = {";
        outf << "Bitboard pawnWMagics[64] = {";
        for (int i = 0; i < 8; i++) {         
            out << "{ ";
            for (int j = 0; j < 4; j++) {
                out  << "0x" << std::hex << "0" << ", ";
           }
            out << "}, ";
            outf<< "0x" << std::hex << "0" << ",\n ";
           
        }
        for (int i = 8; i < 56; i++) {
            Blockers::GeneratePermutations(2, i);
            TestMagics(pawnW_forward[i], i, Blockers::pos, Blockers::variations, outf, out, 2);
            cout << "generated PawnW at" << i << endl;
            Blockers::pos = 0;
        }
        for (int i = 0; i < 8; i++) {
            out << "{ ";
            for (int j = 0; j < 4; j++) {
                out << "0x" << std::hex << "0" << ", ";
            }
            out << "}, ";
            outf << "0x" << std::hex << "0" << ",\n ";          
        }
        out << "};\n\n";
        outf << "};\n\n";
        out << "Bitboard pawnBMoves[64][4] = {";
        outf << "Bitboard pawnBMagics[64] = {";
        for (int i = 0; i < 8; i++) {
            out << "{ ";
            for (int j = 0; j < 4; j++) {
                out << "0x" << std::hex << "0" << ", ";
            }
            out << "}, ";
            outf << "0x" << std::hex << "0" << ",\n ";          
        }
        for (int i = 8; i < 56; i++) {
            Blockers::GeneratePermutations(3, i);
            TestMagics(pawnB_forward[i], i, Blockers::pos, Blockers::variations, outf, out, 3);
            cout << "generated PawnB at" << i << endl;
            Blockers::pos = 0;
        }
        for (int i = 0; i < 8; i++) {
            out << "{ ";
            for (int j = 0; j < 4; j++) {
                out << "0x" << std::hex << "0" << ", ";
            }
            out << "}, ";
            outf << "0x" << std::hex << "0" << ",\n ";       
        }
        out << "};\n\n";
        outf << "};\n\n";
        // ConfirmNoCollisions();
    }
}
