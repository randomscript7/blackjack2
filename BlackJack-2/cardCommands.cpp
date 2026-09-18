
//libs
#include <SFML/Graphics.hpp> //includes SFML libraries
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
//#include <windows.h>

using namespace std;
using namespace sf; //for graphics

//Texture cardTextures[52];

struct returncard{ //structure that stores card value and suit for pickcard function
    int cardvalue; //the numerical card value
    string cardname; //the exact card name for texturing
    string fixedCardName; //The exact same thing as cardname, but with the second char intact
    int cardTextureIndice;//The indice number of its graphical texture
};

namespace cardManipulation{

    inline void setupDeck(vector <string> &deck) {
        //-------------regular cards-----------

        for (int j = 1; j <= 10; j++) {

            deck.push_back(to_string(j) += "clubs"); //add regular value clubs
        }
        for (int j = 1; j <= 10; j++) {

            deck.push_back(to_string(j) += "diamonds"); //add regular value diamonds
        }
        for (int j = 1; j <= 10; j++) {

            deck.push_back(to_string(j) += "spades"); //add regular value spades
        }
        for (int j = 1; j <= 10; j++) {

            deck.push_back(to_string(j) += "hearts"); //add regular value hearts
        }

        //---------royals-----------

        deck.push_back("jackspades");
        deck.push_back("jackdiamonds");
        deck.push_back("jackhearts");
        deck.push_back("jackclubs");

        deck.push_back("queenspades");
        deck.push_back("queendiamonds");
        deck.push_back("queenhearts");
        deck.push_back("queenclubs");

        deck.push_back("kingspades");
        deck.push_back("kingdiamonds");
        deck.push_back("kinghearts");
        deck.push_back("kingclubs");

        //random_shuffle(deck.begin(), deck.end()); //shuffle that pre-game
    };

    //struct defined by funtion (returns two values)
    inline returncard pickcard(vector <string> deck) { //deck and filename system are different, this system is cross-compatible between the two

    int card; //var to store translated value
    string tmp; //var for untranslated value
    char val; //char value for determining whether second char is a number
    returncard rc; //structure to return card value and name, localized as rc
    tmp = deck.back(); //copy the card the dealer picks up to tmp
    rc.fixedCardName = deck.back(); //Copy the proper card name to be printed to screen (For the issue with second char)
    tmp[1] = val; //second char is copied to val
    if (! val >= 64 && ! val <= 123) { //if second character is NOT a letter (is a number)
        rc.cardvalue = 10; //it must be 10, so that's its int value
        //cout << "card value 10" << endl;
    } else if (tmp[0] >= 48 && tmp[0] <= 57) { //second digit isn't a number so check if the first one is (which it must unless a value of 10)
        rc.cardvalue = (tmp[0] - 48); //the first character is the card value
        //cout << "single digit" << endl;
    } else {
        rc.cardvalue = 10; //card must be a royal, so value is 10
        //cout << "card value 10 (else)" << endl;
    }
    rc.cardname = tmp; //the card name is written exactly in tmp, return it

    return rc; //retun the localized structure with the card name and value
};

    inline int dealergo(int &dcards, vector <string> deck) {

    dcards += pickcard(deck).cardvalue; //take one from the deck and add it to the dealer's hand
    return pickcard(deck).cardvalue; //function exists with card value pulled
    };

    inline int usergo(int &ucards, vector <string> deck) {

    ucards += pickcard(deck).cardvalue; //take one from the deck and add it to the user's hand
    return pickcard(deck).cardvalue; //function exists with card value pulled
};


}

