//libs
#include <SFML/Graphics.hpp> //includes SFML libraries
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <SFML/Audio.hpp>
#include "globalFuncs.cpp"
#include "cardCommands.cpp"

using namespace std;
using namespace sf; //for graphics
using namespace allFuncs; //globalFuncs file, for general function (usually regarding the window)
using namespace cardManipulation; //cardCommands file, for card-related functions minus texture loading

//game is blackjack with graphics
//card name scheme is [n/royal][suit]

// Screen format {

//dCARDS
//INFO
//uCARDS
//ACTION BUTTONS

//}

//Algorithm for locating the card texture
//Works by taking the card's string name and checking every entry in of a string deck synced with the texture array
//Whichever indice the card name is located in is the indice of the desired card texture
int getCardTextureLocation(vector <string> deck, string properDeck[], returncard pickcard) {
    int desiredIndice = 0; //The indice location of the texture file needed
    for (int i = 0; i < 52; i++) {
        if (properDeck[i] == pickcard.fixedCardName) {
            break; //We found the right indice
        } else {
            //go again until it's there
            desiredIndice++; //Increment the indice to the next one
        }
    }

    return desiredIndice;
}

//Function to draw everything onscreen, to be used only within the ending switch statement
//This function condensed the script by around 27%
void drawAll(RenderWindow &window, Sprite bg, Sprite &screenDeck, Text &info, Text uscore, Text dscore, Sprite hit, Sprite fold, Sprite help, Sprite muteButton, Sprite cursor, Sprite uCardSlots[5], Sprite dCardSlots[5]) {

    window.clear();
    window.draw(bg); //Draw background first
    window.draw(screenDeck); //Draw little card deck onscreen
    window.draw(info); //draws the tex to the video card
    window.draw(uscore); //draws the score to the video card
    window.draw(dscore); //draws the score to the video card
    window.draw(hit); //draw green button
    window.draw(fold); //draw red button
    window.draw(help); //draw red button
    window.draw(muteButton); //draw mute button
    window.draw(cursor); //draw cursor to mouse
    for (int i = 0; i < 5; i++) {
        window.draw(uCardSlots[i]);
        window.draw(dCardSlots[i]);
    }
    window.display();
}

