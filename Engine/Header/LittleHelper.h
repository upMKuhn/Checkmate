#pragma once
#include "stdafx.h"
#ifndef SSTREAM_H
#define SSTREAM_H
#include <sstream>
#endif
#include <boost\algorithm\algorithm.hpp>
#include <boost\algorithm\string.hpp>

namespace Checkmate
{
	std::string& strFormat(std::string& str, char inserts[], int length);
	
	// trim from start
	static inline std::string &ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
		return s;
	}

	// trim from end
	static inline std::string &rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	// trim from both ends
	static inline std::string &trim(std::string &s) {
		return ltrim(rtrim(s));
	}


	inline std::string square_toString(Square s)
	{
		string st = string();
		st += file_tochar(file_of(s));
		st += '0' + rank_of(s) + 1;
		return st;
	}

	inline std::string movetype_tostring(Move m)
	{
		string st = string();
		switch (type_of(m))
		{
		case NORMAL:
			st = "NORMAL";
			break;
		case PROMOTION:
			st = "PROMOTION";
			break;
		case CAPTURE:
			st = "CAPTURE";
			break;
		case CASTLING:
			st = "CASTELING";
			break;
		case ENPASSANT:
			st = "ENPASSANT";
			break;
		default:
			st = "UNDEFINED!";
			break;
		}
		return st;
	}

	inline std::string compactMoveToString(Move m)
	{
		return square_toString(from_sq(m)) + square_toString(to_sq(m));
	}

	inline std::string move_tostring(Move m)
	{
		string st = string();
		st += square_toString(from_sq(m)) + square_toString(to_sq(m));

		Piece capture = make_piece(WHITE, capture_type(m));
		Piece promotion = make_piece(WHITE, promotion_type(m));

		if (capture != NO_PIECE)
		{
			st +=  piece_tochar(capture);
		}

		st += " - " + movetype_tostring(m);


		return st;
	}


}
