#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <windows.h>




using namespace sf;
using namespace std;



class Animation {
private:
	int imageCount;
	int frize_anim=0;

	Vector2u currentImage;
	IntRect uvRect;

	Texture Hero_texture;

	float totalTime;
	float switchTime = 0.13f;

	bool fin=0;

public:

	Animation() {};
	~Animation() {};
	void Set_Animation(int Animation_num) {

		switch (Animation_num) {
		case 1: imageCount = 2;  break;
		case 2: imageCount = 4;  break;
		case 3: imageCount = 6;  break;
		case 4: imageCount = 6;  break;
		case 5: imageCount = 6;  break;
		case 6: imageCount = 8;  break;
		case 7: imageCount = 8;  break;
		};

		uvRect.width = (624 * imageCount) / imageCount; 
		uvRect.height = 624 * Animation_num;

		this->switchTime = switchTime;
		totalTime = 0.0f;
		currentImage.x = 0;
	}


	void Update(float deltaTime, bool side) {

		currentImage.y = 1;
		totalTime += deltaTime;

		if (totalTime >= switchTime) {
			totalTime -= switchTime;
			currentImage.x++;

			if (currentImage.x >= imageCount) {
				if (frize_anim) { currentImage.x = imageCount - 1; fin = 1; }
				else {
					currentImage.x = 0;
					fin = 1;
				}
			}
		}

		if (side) {
			uvRect.left = currentImage.x * uvRect.width;
		}
		else {
			uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
			uvRect.width = -abs(uvRect.width);
		}

		uvRect.top = uvRect.height;
	}

	bool Animation_Done() {
		if (fin) { fin = 0; return true; }
		else return false;
	}

	void Frize() {
		this->frize_anim = 1;
	}

	void un_frize() {
		currentImage.x = 0;
	}

	IntRect get_rect() {
		return uvRect;
	}

};

class tPlayer {
protected:
	int win_count=0;
	int Max_HP;
	int HP;
	int STR;
	int AGI;
	int CritRate;
	int Crit;
	int side;
	int Ult_energy = 0;

private:

	Texture Hero_texture;
	Sprite Hero_sprite;

	Animation all[7];

public:

	tPlayer() { set_all_animations(); }
	~tPlayer() {}

	void set_texture(string Hero_name) {
		Hero_texture.loadFromFile("img/char/" + Hero_name + "/" + Hero_name + "_all.png");
		Hero_sprite.setTexture(Hero_texture);

		if (side) { Hero_sprite.setTextureRect(IntRect(0, 174, 400, 450)); Hero_sprite.setPosition(370, 100); }
		else { Hero_sprite.setTextureRect(IntRect(0 + 400, 174, -400, 450)); Hero_sprite.setPosition(390, 100); }
	}

	Sprite get_sprite() {
		return Hero_sprite;
	}

	Texture get_texture() {
		return  Hero_texture;
	}

	void set_rect(IntRect rect) {
		Hero_sprite.setTextureRect(rect);
	}

	bool get_side() {
		return side;
	}

	void set_all_animations() {
		all[0].Set_Animation(1);
		all[1].Set_Animation(2);
		all[2].Set_Animation(3);
		all[3].Set_Animation(4);
		all[4].Set_Animation(5);
		all[5].Set_Animation(6);
		all[6].Set_Animation(7);
	}

	bool play_animation(float deltaTime, int n) {
		if (n == 4) { 
			all[n].Frize();
		}
		all[n].Update(deltaTime, side);
		set_rect(all[n].get_rect());
		if (all[n].Animation_Done()) { return true; }
		else  return false;
	}

	virtual void get_hit(int dmg) = 0;
	virtual int do_damage(tPlayer& enemy, int hit_type) = 0;
	virtual int set_hit() = 0;

	void Reset_Player() { if (HP <= 0) all[4].un_frize(); HP = Max_HP; Ult_energy = 0; }

	void Reset_Wins_Count() { win_count = 0; }

	bool death() { if (HP <= 0)return 1; else return 0; }

	int get_AGI() { return AGI; }

	int get_STR() { return STR; }

	int get_HP() { return HP; }

	int get_UltEnergy() { return Ult_energy; }

	int get_wins_count() { return win_count; }

	Sprite get_Sprite() { return Hero_sprite; }
};


class Punk : public tPlayer {
private:

public:

	Punk(int iside) {
		STR = 15; Max_HP = HP = 200; AGI = 40; CritRate = 60; Crit = 110; side = iside;  set_texture("Punk");
	}


