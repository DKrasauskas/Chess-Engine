#pragma once
#include "Enumeration.h"
#include <thread>
namespace DFS {

	enums enumas;
	string path[5] = {"", "", "", "", ""};
	int bes_begin;
	int bes_end;
	BoardData::Board board;
	int best_eval;
	int alfa = 0x80000000;
	int beta = 0x7FFFFFFF;
	TTable::Entry initial;
	TTable::Table table;
	int Search(bool tag, int depth, int current, int alfa, int beta);
	int SearchT(bool tag, int depth, int current);
	void INIT_Search(bool tag, int depth) {
		bool upgrade = 0;
		unsigned long index;
		board.tag = tag;
		if (tag) {
			best_eval = 0x80000000;
			bool has_move = 0;
			for (int i = 0; i < board.sizeW; i++) {
				if (board.piecesW[i].captured)continue;
				Bitboard captures = board.capturesW[i];
				while (_BitScanForward64(&index, captures)) {
					int captured_index;
					int begin = board.hashtable[board.piecesW[i].position];
					int start = board.piecesW[i].position;
					board.do_move_capture(begin, index, captured_index);					
					if (board.white_in_check) {
						board.undo_move_capture(board.hashtable[index], start, captured_index);
						TOGGLEBIT(captures, index);
						continue;
					}
					int val = Search(!tag, depth, 1, (int)0x80000000, (int)0x7FFFFFFF);
					if (val > best_eval) {
						path[0] = "";
						path[0] += enumas.names[start][0];
						path[0] += enumas.names[start][1];
						path[0] += enumas.names[index][0];
						path[0] += enumas.names[index][1];
						path[0] += " ";
						best_eval = val;
						bes_begin = start;
						bes_end = index;
					}
					if (alfa < best_eval) {
						alfa = best_eval;
					}
					if (alfa >= beta) {
						board.tag = tag;
						board.undo_move_capture(board.hashtable[index], start, captured_index);
						break;
					}

					board.tag = tag;
					board.undo_move_capture(board.hashtable[index], start, captured_index);
					TOGGLEBIT(captures, index);
					}				
			}
			for (int i = 0; i < board.sizeW; i++) {
				if (board.piecesW[i].captured)continue;
				Bitboard captures = board.quietW[i];
				while (_BitScanForward64(&index, captures)) {
					int begin = board.hashtable[board.piecesW[i].position];
					int start = board.piecesW[i].position;
					board.do_move_quiet(begin, index);

					if (board.white_in_check) {
						board.undo_move_quiet(board.hashtable[index], start);
						TOGGLEBIT(captures, index);
						continue;
					}


					int val = Search(!tag, depth, 1, (int)0x80000000, (int)0x7FFFFFFF);
					if (val > best_eval) {
						path[0] = "";
						path[0] += enumas.names[start][0];
						path[0] += enumas.names[start][1];
						path[0] += enumas.names[index][0];
						path[0] += enumas.names[index][1];
						path[0] += " ";
						best_eval = val;
						bes_begin = start;
						bes_end = index;
					}
					if (alfa < best_eval) {
						alfa = best_eval;
					}
					if (alfa >= beta) {
						board.tag = tag;
						board.undo_move_quiet(board.hashtable[index], start);
						break;
					}
					board.tag = tag;
					board.undo_move_quiet(board.hashtable[index], start);
					TOGGLEBIT(captures, index);
				}
			}
		}
		else {
			cout << "here";
			best_eval = 0x7FFFFFFF;
			for (int i = 0; i < board.sizeB; i++) {
				if (board.piecesB[i].captured)continue;
				Bitboard captures = board.capturesB[i];
				//DEBUG::Print(captures);
				while (_BitScanForward64(&index, captures)) {
					int captured_index;
					int begin = board.hashtable[board.piecesB[i].position];
					int start = board.piecesB[i].position;
					board.do_move_capture(begin, index, captured_index);
					//std::cout << "after capturees _>";
					//DEBUGDATA::GetInfo(board);
					if (board.black_in_check) {
						board.undo_move_capture(board.hashtable[index], start, captured_index);
						TOGGLEBIT(captures, index);
						continue;
					}					
						int val = Search(!tag, depth, 1, (int)0x80000000, (int)0x7FFFFFFF);
						
						if (val < best_eval) {
							path[0] = "";
							path[0] += enumas.names[start][0];
							path[0] += enumas.names[start][1];
							path[0] += enumas.names[index][0];
							path[0] += enumas.names[index][1];
							path[0] += " ";
							best_eval = val;
							bes_begin = start;
							bes_end = index;
						}
						if (beta > best_eval) {
							beta = best_eval;
						}
						if (alfa >= beta) {
							board.tag = tag;
							board.undo_move_capture(board.hashtable[index], start, captured_index);
							
							break;
						}

						board.tag = tag;
						board.undo_move_quiet(board.hashtable[index], start);
						
						TOGGLEBIT(captures, index);
					
				}
			}
			for (int i = 0; i < board.sizeB; i++) {
				Bitboard captures = board.quietB[i];
				if (board.piecesB[i].captured)continue;
				//DEBUG::Print(captures);
				while (_BitScanForward64(&index, captures)) {
					int begin = board.hashtable[board.piecesB[i].position];
					int start = board.piecesB[i].position;
					board.do_move_quiet(begin, index);
					if (board.black_in_check) {
						
						board.undo_move_quiet(board.hashtable[index], start);
						
						TOGGLEBIT(captures, index);
						continue;
					}
						int val = Search(!tag, depth, 1, (int)0x80000000, (int)0x7FFFFFFF);
						if (val == 0x7FFFFFFF - 10000) {
							path[0] = "";
							path[0] += enumas.names[start][0];
							path[0] += enumas.names[start][1];
							path[0] += enumas.names[index][0];
							path[0] += enumas.names[index][1];
							path[0] += " ";
							best_eval = val;
							bes_begin = start;
							bes_end = index;
							return;
						}
						if (val < best_eval) {
							path[0] = "";
							path[0] += enumas.names[start][0];
							path[0] += enumas.names[start][1];
							path[0] += enumas.names[index][0];
							path[0] += enumas.names[index][1];
							path[0] += " ";
							best_eval = val;
							bes_begin = start;
							bes_end = index;
						}
						if (beta > best_eval) {
							beta = best_eval;
						}
						if (alfa >= beta) {
							board.tag = tag;
							board.undo_move_quiet(board.hashtable[index], start);
							break;
						}

						board.tag = tag;
						board.undo_move_quiet(board.hashtable[index], start);
						TOGGLEBIT(captures, index);
					
				}
			}
		}
	}
	int Search(bool tag, int depth, int current, int alfa, int beta) {	
		if (depth == current) {
			if (tag && board.eval == (int)0x80000000) {
				return (int)0x80000000 + current * 1000;
			}
			if(!tag && board.eval == (int)0x7FFFFFFF){
				return (int)0x7FFFFFFF - current * 1000;
			}
			return board.eval;
		}
		board.tag = tag;
		unsigned long index;
		if (tag) {
			int eval = (int)0x80000000;
			bool def = 1;
			bool moved = 0;
			for (int i = 0; i < board.sizeW; i++) {
				if (board.piecesW[i].captured)continue;
				Bitboard captures = board.capturesW[i];
				while (_BitScanForward64(&index, captures)) {
					int captured_index;
					int begin = board.hashtable[board.piecesW[i].position];
					int start = board.piecesW[i].position;
					bool state = board.white_in_check;
					board.do_move_capture(begin, index, captured_index);
					if (board.white_in_check) {
						board.undo_move_capture(board.hashtable[index], start, captured_index);
						TOGGLEBIT(captures, index);
						continue;
					}
					moved = 1;
					int val = Search(!tag, depth, current + 1, (int)0x80000000, (int)0x7FFFFFFF);
						
					if (def) {
						def = 0;
						path[current] = "";
						path[current] += enumas.names[start][0];
						path[current] += enumas.names[start][1];
						path[current] += enumas.names[index][0];
						path[current] += enumas.names[index][1];
						path[current] += " ";
						eval = val;
						alfa = eval;
					}
					else {
						if (val > eval) {
							eval = val;
							path[current] = "";
							path[current] += enumas.names[start][0];
							path[current] += enumas.names[start][1];
							path[current] += enumas.names[index][0];
							path[current] += enumas.names[index][1];
							path[current] += " ";
						}
						if (alfa < eval) {
							alfa = eval;
						}
						if (alfa >= beta) {
							board.tag = tag;
							board.undo_move_capture(board.hashtable[index], start, captured_index);
							break;
						}
					}

					board.tag = tag;
					board.undo_move_capture(board.hashtable[index], start, captured_index);
					
					TOGGLEBIT(captures, index);
					
				}
			}	
			
			for (int i = 0; i < board.sizeW; i++) {
				if (board.piecesW[i].captured)continue;
				Bitboard captures = board.quietW[i];
				//DEBUG::Print(captures);
				while (_BitScanForward64(&index, captures)) {				
					int begin = board.hashtable[board.piecesW[i].position];
					int start = board.piecesW[i].position;
					bool state = board.white_in_check;
					board.do_move_quiet(begin, index);
					if (board.white_in_check) {
						
						board.undo_move_quiet(board.hashtable[index], start);
						TOGGLEBIT(captures, index);
						continue;
					}
					moved = 1;
						int val = Search(!tag, depth, current + 1, (int)0x80000000, (int)0x7FFFFFFF);
						if (def) {
							def = 0;
							path[current] = "";
							path[current] += enumas.names[start][0];
							path[current] += enumas.names[start][1];
							path[current] += enumas.names[index][0];
							path[current] += enumas.names[index][1];
							path[current] += " ";
							eval = val;
							alfa = eval;
						}
						else {
							if (val > eval) {
								eval = val;
								path[current] = "";
								path[current] += enumas.names[start][0];
								path[current] += enumas.names[start][1];
								path[current] += enumas.names[index][0];
								path[current] += enumas.names[index][1];
								path[current] += " ";
							}
							if (alfa < eval) {
								alfa = eval;
							}
							if (alfa >= beta) {
								board.tag = tag;
								board.undo_move_quiet(board.hashtable[index], start);
								
								break;
							}
						}

						board.tag = tag;
						board.undo_move_quiet(board.hashtable[index], start);						
						TOGGLEBIT(captures, index);
					
				}
			}
			if (!moved && board.white_in_check == 0) {
				return 0x7FFFFFFF;
			}
			if (!moved && board.white_in_check == 1) {
				return eval + current * 1000;
			}		
			return eval;
		}
		else {
			
			int eval = (int)0x7FFFFFFF;
			bool def = 1;
			bool moved = 0;
			//std::wcout << "juodas\n";
			for (int i = 0; i < board.sizeB; i++) {
				if (board.piecesB[i].captured)continue;
				Bitboard captures = board.capturesB[i];
				while (_BitScanForward64(&index, captures)) {
					int captured_index;
					int begin = board.hashtable[board.piecesB[i].position];
					int start = board.piecesB[i].position;
					board.do_move_capture(begin, index, captured_index);
					if (board.black_in_check) {
						board.undo_move_capture(board.hashtable[index], start, captured_index);
						TOGGLEBIT(captures, index);
						continue;
					}
					moved = 1;
						int val = Search(!tag, depth, current + 1, (int)0x80000000, (int)0x7FFFFFFF);
						if (def) {
							eval = val;
							def = 0;
							path[current] = "";
							path[current] += enumas.names[start][0];
							path[current] += enumas.names[start][1];
							path[current] += enumas.names[index][0];
							path[current] += enumas.names[index][1];
							path[current] += " ";
						}
						else {
							if (val < eval) {
								eval = val;
								path[current] = "";
								path[current] += enumas.names[start][0];
								path[current] += enumas.names[start][1];
								path[current] += enumas.names[index][0];
								path[current] += enumas.names[index][1];
								path[current] += " ";
							}
							if (eval < beta) {
								beta = eval;
							}
							if (alfa >= beta) {
								board.tag = tag;
								board.undo_move_capture(board.hashtable[index], start, captured_index);					
								break;
							}

						}
						board.tag = tag;
						board.undo_move_capture(board.hashtable[index], start, captured_index);					
						TOGGLEBIT(captures, index);
					
				}
			}				
			for (int i = 0; i < board.sizeB; i++) {
				Bitboard captures = board.quietB[i];
				//DEBUG::Print(captures);
				//std::cout << tag << std::endl;
				if (board.piecesB[i].captured)continue;
				while (_BitScanForward64(&index, captures)) {					
					int begin = board.hashtable[board.piecesB[i].position];
					int start = board.piecesB[i].position;
					board.do_move_quiet(begin, index);
					if (board.black_in_check) {
						board.undo_move_quiet(board.hashtable[index], start);
						TOGGLEBIT(captures, index);
						continue;
					}
					moved = 1;
						int val = Search(!tag, depth, current + 1, (int)0x80000000, (int)0x7FFFFFFF);						
						if (def) {
							eval = val;
							def = 0;
							path[current] = "";
							path[current] += enumas.names[start][0];
							path[current] += enumas.names[start][1];
							path[current] += enumas.names[index][0];
							path[current] += enumas.names[index][1];
							path[current] += " ";
						}
						else {
							if (val < eval) {
								eval = val;
								path[current] = "";
								path[current] += enumas.names[start][0];
								path[current] += enumas.names[start][1];
								path[current] += enumas.names[index][0];
								path[current] += enumas.names[index][1];
								path[current] += " ";
							}
							if (eval < beta) {
								beta = eval;
							} 
							if (alfa >= beta) {
								board.tag = tag;
								board.undo_move_quiet(board.hashtable[index], start);
								break;
							}

						}

						board.tag = tag;
						board.undo_move_quiet(board.hashtable[index], start);						
						TOGGLEBIT(captures, index);
					
				}
			}
			if (!moved && board.black_in_check == 0) {
				return (int)0x80000000;
			}
			if (!moved && board.black_in_check == 1) {
				return eval - current * 1000;
			}
			return eval;
		}
	}

	struct BestMove {
		int begin;
		int end;
		string path[5] = { "", "", "", "", "" };
		int eval;
	};
}