// REWORK.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define DEFAULT_BUFLEN 512
#include "MagicGenerator.h"
#include <fstream>
//#include "FEN.h"
//#include "IBoard.h"
#include "ZoobristHashing.h"
#include <thread>
#include "Search.h"
//#include "Enumeration.h"

int* array1 = (int*)malloc(sizeof(int) * 1024);
int* array2 = (int*)malloc(sizeof(int) * 1024);
int index1 = 0;
int index2 = 0;


void Search_To_Next(int depth, int num, int* array, int n) {
	if (depth == 10) {
		array[n] = num;
		n++;
		return;
	}
	else {
		for (int i = 0; i < 80000; i++) {

		}
		num = rand()%1000000;
		//Search(depth + 1, num, array, n);
		num = rand() % 10000000;
		//Search(depth + 1, num, array, n);
		return;
	}
}



///TODO
// Implement quiet moves                                  //done 
// Implement additional quiet move array for both colors. //done
// Implement Captures                                     //done?
// Implement EnPessant
// implement evaluation and its updation                  //done
//Implement castling
//Implement Checks                                        //done


/*
int main() {
	FEN::Board board;
	Pieces::Piece piece;
	BoardData::Board finalpos;
	FEN::BoardInfo info = board.GetBoardData("rnbqkbnr/ppp2ppp/8/1N1pp3/8/8/PPPPPPPP/R1BQKBNR w KQkq - 0 1");
	FEN::CreatePosition(info, DFS::board);
	int capturedid;
	DFS::board.tag = 1;
	DFS::Stack stekas;
	stekas._INIT_();
	stekas.OPERATE();
	thread a = thread(FC, 1);
}
*/


