
//libs
#include <SFML/Graphics.hpp> //includes SFML libraries
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <windows.h> //Needed for the Sleep function
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf; //for graphics

//Refactored this to high heaven, reducing the line count by ~68%
//Clean and efficient code is a lifestyle baby

namespace allFuncs{

    inline void confLabel(Text &chosenLabel, Font &chosenFont, int fontSize, Text::Style fontStyle, Color chosenColor, int posX, int posY){
        chosenLabel.setFont(chosenFont);
        chosenLabel.setCharacterSize(fontSize);
        chosenLabel.setStyle(fontStyle);
        chosenLabel.setFillColor(chosenColor);
        chosenLabel.setPosition(posX, posY);
    };

    inline void confSprite(Sprite &chosenSprite, int scaleX, int scaleY, int originX, int originY, int posX, int posY){
        chosenSprite.setScale(scaleX, scaleY);
        chosenSprite.setOrigin(originX, originY);
        chosenSprite.setPosition(posX, posY);
    };

    inline void pause(int duration_in_seconds) {
        Sleep(duration_in_seconds*1000); //wait 2 seconds
    };

    inline void echo(string inputText, Text &targetLabel) { //I like echo better then cout/print, bash supremacy
        targetLabel.setString(inputText); //set the top label to the argument
        targetLabel.setFillColor(sf::Color::White);

        //pause(2);
    };

    inline void winEcho(RenderWindow &window, Text &chosenLabel, string chosenText, Color chosenColour) {
        //sf::Color::chosenColour;

        window.clear();
        chosenLabel.setString(chosenText);
        chosenLabel.setFillColor(chosenColour);
        window.draw(chosenLabel);
        window.display();
    }

    //Put some instructions onscreen like the intro credits
    inline void flashEcho(RenderWindow &window, Text &alertText, Color chosenColour, float duration, string chosenText) {
        Clock waitTime;
        alertText.setFillColor(chosenColour);

        float cycleTime = duration/3; //Total time a single blink cycle, always three blinks
        float blinkTime = cycleTime/2; //Time text can be visible for in a cycle, half a cycle

        for(int i=0; i<(cycleTime); i++) {

            //---- Blink (Filled)
            window.clear();
            alertText.setString(chosenText);
            window.draw(alertText);

            while (waitTime.getElapsedTime().asSeconds() < blinkTime) {
                //Do nothing
            };  waitTime.restart();

            window.display();

            //---- Blink (blank)

            window.clear();
            alertText.setString("");
            window.draw(alertText);

            while (waitTime.getElapsedTime().asSeconds() < blinkTime) {
                //Do nothing
            };  waitTime.restart();

            window.display();
        }
}


    //Play intro instructions
    inline void intro(Text &chosenLabel, RenderWindow &window, Clock clickTock, Text alertText) {
        string script[9] = {
        "(Skip this with Left Shift)",
        "Welcome to the casino!",
        "This is a simplified blackjack game.",
        "The rules are simple.",
        "Get as close to 21 as possible.",
        "If you get any more, you lose.",
        "The dealer will try for the same.",
        "But, they can't take cards after 16.",
        "Let's play!"
        };

        //Number of lines in the intro script array
        int linesInScript = sizeof(script) / sizeof(script[0]);

        Clock waitTime;
        int duration = 3.0;
        Color skipCol = sf::Color::Magenta;
        Color classicCol = sf::Color::Yellow;

        for (int i = 0; i < linesInScript; i++) {
            winEcho(window, chosenLabel, script[i], classicCol);
            while (waitTime.getElapsedTime().asSeconds() < 1) {
                //Do nothing

                //If shift is clicked, skip all the text
                if(Keyboard::isKeyPressed(Keyboard::LShift) && clickTock.getElapsedTime().asMilliseconds() > 5) {
                    //p1.updateSize(400,world);
                    clickTock.restart();
                    flashEcho(window, alertText, skipCol, duration, "Skipping...");

                    goto skipped; //I know I could've used a bool flag but I wanted to use goto at least once, for the lolz!! Plus it's actually good sometimes for optimization
                }

            }; waitTime.restart();
        }

        //Goto label, skipping all the text
        skipped:
        winEcho(window, chosenLabel, "",classicCol);

    }

