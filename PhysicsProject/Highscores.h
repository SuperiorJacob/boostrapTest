#pragma once

#include <Renderer2D.h>
#include <map>
#include <string>

class Highscores
{
public:
	typedef std::map<const std::string, int> HMap;

	void Load();
	void Draw(aie::Font* font, aie::Renderer2D* render, int x, int y);

	HMap GetHighScores();

	bool CheckIfHighScore(int a_highscore);
	bool CheckIfHighScore(std::string a_name, int a_highscore);

	void AddHighScore(int a_highscore);
	void AddHighScore(std::string a_name, int a_highscore);

	bool CheckDir(std::string a_dir);
	void CreateProfile(std::string a_game, std::string a_name, int a_highscore);

	void SetGameName(std::string a_game) { m_gameName = a_game; }

	std::string GetUser();

protected:
	HMap m_highscores;

	std::string m_gameName = "ERROR";

	// DO NOT CHANGE
	char* m_dir = "Z:/Prog_Yr2/Jacobs_Bootstrap_Highscores/DataBank/";
};

