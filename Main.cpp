#include <conio.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Loca
{
	int x = 0, y = 0;
};

struct WordOnMap
{
	std::string word;

	Loca loca;
};

int main() {

	srand(time(NULL));

	const int WIDTH = 80;
	const int HEIGHT = 40;

	char map[WIDTH][HEIGHT];

	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			map[i][j] = ' ';
		}
	}

	std::string word;
	std::vector<std::string> vword;

	std::vector<WordOnMap> wordonmap;

	std::ifstream infile("Library.lib");// save all the word
	if (!infile.good()) {// file error
		std::cerr << "Can't open file." << std::endl;

		std::cin.get();
	}

	while (!infile.eof()) {
		std::getline(infile, word);

		vword.push_back(word);
	}

	/*for (int i = 0; i < vword.size(); ++i) {
		std::cout << vword.at(i) << std::endl;
	}*/

	const double UPDATA_DISPLAY_TIME = 0.25;
	clock_t updatadelay = clock();

	const double WORD_COME_OUT_TIME = 1;
	clock_t wordcomeoutdelay = clock();

	std::string userinput, userinputb;

	while (true) {
		userinputb = userinput;

		if (double(clock() - updatadelay) / CLOCKS_PER_SEC > UPDATA_DISPLAY_TIME) {
			/*for (int i = 0; i < wordonmap.size(); ++i) {//print new display
				std::string outputword = wordonmap.at(i).word.substr(wordonmap.at(i).loca.x < 0 ? -wordonmap.at(i).loca.x : 0);


			}*/

			for (int i = WIDTH - 1; i > 0; --i) {//every word move one block to right
				for (int j = 0; j < HEIGHT; ++j) {
					map[i][j] = map[i - 1][j];
				}
			}

			for (int i = 0; i < HEIGHT; ++i) {//fill ' ' in each map[0][i];
				map[0][i] = ' ';
			}

			for (int i = 0; i < wordonmap.size(); ++i) {//add the word to the map
				if (wordonmap.at(i).loca.x <= 0) {//need to add ?
					map[0][wordonmap.at(i).loca.y] = wordonmap.at(i).word.at(-wordonmap.at(i).loca.x);
				}

				wordonmap.at(i).loca.x++;//move to right
			}

			system("CLS");//clean up the old display
			for (int i = 0; i < HEIGHT; ++i) {//print new display
				for (int j = 0; j < WIDTH; ++j) {
					std::cout << map[j][i];
				}

				std::cout << std::endl;
			}

			std::cout << userinput;

			updatadelay = clock();
		}

		if (double(clock() - wordcomeoutdelay) / CLOCKS_PER_SEC > WORD_COME_OUT_TIME) {
			int randy = -1;

			do{//choose random line
				randy = rand() % HEIGHT;

				for (int i = 0; i < wordonmap.size(); ++i) {
					if (wordonmap.at(i).loca.y == randy && wordonmap.at(i).loca.x <= 1) {
						randy = -1;

						break;
					}
				}
			}while (randy < 0);

			word = vword.at((rand() * rand()) % vword.size());//the new word on map.

			WordOnMap wom;
			wom.word = word;
			wom.loca.y = randy;
			wom.loca.x = 1 - wom.word.size();

			wordonmap.push_back(wom);

			wordcomeoutdelay = clock();
		}

		while (_kbhit()) {//user pause anykey?
			char ch = _getch();

			if (ch >= 'a' && ch <= 'z') {//only pause [a, b, c, d, ....., z]
				userinput += ch;
			}
			else if (ch == 8 && userinput.size() > 0) {//user delete
				userinput.erase(userinput.size() - 1);
			}
			else if (ch = 13) {// pause enter
				for(int i = 0;i < wordonmap.size();++i){
					if (wordonmap.at(i).word == userinput) {
						userinput.clear();

						for (int j = (wordonmap.at(i).loca.x < 0 ? 0 : wordonmap.at(i).loca.x - 1); j < wordonmap.at(i).word.size() + (wordonmap.at(i).loca.x < 0 ? -wordonmap.at(i).loca.x : wordonmap.at(i).loca.x); ++j){// clean up the word on map
							map[j][wordonmap.at(i).loca.y] = ' ';
						}
						
						wordonmap.erase(wordonmap.begin() + i);
					}
				}
			}
		}

		if (userinputb != userinput) {// not that mush delay
			if (userinputb.size() < userinput.size()) {
				std::cout << userinput.substr(userinputb.size());
			}
			else {
				std::cout << "\r";

				for (int i = 0; i < userinputb.size(); ++i) {
					std::cout.put(' ');
				}

				std::cout << "\r" << userinput;
			}
		}

	}

	std::cin.get();
}