    //IN PROGRESS!!! NOT FUNCTIONAL YET!!!
    inline void setupCardTextures(vector <string> deck, Texture cardTextures[52], Text &chosenLabel, RenderWindow &window, Text alertText, Clock clickTock, Sound &gamblingMusic, SoundBuffer &gamblingMusicFile) {
        string loadingScript[7] = {
            "Loading Club Cards...",
            "Loading Diamond Cards...",
            "Loading Spade Cards...",
            "Loading Heart Cards...",
            "Loading Royals...",
            "Finishing up...",
            "Textures loaded!"
        };

        //Number of lines in the intro script array
        int linesInLoadingScript = sizeof(loadingScript) / sizeof(loadingScript[0]);

        Clock waitTime;
        int duration = 3.0;
        Color skipCol = sf::Color::Magenta;
        Color classicCol = sf::Color::Yellow;

            /*
            #This is the python code I used to automate generating the texture loading commands
            #I'm not great at python so to be able to make this with all its regexing feels pretty epic
            #I have pasted it here for my pride
            suits = ["clubs", "diamonds", "spades", "hearts"]

            for(j) in range(0,4,1):
                for (i) in range(0, 10, 1):
                    print(f"cardTextures[{i + (j * 10)}].loadFromFile(\"cards/{i+1}{suits[j]}.png\");")

            */

            flashEcho(window, alertText, skipCol, duration, loadingScript[0]);
            cardTextures[0].loadFromFile("cards/1clubs.png");
            cardTextures[1].loadFromFile("cards/2clubs.png");
            cardTextures[2].loadFromFile("cards/3clubs.png");
            cardTextures[3].loadFromFile("cards/4clubs.png");
            cardTextures[4].loadFromFile("cards/5clubs.png");
            cardTextures[5].loadFromFile("cards/6clubs.png");
            cardTextures[6].loadFromFile("cards/7clubs.png");
            cardTextures[7].loadFromFile("cards/8clubs.png");
            cardTextures[8].loadFromFile("cards/9clubs.png");
            cardTextures[9].loadFromFile("cards/10clubs.png");
            flashEcho(window, alertText, skipCol, duration, loadingScript[1]);
            cardTextures[10].loadFromFile("cards/1diamonds.png");
            cardTextures[11].loadFromFile("cards/2diamonds.png");
            cardTextures[12].loadFromFile("cards/3diamonds.png");
            cardTextures[13].loadFromFile("cards/4diamonds.png");
            cardTextures[14].loadFromFile("cards/5diamonds.png");
            cardTextures[15].loadFromFile("cards/6diamonds.png");
            cardTextures[16].loadFromFile("cards/7diamonds.png");
            cardTextures[17].loadFromFile("cards/8diamonds.png");
            cardTextures[18].loadFromFile("cards/9diamonds.png");
            cardTextures[19].loadFromFile("cards/10diamonds.png");
            flashEcho(window, alertText, skipCol, duration, loadingScript[2]);
            cardTextures[20].loadFromFile("cards/1spades.png");
            cardTextures[21].loadFromFile("cards/2spades.png");
            cardTextures[22].loadFromFile("cards/3spades.png");
            cardTextures[23].loadFromFile("cards/4spades.png");
            cardTextures[24].loadFromFile("cards/5spades.png");
            cardTextures[25].loadFromFile("cards/6spades.png");
            cardTextures[26].loadFromFile("cards/7spades.png");
            cardTextures[27].loadFromFile("cards/8spades.png");
            cardTextures[28].loadFromFile("cards/9spades.png");
            cardTextures[29].loadFromFile("cards/10spades.png");
            flashEcho(window, alertText, skipCol, duration, loadingScript[3]);
            cardTextures[30].loadFromFile("cards/1hearts.png");
            cardTextures[31].loadFromFile("cards/2hearts.png");
            cardTextures[32].loadFromFile("cards/3hearts.png");
            cardTextures[33].loadFromFile("cards/4hearts.png");
            cardTextures[34].loadFromFile("cards/5hearts.png");
            cardTextures[35].loadFromFile("cards/6hearts.png");
            cardTextures[36].loadFromFile("cards/7hearts.png");
            cardTextures[37].loadFromFile("cards/8hearts.png");
            cardTextures[38].loadFromFile("cards/9hearts.png");
            cardTextures[39].loadFromFile("cards/10hearts.png");
            flashEcho(window, alertText, skipCol, duration, loadingScript[4]);

            //--------------ROYALS---------------

            cardTextures[40].loadFromFile("cards/jackspades.png");
            cardTextures[41].loadFromFile("cards/jackdiamonds.png");
            cardTextures[42].loadFromFile("cards/jackhearts.png");
            cardTextures[43].loadFromFile("cards/jackclubs.png");

            cardTextures[44].loadFromFile("cards/queenspades.png");
            cardTextures[45].loadFromFile("cards/queendiamonds.png");
            cardTextures[46].loadFromFile("cards/queenhearts.png");
            cardTextures[47].loadFromFile("cards/queenclubs.png");

            cardTextures[48].loadFromFile("cards/kingspades.png");
            cardTextures[49].loadFromFile("cards/kingdiamonds.png");
            cardTextures[50].loadFromFile("cards/kinghearts.png");
            cardTextures[51].loadFromFile("cards/kingclubs.png");

            flashEcho(window, alertText, skipCol, duration, loadingScript[5]);

            gamblingMusicFile.loadFromFile("Balatro-sound.ogg");
            gamblingMusic.setBuffer(gamblingMusicFile);
            gamblingMusic.setLoop(true);
            gamblingMusic.setVolume(50);
            flashEcho(window, alertText, skipCol, duration, loadingScript[6]);
    }
}
