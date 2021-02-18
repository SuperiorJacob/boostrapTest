#include "Highscores.h"
#include <iostream>
#include <fstream>
#include <direct.h>
#include <windows.h>
#include <Lmcons.h>

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING 1;

#include <experimental/filesystem>

// Experimental, probably fucked.
std::vector<std::string> get_filenames(std::experimental::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	const stdfs::directory_iterator end{};

	for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (stdfs::is_regular_file(*iter))
			filenames.push_back(iter->path().string());
	}

	return filenames;
}
//

void Highscores::Load()
{
	m_user = GetUser();

	for (const auto& name : get_filenames(m_dir + m_gameName))
	{
		std::ifstream hs;
		std::string user;
		std::string score;

		hs.open(name);

		if (hs.is_open())
		{
			getline(hs, user);
			getline(hs, score);

			m_highscores.insert(std::pair<const std::string, int>(user, std::stoi(score)));
		}
	}

	for each (auto & hs in m_highscores)
	{
		std::cout << hs.first << " " << hs.second << std::endl;
	}
}

void Highscores::Draw(aie::Font* font, aie::Renderer2D* render, int x, int y)
{
	render->drawText(font, "HIGHSCORES:", x, y);

	int spot = 0;
	for (auto score : SortScores())
	{
		spot++;
		std::string first = "(" + std::to_string(spot) + ") " + score.first + ": " + std::to_string(score.second);
		render->drawText(font, first.c_str(), x, y - 34 * spot);
	}
}

Highscores::HMap Highscores::GetHighScores()
{
	return m_highscores;
}

bool Highscores::CheckIfHighScore(int a_highscore)
{
	return CheckIfHighScore(GetUser(), a_highscore);
}

bool Highscores::CheckIfHighScore(std::string a_name, int a_highscore)
{
	bool isHighScore = false;
	
	const char* name = a_name.c_str();
	auto search = m_highscores.find(name);

	if (search == m_highscores.end())
	{
		isHighScore = true;
	}
	else
	{
		isHighScore = (search->second < a_highscore);
	}

	return isHighScore;
}

// Adds a highscore to the list using the USERNAME of the PC.
void Highscores::AddHighScore(int a_highscore)
{
	AddHighScore(GetUser(), a_highscore);
}

// Adds a highscore using a custom username input.
void Highscores::AddHighScore(std::string a_name, int a_highscore)
{
	if (!CheckIfHighScore(a_name, a_highscore)) return;

	const char* name = a_name.c_str();
	auto search = m_highscores.find(name);

	if (search != m_highscores.end())
	{
		search->second = a_highscore;
	}

	CreateProfile(m_gameName, a_name, a_highscore);
}

// Creates a directory, if it already exists, returns false.
bool Highscores::CheckDir(std::string a_dir)
{
	int check;
	a_dir = m_dir + a_dir;

	check = mkdir(a_dir.c_str());

	return check;
}

void Highscores::CreateProfile(std::string a_game, std::string a_name, int a_highscore)
{
	CheckDir(a_game);
	
	std::ofstream myfile;
	myfile.open(m_dir + a_game + "/" + a_name + ".txt", std::ofstream::trunc);
	myfile << a_name + "\n" << a_highscore;
	myfile.close();
}

bool cmp(std::pair<std::string, int>& a,
	std::pair<std::string, int>& b)
{
	return a.second > b.second;
}

std::vector<std::pair<std::string, int> > Highscores::SortScores()
{
	std::vector<std::pair<std::string, int> > A;

	// Copy key-value pair from Map 
	// to vector of pairs 
	for (auto& it : m_highscores) {
		A.push_back(it);
	}

	// Sort using comparator function 
	sort(A.begin(), A.end(), cmp);

	return A;
}

std::string Highscores::GetUser()
{
	if (m_user != "ERROR" && m_user != "") return m_user;

	TCHAR username[100];
	DWORD username_len = sizeof(username);
	GetUserName(username, &username_len);

	char user[sizeof(username)];
	wcstombs(user, username, wcslen(username) + 1);

	return user;
}