	virtual int do_damage(tPlayer& enemy, int hit_type) {
		int dmg = STR;
		bool ult_can_energy_up = true;
		switch (hit_type)
		{
		case 5:
			dmg *= 3;
			ult_can_energy_up = false;
			break;
		case 6:
			dmg *= 2;
			ult_can_energy_up = false;
			break;
		}

		float crit = float(dmg * (Crit / 100.f));
		int do_crit = rand() % 91;
		int miss_rate = rand() % 91;

		if (miss_rate > enemy.get_AGI()) {
			cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nPunk hit: " << dmg << " | crit: " << crit << " rate: " << do_crit << "  hit type: " << hit_type;
			if (do_crit <= CritRate) { dmg += crit; enemy.get_hit(dmg); }
			else  enemy.get_hit(dmg);
			if (ult_can_energy_up) Ult_energy += 20;
			if (enemy.death()) win_count++;
			cout << "\nPunk's Ult energy now : " << Ult_energy << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			return dmg;
		}
		else {
			cout << "\nPunk MISS\n";
			return 0;
		}
	}

	virtual void get_hit(int dmg = 0) {
		HP -= dmg;
		cout << "\nPunk Lost: " << dmg << " Now: " << HP << "\n";
	}

	virtual int set_hit() {
		if (Ult_energy >= 100) {
			Ult_energy = 0;
			return 5;
		}
		int type = rand() % 101;

		if (type <= 45) { return 3; }
		else if (type <= 90) { return 2; }
		else return 6;
	}
};

class Biker : public tPlayer {
	private:

	public:
		
	Biker(int iside) {
		STR = 20;  Max_HP = HP = 200; AGI = 25;  CritRate = 30; Crit = 140; side = iside;  set_texture("Biker");
	}

	virtual int do_damage(tPlayer& enemy, int hit_type) {
		int dmg = STR;
		bool ult_can_energy_up = true;
		switch (hit_type)
		{
		case 5:
			dmg *= 3;
			ult_can_energy_up = false;
			break;
		case 6:
			dmg *= 2;
			ult_can_energy_up = false;
			break;
		}

		float crit = float(dmg * (Crit / 100.f));
		int do_crit = rand() % 101;
		int miss_rate = rand() % 101;

		if (miss_rate > enemy.get_AGI()) {
			cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nBiker hit: " << dmg << " | crit: " << crit << " rate: " << do_crit << "  hit type: " << hit_type;
			if (do_crit <= CritRate) { dmg += crit; enemy.get_hit(dmg); }
			else  enemy.get_hit(dmg);
			if (ult_can_energy_up) Ult_energy += 20;
			if (enemy.death()) win_count++;
			cout << "\nBiker's Ult energy now : " << Ult_energy << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			return dmg;
		}
		else {
			cout << "\nBiker MISS\n";
			return 0;
		}

	}

	virtual void get_hit(int dmg = 0) {
		HP -= dmg;
		cout << "\nBiker Lost: " << dmg << " Now: " << HP << "\n";
	}

	virtual int set_hit() {
		
		if (Ult_energy >= 100) {
			Ult_energy = 0;
			return 5;
		}
		int type = rand() % 101;

		if (type <= 45) { return 3; }
		else if (type <= 90) { return 2; }
		else return 6;
	}

};

class Cyborg : public tPlayer {
private:

public:

	Cyborg(int iside) {
		STR = 25;  Max_HP = HP = 200; AGI = 15; CritRate = 17; Crit = 170; side = iside;  set_texture("Cyborg");
	}

	virtual int do_damage(tPlayer& enemy, int hit_type) {
		int dmg = STR;
		bool ult_can_energy_up = true;
		switch (hit_type)
		{
		case 5:
			dmg *= 4;
			ult_can_energy_up = false;
			break;
		case 6:
			dmg *= 3;
			ult_can_energy_up = false;
			break;
		}

		float crit = float(dmg * (Crit / 100.f));
		int do_crit = rand() % 101;
		int miss_rate = rand() % 111;

		if (miss_rate > enemy.get_AGI()) {
			cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\nCyborg hit: " << dmg << " | crit: " << crit << " rate: " << do_crit << "  hit type: " << hit_type;
			if (do_crit <= CritRate) { dmg += crit; enemy.get_hit(dmg); }
			else  enemy.get_hit(dmg);
			if (ult_can_energy_up) Ult_energy += 15;
			if (enemy.death()) win_count++;
			cout << "\nCyborg's Ult energy now : " << Ult_energy << "\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
			return dmg;	
		}
		else {
			cout << "\nCyborg MISS\n";
			return 0;
		}
	}

	virtual void get_hit(int dmg = 0) {
		HP -= dmg;
		cout <<"\nCyborg Lost: " << dmg << " Now: " << HP << "\n";
	}

	virtual int set_hit() {
		
		if (Ult_energy >= 100) {
			Ult_energy = 0;
			return 5;
		}
		int type = rand() % 101;

		if (type <= 45) { return 3; }
		else if (type <= 95) { return 2; }
		else return 6;
	}

};


void update_sHp(Sprite& sHp, int hp);
void update_sEn(Sprite& sEn, int en);
void update_dmg_txt(Text& txt_dmg, Text& txt_dmg_shedow, string txt);
void update_dmg_txt(Text& txt_dmg, Text& txt_dmg_shedow, int num);
void update_txt(Text& txt, int num);