int main()
{
	WSADATA wsa_data;
	SOCKADDR_IN addr;
	WSAStartup(MAKEWORD(2, 0), &wsa_data);
	const auto server = socket(AF_INET, SOCK_STREAM, 0);

	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

	addr.sin_family = AF_INET;
	ifstream in("Config.txt");
	bool color = 1;
	in >> color;
	if (color) {
		cout << "playing as white\n";
		addr.sin_port = htons(6969); //  6970 if black

		connect(server, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
		cout << "Connected to server!" << endl;	
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		int result;

		do {
			// Receive message
			result = recv(server, recvbuf, recvbuflen, 0); // Jûsø programa lauks èia, kol serveris atsiøs naujà lentos informacijà.
			std::string message(recvbuf);
			message = message.substr(0, result);
			cout << "Message received:" << message << endl;
			DFS::alfa = (int)0x80000000;
			DFS::beta = 0x7FFFFFFF;
			FEN::Board board;
			Pieces::Piece piece;
			BoardData::Board finalpos;
			FEN::BoardInfo info = board.GetBoardData(message.c_str());
			FEN::CreatePosition(info, DFS::board);
			int capturedid;
			//DEBUG::Print(HashQueen(11, DFS::board.occupied, bishop_hash[11], rook_hash[11]));
			DFS::board.tag = 1;
			//DEBUGDATA::GetInfo(DFS::board);
			int depth = 4;// -0.1 * (DFS::board.sizeB + DFS::board.sizeW) + 8;
			std::cout << "Current depth ->" << depth << "\n";
			DFS::INIT_Search(1, depth);
			//DEBUGDATA::GetInfo(DFS::board);
			enums enumas;
			string s = "";
			s += enumas.names[DFS::bes_begin][0];
			s += enumas.names[DFS::bes_begin][1];
			s += enumas.names[DFS::bes_end][0];
			s += enumas.names[DFS::bes_end][1];
			send(server, s.c_str(), s.length(), 0);
			cout << "Message sent!" << endl;
		} while (result > 0);
		return 0;
	}
	else {
		addr.sin_port = htons(6970); // Pakeisti á 6970, jeigu jungiates prie juodøjø

		connect(server, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
		cout << "Connected to server!" << endl;
		/// WINSOCKET JUODOJI MAGIJA, JEI VEIKIA, SIULAU NEKEIST

		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;
		int result;

		do {
			// Receive message
			result = recv(server, recvbuf, recvbuflen, 0); // Jûsø programa lauks èia, kol serveris atsiøs naujà lentos informacijà.
			std::string message(recvbuf);
			message = message.substr(0, result);
			cout << "Message received:" << message << endl;
			DFS::alfa = (int)0x80000000;
			DFS::beta = 0x7FFFFFFF;
			FEN::Board board;
			Pieces::Piece piece;
			BoardData::Board finalpos;
			FEN::BoardInfo info = board.GetBoardData(message.c_str());
			std::cout << "done\n";
			FEN::CreatePosition(info, DFS::board);
			int capturedid;
			//DEBUG::Print(HashQueen(11, DFS::board.occupied, bishop_hash[11], rook_hash[11]));
			DFS::board.tag = 0;
			//DEBUGDATA::GetInfo(DFS::board);
			DFS::INIT_Search(0, 5);
			//DEBUGDATA::GetInfo(DFS::board);
			enums enumas;
			string s = "";
			s += enumas.names[DFS::bes_begin][0];
			s += enumas.names[DFS::bes_begin][1];
			s += enumas.names[DFS::bes_end][0];
			s += enumas.names[DFS::bes_end][1];
			send(server, s.c_str(), s.length(), 0);
			cout << "Message sent!" << endl;
		} while (result > 0);
		return 0;
	}
	
}

/*
int main()
{ // 2K1k3/3Q4/8/8/2Rqr3/8/8/8 w - - 0 1
	// 2K1k3/8/8/8/2RQr3/8/8/8 w - - 0 1
	//MagicGeneration::CreateLookup();
	//MagicGeneration::ConfirmNoCollisions();
	FEN::Board board;
	Pieces::Piece piece;
	BoardData::Board finalpos;
	FEN::BoardInfo info = board.GetBoardData("rnbqkbnr/pppp1ppp/8/8/4p3/N7/PPPPPPPP/1RBQKBNR w Kkq - 0 1");
	FEN::CreatePosition(info, DFS::board);
	int capturedid;
	//DEBUG::Print(HashQueen(11, DFS::board.occupied, bishop_hash[11], rook_hash[11]));
	int b = 0x80000000;
	std::cout << b << "vo ir skaiciuks\n";
	DFS::board.tag = 1;
	int captureindex;
	DFS::board.ID = DFS::initial.hash;
	std::cout << sizeof(TTable::Entry) << "\n";
	enums enumas;
	Zobrist::_Create_();
	DFS::initial = Zobrist::GetHashFull(DFS::board);
    DFS::INIT_Search(1, 5);
	
	//DEBUGDATA::GetInfo(DFS::board);
	//DEBUGDATA::GetInfo(DFS::board);
	//DEBUGDATA::GetInfo(DFS::board);

	std::cout << enumas.names[DFS::bes_begin][0] << enumas.names[DFS::bes_begin][1] << " " << enumas.names[DFS::bes_end][0] << enumas.names[DFS::bes_end][1];
	//DEBUGDATA::GetInfo(DFS::board);
	//DFS::board.do_move_capture(DFS::board.hashtable[35], 36, capturedid);
	//DFS::board.tag = 0;
	//DFS::board.do_move_quiet(DFS::board.hashtable[4], 3);
	//DEBUGDATA::GetInfo(DFS::board);
	//DEBUGDATA::GetInfo(DFS::board);
	//std::cout << "lalaal\n";
	//DFS::INIT_Search(1, 3);  

	//DEBUG::Print(HashPawnW(49, finalpos.occupied, finalpos.occupied, 0));
	//DEBUG::Print(Gen_Forward_LEGAL_PAWNW(49, finalpos.occupied, pawnW_forward[49], 0, index, 0));

	//DEBUG::Print(finalpos.occupied&rook[25]);
	//Blockers::GeneratePermutations(1, 25);
	//Blockers::AllUnique();
	//DEBUG::Print(HashRook(25, finalpos.occupied, bishop_hash[25], rook_hash[25]));
	//MagicGeneration::CreateLookup();
	//MagicGeneration::ConfirmNoCollisions();
	//MagicGeneration::ConfirmNoCollisions();
	//std::cout << "0x" << std::hex << (finalpos.occupied & rook[25]) << "\n";

	//MicGeneration::CreateLookup();
	//MagicGeneration::ConfirmNoCollisions();
	//CreateLookupRook();
	//FEN::Board board;
	//FEN::BoardInfo info = board.GetBoardData("");
	//DEBUG::Print(info.occupied);
	//DEBUG::Print(info.empty);
	//Search(0, 0, array1, index1);
	//thread th[1024];
	//for (int i = 0; i < 1024; i++) {	
		// th[i] = thread(Search, 0, 0, array2, index2);
		//Search(0, 0, array2, index2);
	//}

	//for (int i = 0; i < 1024; i++) {
	//	th[i].join();
		//Search(0, 0, array2, index2);
	//}
	//free(array1);
	//free(array2);
}
*/
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
