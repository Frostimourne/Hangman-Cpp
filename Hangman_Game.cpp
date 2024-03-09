#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;



class HangmanGame
{

public:

    vector<string> wordList;
    bool guessedLetters[25];
    char gallowsMap[8][22];
    string secretWord;
    char validGuess;
    bool gameOver;
    bool win;
    int numberOfWrongGuesses;
    int ASCII_OFFSET;

    void initializeGallows()
    {
        /*
        //Initialize Gallows - There's definitely a better way to do this
        char longFormGallows[] = "  -+-----+    x~~~~~~~x |          |       |  |          |       |  |          |       |  |          |       |  |          x~~~~~~~x==^=======            ";
        for(int i=0; i < 7; i++)
        {
            for(int j=0; j < 22; j++)
            {
                gallowsMap[i][j] = longFormGallows[(i*22) + j];
            }
        }
        */
        strcpy(gallowsMap[0], "                      ");
        strcpy(gallowsMap[1], "  +------+   x~~~~~~~x");
        strcpy(gallowsMap[2], "  |          |       |");
        strcpy(gallowsMap[3], "  |          |       |");
        strcpy(gallowsMap[4], "  |          |       |");
        strcpy(gallowsMap[5], "  |          |       |");
        strcpy(gallowsMap[6], "  |          x~~~~~~~x");
        strcpy(gallowsMap[7], "==^=======            ");


    }

    void initializeGuessedLetters()
    {
        for(int i = 0; i < 26; i++)
        {
            guessedLetters[i] = false;
        }
    }

    //Displays current game state in terminal window
    void printGallows()
    {
        for (int i=0;i<8;i++)
        {
            for(int j=0;j<22;j++)
            {
                if(gallowsMap[i][j] != '\0')
                {
                    cout << gallowsMap[i][j];
                } else {
                    break;
                }
            }
            cout << endl;
        }

        //Endline buffer for secretWord
        cout << "\n\n";
    }

    //Displays current correct guesses and secret word length
    void printSecretWord()
    {
        //Small buffer
        cout << "  ";

        //For each letter in secretWord, check if letter has been guessed. If not, print underscore
        for(int i=0; i< secretWord.length(); i++)
        {
            if(!guessedLetters[int(char(secretWord[i])) - ASCII_OFFSET])
            {
                cout << "_ ";

            } else {
                cout << secretWord[i] << " ";
            }

        }
        cout << "\n\n\n";
    }

    //Validate guess input
    void getValidGuess() 
    {
        char guess = NULL;
        bool isValid = false;

        //Keep prompting the user until a valid guess is entered
        while (!isValid) 
        {
            cout << "Time to guess a letter!  Enter your guess: ";
            cin >> guess;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Clear input buffer

                //if input is a letter, isValid will be set to True
                isValid = isalpha(guess);

                if(!isValid)
                {
                    cout << "Invalid input. Please try again.\n\n";
                } 
        
        }
    //Make guess uppercase for consistency
    validGuess = toupper(guess);
    }

    void checkGuess()
    {
        cout << "\n\n";

        if (guessedLetters[int(validGuess) - ASCII_OFFSET]) //Letter already guessed
        {
            cout << "You've already guessed that!  Please try again\n\n";

        } else if (secretWord.find(validGuess) != string::npos) // Guess is correct
        {
            cout << "Yes! " << validGuess << " is in the secret word! \n\n";
            guessedLetters[int(validGuess) - ASCII_OFFSET] = true;
            updateGallowsMap(false);

        } else //Guess is incorrect
        {
            cout << "Oops! Sorry, but " << validGuess << " is not in the secret word\n\n";
            guessedLetters[int(validGuess) - ASCII_OFFSET] = true;
            numberOfWrongGuesses++;
            updateGallowsMap(true);
        }
    }

    void updateGallowsMap(bool guessedWrong)
    {
        if(guessedWrong)
        {
            switch (numberOfWrongGuesses)
            {
                case 1:
                    gallowsMap[2][9] = '|';
                    break;
                case 2:
                    gallowsMap[3][9] = 'O';
                    break;
                case 3:
                    gallowsMap[4][9] = '|';
                    break;
                case 4:
                    gallowsMap[4][8] = '/';
                    break;
                case 5:
                    gallowsMap[4][10] = '\\';
                    break;
                case 6:
                    gallowsMap[5][8] = '/';
                    break;
                case 7:
                    gallowsMap[5][10] = '\\';
                    break;
                default:
                    break;
            }
        }

        int rowOffset = 2;
        int columnOffset = 14;

        int changeRow = rowOffset + ((int(validGuess) - ASCII_OFFSET) / 7);
        int changeCol = columnOffset + ((int(validGuess) - ASCII_OFFSET) % 7);

        gallowsMap[changeRow][changeCol] = validGuess;

    }

    //If all letters in secretWord have been guessed, set win to true so game can end.
    void checkForVictory()
    {
        bool victory = true;
        for(int i=0; i<secretWord.length(); i++)
        {
            if(!guessedLetters[int(char(secretWord[i])) - ASCII_OFFSET])
            victory = false;
        }
        win = victory;
        gameOver = win;

        if(win) {cout << "You guessed the secret word!  Great job!\n\n";}
    }


    //Loops guesses until either victory or loss
    void GuessingLoop()
    {
        while(!gameOver)
        {
            printGallows();
            printSecretWord();
            checkForVictory();

            if(numberOfWrongGuesses > 6)
            {
                cout << "Oh no!  The man is hung!  You lose! The secret word was: " << secretWord << "\n\n";
                gameOver = true;
            } else if (!win)
            {
                getValidGuess();
                checkGuess();

            }
        }
        
    }

    void ChooseWord()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        secretWord = wordList[rand() % wordList.size()];
    }


    //Word List Constructor
    HangmanGame()
    {   
        //Open Text File with words
        fstream wordListFile;
        wordListFile.open("WordList.txt", fstream::in);

        //Initialize ASCII_OFFSET
        ASCII_OFFSET = int('A');

        initializeGallows();
        initializeGuessedLetters();

        //Populate wordList with words from text file wordListFile
        if(wordListFile.is_open())
        {
            string word;
            while (wordListFile >> word)
            {
                transform(word.begin(), word.end(), word.begin(), ::toupper);
                wordList.push_back(word);
            }
        }
        
        //Game Over starts false
        gameOver = false;

    }


    //Initialize incorrect guess counter, choose word if needed, then begin guessing loop 
    void BeginGame()
    {
        while(!gameOver)
        {
            ChooseWord();
            numberOfWrongGuesses = 0;
            win = false;
            initializeGallows();
            initializeGuessedLetters();

            //Stay in GuessingLoop until either number of incorrect guesses gets too large or word is guessed
            GuessingLoop();

            //Ask user if they would like to play again
            char decision = NULL;
            cout << "That was fun!  Would you like to play again? (y/n): ";
            cin >> decision;

            //If yes, reset gameOver to false because the overall game is not over yet
            if(decision == 'y' || decision == 'Y')
            {
                gameOver = false;
            }
        }
    }


};



int main()
{


    HangmanGame HG1;

    HG1.BeginGame();

    cout << "Thanks for playing!\n";

    return 0;
}