int main()
{
    RenderWindow window(VideoMode(1200,800),"SFML Works");  //creates a window on the screen that is 800 by 600
    window.setFramerateLimit(60); //sets the game loop to run 60 times per second

    bool gameRestarted = false; //Bool for whether the game was restarted or not, to determine whether textures need to be loaded
    Texture cardTextures[52]; //Card textures arraged in the same order as properDeck, placed pre-gameStart to preserve textures

    //Goto label that restarts the entire game when triggered
    //Goto is like a drug man... it's so powerful because it's so BAD for coding9k
    gameStart:

    vector <string> deck; //vector of 52 cards (Aces are 1, royals 10)
    string properDeck[52]; //Card names arranged coincidently with textures

    //vector <Sprite> cardimage; //vector of cards to be drawn to the screen
    vector <Texture> uCardImages; //vector of card textures for the user
    vector <Texture> dCardImages; //vector of card textures for the dealer

    Clock clickTock; //clock to ensure loop code runs once

    Sound gamblingMusic;
    SoundBuffer gamblingMusicFile;

    int dcards = 0;//the dealer's hand
    int ucards = 0; //user's hand

    int dSlotsUsed = 0;
    int uSlotsUsed = 0;

    Text info; //info label
    Text dscore; //score label for the dealer
    Text uscore; //score label for the user

    string in; //When players go, this stores text output to get displayed

    float mx; //mouse x position
    float my; //mouse y position

    bool turn = true;  //true for user, false for dealer
    bool quit = false; //True if the user quit the game, false if not

    int ending = 0; // non 1-7 ending value means no ending, 0 means game is not over
    bool muted = false; //Music plays at start

    Sprite hit; //green button
    Sprite fold; //red button
    Sprite cursor; //dice cursor
    Sprite bg; //Background
    Sprite screenDeck; //Onscreen deck
    Sprite help; //Onscreen help button (to play intro/tutorial sequence)
    Sprite muteButton; //Image with both the muted and non-muted texture
    Sprite uCardSlots[5]; //Graphical Card slots for user
    Sprite dCardSlots[5]; //Graphical Card slots for dealer

    //confSprite(hit, 0.3, 0.3, 150, 100, 1030, 640);

    for (int i = 0; i < 5; i++) {
        uCardSlots[i].setScale(3,3);
        uCardSlots[i].setOrigin(-115, 0);
        uCardSlots[i].setPosition(100 * i, 640);
    }

    for (int i = 0; i < 5; i++) {
        dCardSlots[i].setScale(3,3);
        dCardSlots[i].setOrigin(-115, 0);
        dCardSlots[i].setPosition(100 * i, 50);
    }

    hit.setScale(0.3,0.3);
    hit.setOrigin(150, 100);
    hit.setPosition(1030, 640);

    hit.setScale(0.3,0.3);
    hit.setOrigin(150, 100);
    hit.setPosition(1030, 640);

    fold.setScale(0.3,0.3);
    fold.setOrigin(150, 100);
    fold.setPosition(50, 640);

    cursor.setScale(0.25,0.25);
    cursor.setOrigin(100, 100);

    bg.setPosition(0,0);
    bg.setOrigin(90,-30);

    screenDeck.setScale(3,3);
    screenDeck.setOrigin(0, 0);
    screenDeck.setPosition(1100, 300);

    help.setScale(1.85,1.85);
    help.setOrigin(0, 0);
    help.setPosition(515, -10);

    muteButton.setScale(0.17,0.17);
    muteButton.setOrigin(0, 0);
    muteButton.setPosition(655, 0);

    Texture red;
    red.loadFromFile("redbutton.png"); //fold button texture
    Texture green;
    green.loadFromFile("greenbutton.png"); //hit button texture
    Texture gamble;
    gamble.loadFromFile("dice.png"); //dice cursor texture
    Texture backdrop;
    backdrop.loadFromFile("backdrop.png"); //dice cursor texture
    Texture deckFace;
    deckFace.loadFromFile("cards/facedown.png"); //dice cursor texture
    Texture helpTex;
    helpTex.loadFromFile("helpButton.png"); //dice cursor texture
    Texture muteTex;
    muteTex.loadFromFile("mute.png"); //dice cursor texture

    hit.setTexture(green); //set green texture
    fold.setTexture(red); //set red texture
    cursor.setTexture(gamble); //set dice texture
    bg.setTexture(backdrop); //set dice texture
    screenDeck.setTexture(deckFace); //set screen deck texture
    help.setTexture(helpTex); //set screen deck texture
    muteButton.setTexture(muteTex); //set Mute buttonn texture to unmuted
    muteButton.setTextureRect(IntRect(0,0,235,170)); //Show unmuted texture

    Font ps2;
    ps2.loadFromFile("PressStart2P.ttf");

    //allFuncs::confLabel(info, ps2, 22, Text::Bold, Color::White, 310, 400);

    info.setFont(ps2);
    info.setCharacterSize(22);
    info.setStyle(Text::Bold);
    info.setFillColor(Color::White);
    info.setPosition(310, 400);

    dscore.setFont(ps2);
    dscore.setCharacterSize(24);
    dscore.setStyle(Text::Bold);
    dscore.setFillColor(Color::White);
    dscore.setPosition(0, 5);

    uscore.setFont(ps2);
    uscore.setCharacterSize(24);
    uscore.setStyle(Text::Bold);
    uscore.setFillColor(Color::White);
    uscore.setPosition(800, 5);

    Text alertText;
    alertText.setFont(ps2);
    alertText.setCharacterSize(30);
    alertText.setFillColor(Color::Magenta);
    alertText.setPosition(100, 280);

    setupDeck(deck); //Setup the numerical and string deck
    if (gameRestarted == false){
        setupCardTextures(deck, cardTextures, info, window, alertText, clickTock, gamblingMusic, gamblingMusicFile); //Load the card textures with output
    } else {
        //Don't reload the textures
    }
    gamblingMusic.play();

    //Copy the deck to an array for texture references
    for (int i = 0; i< 52; i++) {
        properDeck[i] = deck[i];
    }

    while (window.isOpen()&& !Keyboard::isKeyPressed(Keyboard::Escape))     //the main game loop, exits if someone closes the window
    {
        Event event; //creates an event object, events include mouse clicks, mouse movement, keyboard presses, etc..
        while (window.pollEvent(event)) //loop that checks for events
        {
            if (event.type == Event::Closed) //checks if window is closed
                window.close();
        }   //ends the event loop

        mx = (Mouse::getPosition(window).x);
        my = (Mouse::getPosition(window).y);
        cursor.setPosition(mx - 35, my - 50);

        //GAME START
        //------------------------------

        if (turn == true) { //allow button clicking if it's the user's turn

            //if green button clicked, pick up card
            if ((Mouse::isButtonPressed(Mouse::Left)) && (cursor.getGlobalBounds().intersects(hit.getGlobalBounds())) && clickTock.getElapsedTime().asSeconds() > 1) { //if green button clicked
                clickTock.restart();
                cout << "Player hit" << endl;
                usergo(ucards, deck);
                uCardSlots[0 + uSlotsUsed].setTexture(cardTextures[getCardTextureLocation(deck, properDeck, pickcard(deck))]);
                echo("You drew a " + pickcard(deck).fixedCardName + "!", info); //print window text
                cout << "User drew a " << pickcard(deck).fixedCardName << endl;
                //pause(2);

                random_shuffle(deck.begin(), deck.end()); //we have all we need from this draw, reshuffle

                if (ucards > 21) {
                    ending = 1;

                } else if (ucards == 21) {
                    ending = 2;

                } else {
                    //User have more space to hit again
                    uSlotsUsed++; //Card slot was used
                    ending = 0; //Game not over
                }
            }

            //if red button clicked, let dealer play
            if ((Mouse::isButtonPressed(Mouse::Left)) && (cursor.getGlobalBounds().intersects(fold.getGlobalBounds())) && clickTock.getElapsedTime().asSeconds() > 1) { //if red button clicked
                clickTock.restart();
                cout << "Player folded" << endl;
                echo("You folded! Dealer's turn.", info); //print to window
                pause(2);
                turn = false; //start the dealer's turn
                cout << "Dealer's turn" << endl;
            }

            //if red button clicked, let dealer play
            if ((Mouse::isButtonPressed(Mouse::Left)) && (cursor.getGlobalBounds().intersects(help.getGlobalBounds())) && clickTock.getElapsedTime().asSeconds() > 1) { //if red button clicked
                clickTock.restart();
                cout << "Help button clicked, playing intro function" << endl;
                intro(info, window, clickTock, alertText); //Run the help spiel
            }

            if ((Mouse::isButtonPressed(Mouse::Left)) && (cursor.getGlobalBounds().intersects(muteButton.getGlobalBounds())) && clickTock.getElapsedTime().asSeconds() > 1) { //if red button clicked
                clickTock.restart();
                cout << "Mute button clicked, toggling audio" << endl;

                if (muted == false) {
                    gamblingMusic.pause(); //Pause instead of stop to preserve audio when it was paused
                    muteButton.setTextureRect(IntRect(292,0,235,170)); //Show muted texture
                } else {
                    gamblingMusic.play();
                    muteButton.setTextureRect(IntRect(0,0,235,170)); //Show unmuted texture
                }; muted = !muted; //Flip bool, muted is now not muted and vice versa
            }

        } else if (turn == false) {

            //Dealer's turn
            //This moves slower because it's automated
            echo("It is now the dealer's turn.", info); //print window text
            //drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, cursor, uCardSlots, dCardSlots);
            pause(2);
            dealergo(dcards, deck); //dealer takes a card
            dCardSlots[0 + dSlotsUsed].setTexture(cardTextures[getCardTextureLocation(deck, properDeck, pickcard(deck))]);
            echo("The dealer drew a " + pickcard(deck).fixedCardName + "!", info); //print window text
            cout << "Dealer drew a " << pickcard(deck).fixedCardName << endl;
            //pause(2);
            random_shuffle(deck.begin(), deck.end()); //we have all we need from this draw, reshuffle
            if (dcards > 21) {
                    ending = 3;
                } else if (dcards == 21) {

                    ending = 4;

                } else if (dcards >= 16) { //if the dealer has 16 or more cards, they can't hit

                    cout << "Dealer card value is over 16. End score: " << ucards;
                    echo("The dealer's score is >= 16.", info); //print window text
                    drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                    pause(2);

                    if (dcards > ucards) {
                        ending = 5;

                    } else if (dcards < ucards) {
                        ending = 6;

                    } else {
                        ending = 7;
                    }

                } else {
                    //Dealer has more space to hit again
                    dSlotsUsed++;
                    ending = 0;
                }

        } else {
            cout << "I don't even know man..." << endl;
        }


        uscore.setString("User's score: "+ to_string(ucards)); //ocnstantly update user's score onscreen
        dscore.setString("Dealer's score: "+ to_string(dcards)); //ocnstantly update dealer's score onscreen

        //If it was set to an ending, play it, but if not then draw like usual
        switch (ending) {
            //Player bust
            case 1:
                cout << "Player bust" << endl;
                echo("You bust!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                echo("The dealer wins!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                quit = true; //close program
            break;

            //Player 21
            case 2:
                cout << "Player 21" << endl;
                echo("You got 21!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                echo("You Win!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                quit = true;
            break;

            //Dealer bust
            case 3:
                cout << "Dealer bust" << endl;
                echo("The dealer bust!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                echo("You win!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                quit = true; //close program
            break;

            //Dealer 21
            case 4:
                cout << "Dealer 21" << endl;
                echo("The dealer got 21!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                echo("The dealer wins!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                quit = true;
            break;

            //--------Endings where dealer hand >= 16

            //Dealer hand > user hand -- dealer win
            case 5:
                cout << "Dealer win" << endl;
                echo("The dealer wins!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                cout << info.getString().toAnsiString() << endl;
                pause(2);
                quit = true;
            break;

            //Dealer hand < user hand -- user win
            case 6:
                cout << "Player win" << endl;
                echo("You have a higher score.", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                cout << info.getString().toAnsiString() << endl;
                pause(2);
                echo("You win!", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                pause(2);
                quit = true;
            break;

            //Dealer hand = user hand -- game tie
            case 7:
                cout << "Tie" << endl;
                echo("You tied with the dealer.", info); //print window text
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
                cout << info.getString().toAnsiString() << endl;
                pause(2);
                quit = true;
            break;

            //Custom input, should not be used. Added soley for scalability reasons "Just in case"
            case 999:
                cout << "How did we get here?" << endl;
            break;

            //Game is not over, draw like usual
            default:
                drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
            break;

        }

        //Was a quit sequence called? If not, ignore. If so, close program
        if (quit == true) {
            gamblingMusic.stop(); //Terminate audio for sake of proper cleanup
            cout << "Game restarted" << endl;
            echo("Game resetting...", info); //print window text
            drawAll(window, bg, screenDeck, info, uscore, dscore, hit, fold, help, muteButton, cursor, uCardSlots, dCardSlots);
            pause(2);
            gameRestarted=true; //State that the game was restarted (textures don't need to be reloaded)
            goto gameStart; //Literally just restart the game
            //exit(0);
        } else {
            //continue
            gameRestarted = false;
        }

    }   //ends the game loop

    return 0;
}

