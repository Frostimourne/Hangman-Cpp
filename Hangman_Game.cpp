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

    private:

    vector<string> wordList;
    bool guessedLetters[25];
    char gallowsMap[8][22];
    string secretWord;
    char validGuess;
    bool gameOver;
    bool win;
    int numberOfWrongGuesses;
    int ASCII_OFFSET;

    //Stats - not necessary for game itself, but will be output to a file for the user to assess their play
    int guessFrequency[25];
    int wins;
    int losses;
    
    //Outputs statistics to text file
    //Overwrites previous session stats
    void outputSessionStats()
    {
        fstream Output;
        Output.open("SessionStats.txt", fstream::out, fstream::trunc);

        if(Output.is_open())
        {
            int commonGuessCount = 0;
            char commonGuessChar = '?';
            for (int i = 0;i < 26; i++)
            {
                if(guessFrequency[i] > commonGuessCount) 
                {
                    commonGuessCount = guessFrequency[i];
                    commonGuessChar = char(i + ASCII_OFFSET);
                }
            }

            Output << "Session statistics:\n\n"
            << "Wins: " << wins << endl
            << "Losses: " << losses << endl
            << "Win percentage: " << int((float(wins) / float(wins + losses))*100) << "%" << endl
            << "Most common guess: " << commonGuessChar << " (" << commonGuessCount << ")" << endl;

        } else {
            cout << "error opening output file.\n\n";
        }
    }

    void initializeGallows()
    {
        //A bit clunky, but it gets the job done.  Initializes the GallowsMap array as "empty" of guesses
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
        //Update guess stats
        guessFrequency[int(validGuess) - ASCII_OFFSET]++;

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

        //Add win to win counter and inform player of victory
        if(win) 
        {
            cout << "You guessed the secret word!  Great job!\n\n";
            wins++;
        }
    }


    //Loops guesses until either victory or loss
    void GuessingLoop()
    {
        while(!gameOver)
        {
            printGallows();
            printSecretWord();
            checkForVictory();

            //If too many wrong guesses, game is over, and losses +1
            if(numberOfWrongGuesses > 6)
            {
                cout << "Oh no!  The man is hung!  You lose! The secret word was: " << secretWord << "\n\n";
                gameOver = true;
                losses++;

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


    public:

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

        //Initialize stats
        wins = 0;
        losses = 0;
        for(int i=0; i<26; i++)
        {
            guessFrequency[i] = 0;
        }

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
        outputSessionStats();
    }


};



int main()
{
    HangmanGame HG1;
    HG1.BeginGame();

    cout << "Thanks for playing!\n";
    return 0;
}