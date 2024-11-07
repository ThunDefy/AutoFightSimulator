#include <SFML/Graphics.hpp>
#include <iostream>
#include "Header.h"

#include <cstdlib>
#include <string>
#include <sstream>

#define INF numeric_limits<int>::max()

using namespace sf;
using namespace std;

int main()
{

    srand(static_cast<unsigned int>(time(0)));

    RenderWindow window(sf::VideoMode(1400, 787), "Auto Fight");
    
     Biker biker1(1), biker2(0);
     Punk punk1(1), punk2(0);
     Cyborg cyborg1(1), cyborg2(0);

     tPlayer* p1 = &biker1, * p2 = &biker2;


    Clock clock, loop;

    float deltaTime = 0.0f;
    float CurrentFrame = 0;

    Font font, font2;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Can't finde .ttf ";
    }

    if (!font2.loadFromFile("font2.ttf")) {
        std::cerr << "Can't finde .ttf ";
    }

    Text txt_round("0", font, 120), txt_p1_wins("0", font, 100), txt_p2_wins("0", font, 100), txt_dmg_shedow(" ", font2, 100),
        txt_dmg(" ", font2, 90), txt_max_rounds("1", font, 60);

    txt_max_rounds.setFillColor(Color::White);
    txt_max_rounds.setPosition(1180, 95);

    txt_round.setFillColor(Color::White);
    txt_round.setPosition(670, -45);

    txt_p1_wins.setFillColor(Color::Red);
    txt_p1_wins.setPosition(550, -40);

    txt_p2_wins.setFillColor(Color::Red);
    txt_p2_wins.setPosition(800, -40);

    txt_dmg_shedow.setFillColor(Color::Black);
    txt_dmg.setFillColor(Color::White);

    txt_dmg_shedow.setPosition(395, 100);
    txt_dmg.setPosition(400, 100);

    int bg_num = rand() % 5;
    if (bg_num == 0) bg_num = 1;

    Texture Bg, Bar, Hp, En, Start, Select, Stop, Exit, Restart, Rounds;

    switch (bg_num) {
    case 1:
        Bg.loadFromFile("img/City1.png");
        break;
    case 2:
        Bg.loadFromFile("img/City2.png");
        break;
    case 3:
        Bg.loadFromFile("img/City3.png");
        break;
    case 4:
        Bg.loadFromFile("img/City4.png");
        break;
    }

    Rounds.loadFromFile("img/interface/rounds.png");
    Stop.loadFromFile("img/interface/pause.png");
    Exit.loadFromFile("img/interface/exit.png");
    Restart.loadFromFile("img/interface/restart.png");
    Select.loadFromFile("img/interface/select.png");
    Start.loadFromFile("img/interface/start.png");
    Bar.loadFromFile("img/interface/bar.png");
    Hp.loadFromFile("img/interface/health_line.png");
    En.loadFromFile("img/interface/energy_line.png");

    Sprite sBg, sBar1, sHp1, sEn1,  sBar2, sHp2, sEn2 , sStart, sSelect, sStop, sExit, sRestart, sRounds;
    sBg.setTexture(Bg); 
    sStart.setTexture(Start); sSelect.setTexture(Select); sStop.setTexture(Stop);
    sExit.setTexture(Exit); sRestart.setTexture(Restart); sRounds.setTexture(Rounds);
    sBar1.setTexture(Bar); sBar2.setTexture(Bar);
    sHp1.setTexture(Hp); sHp2.setTexture(Hp);
    sEn1.setTexture(En); sEn2.setTexture(En);

    sStart.setPosition(400, 100);
    sSelect.setPosition(450,210);
    sStop.setPosition(10, 80);
    sRounds.setPosition(990, 110);

    sRestart.setPosition(110, 80);
    sExit.setPosition(110, 130);
    

    sHp1.setPosition(0, 6.5);
    sEn1.setPosition(0, 41.5);

    sHp2.setPosition(1400, 36);
    sEn2.setPosition(1400, 71);
    sBar2.setPosition(860, 0);

    sHp2.rotate(180.f); // что бы полоска шла вправо
    sEn2.rotate(180.f);

    sEn1.setTextureRect(IntRect(0, 0, 0, 0));
    sEn2.setTextureRect(IntRect(0, 0, 0, 0));

    sBar2.setTextureRect(IntRect(0 + 540, 0, -540, 77));

    int p1_stat = 1; // Статусы игроков 0 - ранение, 1 - готов бить, 4 - смерть
    int p2_stat = 1;

    int turn = -1 + rand()%3; // Определение кто ходит первым
    int hit_type = 0;   // Тип удара 

    bool round_over = false;
    int round = 0;

    int dm = 0;  // для вывода числа урона

    int p1_char_select = 0;
    int p2_char_select = 0;

    bool start_game = 0;
    bool stop_game = 0;
    bool restart_game = 0;

    int max_round = 1;
    bool final = 0; 

  
    
    while (window.isOpen())
    {
        window.draw(sBg);

        ostringstream char_round, char_p1_wins, char_p2_wins, char_dmg;
        
        float re_loop = loop.getElapsedTime().asMicroseconds();

        deltaTime = clock.restart().asSeconds();

        re_loop = re_loop / 800;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!start_game) {
                if (IntRect(418, 256, 265, 531).contains(Mouse::getPosition(window))) {
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        p1_char_select++;
                        if (p1_char_select > 2)p1_char_select = 0;

                        switch (p1_char_select) {
                        case 0:
                            p1 = &biker1;
                            break;
                        case 1:
                            p1 = &punk1;
                            break;
                        case 2:
                            p1 = &cyborg1;
                            break;
                        }

                    }
                }

                if (IntRect(687, 256, 265, 531).contains(Mouse::getPosition(window))) {
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        p2_char_select++;
                        if (p2_char_select > 2)p2_char_select = 0;

                        switch (p2_char_select) {
                        case 0:
                            p2 = &biker2;
                            break;
                        case 1:
                            p2 = &punk2;
                            break;
                        case 2:
                            p2 = &cyborg2;
                            break;
                        }

                    }
                }

                if (IntRect(987, 110, 409, 49).contains(Mouse::getPosition(window))) {
                    sRounds.setColor(Color::Red);
                    txt_max_rounds.setFillColor(Color::Red);
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        if (max_round == INF) max_round = 1;
                        else if(max_round == 1) max_round = 2;
                        else if (max_round == 2) max_round = 5;
                        else if (max_round == 5) max_round = INF;

                        update_txt(txt_max_rounds, max_round);
                    }
                }
                else { sRounds.setColor(Color::White); txt_max_rounds.setFillColor(Color::White);}

                if (IntRect(418, 109, 530, 73).contains(Mouse::getPosition(window))) {
                    sStart.setColor(Color::Red);
                    if (Mouse::isButtonPressed(Mouse::Left)) { start_game = 1; update_dmg_txt(txt_dmg, txt_dmg_shedow, " "); final = 0; }
                }else sStart.setColor(Color::White);
            }

            if (IntRect(0, 77, 104, 77).contains(Mouse::getPosition(window))) {
                sStop.setColor(Color::Red);
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    if (!stop_game)stop_game = 1;
                    else stop_game = 0;
                }
            }else sStop.setColor(Color::White);

            if (stop_game) {
                if (IntRect(124, 79, 193, 49).contains(Mouse::getPosition(window))) {
                    sRestart.setColor(Color::Red);
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        restart_game = 1;
                        stop_game = 0;
                    }
                }
                else sRestart.setColor(Color::White);

                if (IntRect(115, 136, 89, 47).contains(Mouse::getPosition(window))) {
                    sExit.setColor(Color::Red);
                    if (Mouse::isButtonPressed(Mouse::Left)) {
                        return 0;
                    }
                }
                else sExit.setColor(Color::White);
            }

        }


        if (!stop_game) {
            if (p1_stat != 4)p1->play_animation(deltaTime, 1);
            if (p2_stat != 4)p2->play_animation(deltaTime, 1);

            if (start_game) {
                if (round >= max_round) { 
                    final = 1;
                    restart_game = 1;
                }
                if (!round_over && !restart_game) {
                    if (re_loop >= 1000) {
                        if (turn == 1) {
                            dm = 0;
                            if (!p1->death()) {
                                if (!hit_type)hit_type = p1->set_hit();
                                if (p1->play_animation(deltaTime, hit_type)) {
                                    if (dm = p1->do_damage(*p2, hit_type)) {
                                        p2_stat = 0; turn = 0; hit_type = 0;
                                    }
                                    else {
                                        p2_stat = 1; turn = 0; hit_type = 0; dm = 0;
                                    }

                                    update_sEn(sEn1, p1->get_UltEnergy());

                                    if (dm) {
                                        update_dmg_txt(txt_dmg, txt_dmg_shedow, dm);

                                        if (dm > p1->get_STR()) {
                                            txt_dmg.setFillColor(Color::Yellow);
                                        }
                                        else {
                                            txt_dmg.setFillColor(Color::White);
                                        }
                                    }
                                    else {
                                        txt_dmg.setFillColor(Color::White);
                                        update_dmg_txt(txt_dmg, txt_dmg_shedow, "Miss");
                                    }
                                }
                            }

                            if (p1_stat == 0) if (p1->play_animation(deltaTime, 0)) {
                                if (p1->death()) {
                                    p1_stat = 4;
                                }
                                else { p1_stat = 1; loop.restart(); }
                                update_sHp(sHp1, p1->get_HP());
                            }

                            if (p1_stat == 4) {
                                if (p1->play_animation(deltaTime, 4)) {
                                    cout << "\n\nP1  DIE\n";
                                    round_over = true;

                                    txt_dmg.setFillColor(Color::Red);
                                    update_dmg_txt(txt_dmg, txt_dmg_shedow, "Die");
                                    loop.restart();
                                }
                            }

                        }
                        else {
                            dm = 0;
                            if (!p2->death()) {
                                if (!hit_type)hit_type = p2->set_hit();
                                if (p2->play_animation(deltaTime, hit_type)) {
                                    if (dm = p2->do_damage(*p1, hit_type)) {
                                        p1_stat = 0;  turn = 1; hit_type = 0;
                                    }
                                    else {
                                        p1_stat = 1;  turn = 1; hit_type = 0; dm = 0;
                                    }
                                    update_sEn(sEn2, p2->get_UltEnergy());

                                    if (dm) {
                                        update_dmg_txt(txt_dmg, txt_dmg_shedow, dm);

                                        if (dm > p2->get_STR()) {
                                            txt_dmg.setFillColor(Color::Yellow);
                                        }
                                        else {
                                            txt_dmg.setFillColor(Color::White);
                                        }
                                    }
                                    else {
                                        txt_dmg.setFillColor(Color::White);
                                        update_dmg_txt(txt_dmg, txt_dmg_shedow, "Miss");
                                    }
                                }
                            }

                            if (p2_stat == 0) if (p2->play_animation(deltaTime, 0)) {
                                if (p2->death()) {
                                    p2_stat = 4;
                                }
                                else { p2_stat = 1; loop.restart(); }
                                update_sHp(sHp2, p2->get_HP());
                            }
                            if (p2_stat == 4) {
                                if (p2->play_animation(deltaTime, 4)) {
                                    cout << "\n\nP2 DIE\n";
                                    round_over = true;

                                    txt_dmg.setFillColor(Color::Red);
                                    update_dmg_txt(txt_dmg, txt_dmg_shedow, "Die");
                                    loop.restart();
                                }
                            }
                        }

                    }
                }
                else {
                    if (re_loop >= 4000) {
                        if(!restart_game)cout << "\n\nR O U N D  O V E R\n\n";
                        else cout << "\n\nR E S T A R T  G A M E  \n\n";
                        
                        p1->Reset_Player();
                        p2->Reset_Player();

                        update_sHp(sHp1, p1->get_HP());
                        update_sHp(sHp2, p2->get_HP());

                        update_sEn(sEn1, p1->get_UltEnergy());
                        update_sEn(sEn2, p2->get_UltEnergy());

                        update_dmg_txt(txt_dmg, txt_dmg_shedow, " ");

                        if (!restart_game) {
                            round++;
                            p1_stat = 1;
                            p2_stat = 1;
                            round_over = false; 
                        }
                        else {
                            if (p1->get_wins_count() == p2->get_wins_count()) {
                                update_dmg_txt(txt_dmg, txt_dmg_shedow, " ");
                            }
                            else if (p1->get_wins_count() > p2->get_wins_count()) {
                                txt_dmg.setFillColor(Color::Yellow);
                                txt_dmg_shedow.setPosition(29, 250);
                                txt_dmg.setPosition(30, 250);
                                update_dmg_txt(txt_dmg, txt_dmg_shedow, "P1 WIN");                             
                            }
                            else {
                                txt_dmg.setFillColor(Color::Yellow);
                                txt_dmg_shedow.setPosition(950, 250);
                                txt_dmg.setPosition(951, 250);
                                update_dmg_txt(txt_dmg, txt_dmg_shedow, "P2 WIN");
                            }
                            p1->Reset_Wins_Count();
                            p2->Reset_Wins_Count();
                            round = 0;
                            p1_stat = 1;
                            p2_stat = 1;
                            start_game = 0;
                            final = 1;
                            restart_game = 0;
                        }

                        char_round << round;
                        char_p1_wins << p1->get_wins_count();
                        char_p2_wins << p2->get_wins_count();

                        txt_round.setString(char_round.str());
                        txt_p1_wins.setString(char_p1_wins.str());
                        txt_p2_wins.setString(char_p2_wins.str());

                    }
                }
            }
        }
        else {
            window.draw(sRestart);
            window.draw(sExit);
            
        }

        if (!start_game) {
            window.draw(sStart);
            window.draw(sSelect); 
            window.draw(sRounds);
            window.draw(txt_max_rounds);
        }
        else window.draw(sStop);

        window.draw(sHp1);
        window.draw(sEn1);

        

        window.draw(sHp2);
        window.draw(sEn2);

        window.draw(sBar1);
        window.draw(sBar2);

        window.draw(txt_round);
        window.draw(txt_p1_wins);
        window.draw(txt_p2_wins);

        if (!final) {
            if (turn == 1) {
                txt_dmg_shedow.setPosition(319, 250);
                txt_dmg.setPosition(320, 250);
                window.draw(txt_dmg_shedow);
                window.draw(txt_dmg);
            }
            else {
                txt_dmg_shedow.setPosition(919, 250);
                txt_dmg.setPosition(920, 250);
                window.draw(txt_dmg_shedow);
                window.draw(txt_dmg);
            }
        } else{
            window.draw(txt_dmg_shedow);
            window.draw(txt_dmg);
        }

        if (turn) {
            window.draw(p2->get_sprite());  window.draw(p1->get_sprite());
        }

        if (!turn) {
            window.draw(p1->get_sprite());  window.draw(p2->get_sprite());
        }

        

        window.display();
    }


    return 0;
}