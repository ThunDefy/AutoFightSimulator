#include <SFML/Graphics.hpp>
#include <iostream>
#include "Header.h"

#define INF numeric_limits<int>::max()

#include <cstdlib>
#include <string>
#include <sstream>


using namespace sf;
using namespace std;

void update_sHp(Sprite& sHp, int hp) {
	if (hp < 0) hp = 0;
	float x = 2.68 * hp;
	sHp.setTextureRect(IntRect(0, 0, x, 29));
}

void update_sEn(Sprite& sEn, int en) {
	if (en >= 100) {
		en = 100;
		sEn.setColor(sf::Color::Yellow);
	} else sEn.setColor(sf::Color::White);

	float x = 5.37 * en;
	sEn.setTextureRect(IntRect(0, 0, x, 29));

}

void update_dmg_txt(Text& txt_dmg, Text& txt_dmg_shedow, string txt) {
	ostringstream char_dmg;
	char_dmg << txt;
	txt_dmg.setString(char_dmg.str());
	txt_dmg_shedow.setString(char_dmg.str());
}

void update_dmg_txt(Text& txt_dmg, Text& txt_dmg_shedow, int num) {
	ostringstream char_dmg;
	char_dmg << 0-num;
	txt_dmg.setString(char_dmg.str());
	txt_dmg_shedow.setString(char_dmg.str());
}

void update_txt(Text& txt, int num) {
	
	ostringstream charr;
	if (num == INF)charr << "Infinitely";
	else charr << num;
	txt.setString(charr.str());
}

