#pragma once
#include <string>
//#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

namespace cnz {
	using namespace std;
	class ScoreManager {
	public:
		ScoreManager();
		~ScoreManager();

		// score get/set
		int GetCurScore();
		void SetCurScore();

		// player name get/set
		string GetCurName();
		void SetCurName();

		// score board read/write
		map<string, int> ReadScoreBoard();
		void WriteScoreBoard();


	private:
		int score;
		string curPlayerName;
		map<string, int> scoreBoard;

	};
}