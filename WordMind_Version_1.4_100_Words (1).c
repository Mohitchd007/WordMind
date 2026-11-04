#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define WORD_LENGTH 5
#define MAX_INPUT 100
#define MAX_ATTEMPTS 10
#define WORDS_PER_LEVEL 25

// Function declarations
int getChoice(int min, int max);
void getWord(const char *message, char word[]);
int sameLetter(char a, char b, int caseSensitive);
void checkGuess(char target[], char guess[],
                int caseSensitive, char feedback[]);
void displayFeedback(char guess[], char feedback[], int impossible);
void displayHistory(char guesses[][MAX_INPUT],
                    char feedbacks[][WORD_LENGTH],
                    int count, int impossible);
void showInstructions(int caseSensitive);
void playGame(int mode, int difficulty);
const char *getRandomWord(int difficulty);

// 25 familiar five-letter words
const char *easyWords[WORDS_PER_LEVEL] = {
    "APPLE", "BREAD", "CHAIR", "TABLE", "WATER", "HOUSE", "MOUSE", "TIGER", "PLANT", "GRAPE", "SMILE", "LIGHT", "NIGHT", "DANCE", "EARTH", "CLOUD", "RIVER", "STONE", "WORLD", "FLAME", "BEACH", "BRAIN", "BRUSH", "CANDY", "CARRY"
};

// 25 moderately challenging five-letter words
const char *mediumWords[WORDS_PER_LEVEL] = {
    "ABOVE", "ADMIT", "ADOPT", "AGENT", "ALARM", "ALBUM", "ALERT", "ALTER", "AMBER", "ANGLE", "ANKLE", "APRON", "ARENA", "ARGUE", "ARROW", "ASIDE", "AUDIO", "AVOID", "AWAKE", "AWARD", "BADGE", "BAKER", "BASIC", "BATCH", "BEAST"
};

// 25 difficult five-letter words
const char *hardWords[WORDS_PER_LEVEL] = {
    "ABYSS", "ACORN", "ALLOY", "AMEND", "AMPLY", "APPLY", "AROMA", "ASHEN", "ATONE", "BANJO", "BARGE", "BASIL", "BATON", "BEGUN", "BISON", "BLEAK", "BLISS", "BOGUS", "BRAWL", "BRINE", "BROOD", "BROOM", "CAMEO", "CEDAR", "CHALK"
};

// 25 uncommon and advanced five-letter words
const char *impossibleWords[WORDS_PER_LEVEL] = {
    "ABATE", "ABHOR", "ACRID", "ADAGE", "ADORN", "AGAPE", "AGLOW", "ALOOF", "AMBLE", "ANVIL", "APNEA", "ARDOR", "ASCOT", "AUGUR", "AURAL", "AXIOM", "BAIZE", "BAYOU", "BEFIT", "BILGE", "BLURB", "BOUGH", "CAIRN", "CANNY", "CHASM"
};

// Choose a random word from the selected difficulty's word bank
const char *getRandomWord(int difficulty)
{
    switch (difficulty)
    {
        case 1:
            return easyWords[rand() % WORDS_PER_LEVEL];
        case 2:
            return mediumWords[rand() % WORDS_PER_LEVEL];
        case 3:
            return hardWords[rand() % WORDS_PER_LEVEL];
        case 4:
            return impossibleWords[rand() % WORDS_PER_LEVEL];
        default:
            return easyWords[rand() % WORDS_PER_LEVEL];
    }
}

// Main function
int main(void)
{
    int mode, difficulty, again;

    srand((unsigned int)time(NULL));

    printf("\n====================================\n");
    printf("          W O R D M I N D\n");
    printf("             VERSION 1.4\n");
    printf("====================================\n");

    do
    {
        printf("\nSELECT GAME MODE\n");
        printf("1. Random Word Challenge\n");
        printf("2. Two-Player Challenge\n");
        printf("3. Exit\n");

        printf("\nEnter your choice: ");
        mode = getChoice(1, 3);

        if (mode == 3)
            break;

        printf("\nSELECT DIFFICULTY\n");
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("4. Impossible\n");

        printf("\nEnter difficulty: ");
        difficulty = getChoice(1, 4);

        playGame(mode, difficulty);

        printf("\nWould you like to play again?\n");
        printf("1. Yes\n");
        printf("2. No\n");
        printf("Enter your choice: ");

        again = getChoice(1, 2);

    } while (again == 1);

    printf("\nThank you for playing WORDMIND!\n");
    return 0;
}

// Read and validate a menu choice
int getChoice(int min, int max)
{
    char input[MAX_INPUT];
    char *end;
    long choice;

    while (1)
    {
        if (fgets(input, sizeof(input), stdin) == NULL)
            exit(0);

        if (strchr(input, '\n') == NULL && !feof(stdin))
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            printf("Invalid choice. Enter again: ");
            continue;
        }

        choice = strtol(input, &end, 10);

        while (isspace((unsigned char)*end))
            end++;

        if (end != input && *end == '\0' &&
            choice >= min && choice <= max)
        {
            return (int)choice;
        }

        printf("Invalid choice. Enter again: ");
    }
}

// Get a valid five-letter word
void getWord(const char *message, char word[])
{
    while (1)
    {
        printf("%s", message);

        if (fgets(word, MAX_INPUT, stdin) == NULL)
            exit(0);

        if (strchr(word, '\n') == NULL && !feof(stdin))
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            printf("Input is too long. Try again.\n");
            continue;
        }

        word[strcspn(word, "\n")] = '\0';
        word[strcspn(word, "\r")] = '\0';

        if (strlen(word) != WORD_LENGTH)
        {
            printf("Please enter exactly 5 letters.\n");
            continue;
        }

        int valid = 1;

        for (int i = 0; i < WORD_LENGTH; i++)
        {
            if (!isalpha((unsigned char)word[i]))
            {
                valid = 0;
                break;
            }
        }

        if (valid)
            return;

        printf("Only alphabetic letters are allowed.\n");
    }
}

// Compare letters according to difficulty
int sameLetter(char a, char b, int caseSensitive)
{
    if (caseSensitive)
        return a == b;

    return tolower((unsigned char)a) ==
           tolower((unsigned char)b);
}

// Compare the guess with the hidden word
void checkGuess(char target[], char guess[],
                int caseSensitive, char feedback[])
{
    int used[WORD_LENGTH] = {0};

    // First pass: identify correct letters in correct positions
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (sameLetter(guess[i], target[i], caseSensitive))
        {
            feedback[i] = 'G';
            used[i] = 1;
        }
        else
        {
            feedback[i] = 'R';
        }
    }

    // Second pass: identify correct letters in wrong positions
    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (feedback[i] == 'G')
            continue;

        for (int j = 0; j < WORD_LENGTH; j++)
        {
            if (!used[j] &&
                sameLetter(guess[i], target[j], caseSensitive))
            {
                feedback[i] = 'Y';
                used[j] = 1;
                break;
            }
        }
    }
}

// Display feedback for one guess
void displayFeedback(char guess[], char feedback[], int impossible)
{
    int green = 0, yellow = 0, red = 0;

    for (int i = 0; i < WORD_LENGTH; i++)
    {
        if (feedback[i] == 'G')
            green++;
        else if (feedback[i] == 'Y')
            yellow++;
        else
            red++;
    }

    if (impossible)
    {
        printf("GREEN 🟩: %d | YELLOW 🟨: %d | RED 🟥: %d\n",
               green, yellow, red);
    }
    else
    {
        for (int i = 0; i < WORD_LENGTH; i++)
        {
            if (feedback[i] == 'G')
                printf("🟩 ");
            else if (feedback[i] == 'Y')
                printf("🟨 ");
            else
                printf("🟥 ");
        }

        printf("\n");
    }
}

// Display all previous guesses
void displayHistory(char guesses[][MAX_INPUT],
                    char feedbacks[][WORD_LENGTH],
                    int count, int impossible)
{
    printf("\n====================================\n");
    printf("              GUESS HISTORY\n");
    printf("====================================\n");

    for (int i = 0; i < count; i++)
    {
        printf("\nAttempt %d: %s\n", i + 1, guesses[i]);
        displayFeedback(guesses[i], feedbacks[i], impossible);
    }

    printf("\n====================================\n");
}

// Display game instructions
void showInstructions(int caseSensitive)
{
    printf("\n====================================\n");
    printf("             HOW TO PLAY\n");
    printf("====================================\n");

    printf("1. The hidden word contains exactly 5 letters.\n");
    printf("2. Enter a five-letter word for every guess.\n");
    printf("3. Only alphabetic characters are allowed.\n");

    if (caseSensitive)
    {
        printf("4. Capitalization matters in this mode.\n");
        printf("   Enter the letters in the exact case.\n");
        printf("   Random words are displayed in UPPERCASE.\n");
    }
    else
    {
        printf("4. Capitalization does not matter.\n");
    }

    printf("\nCOLOUR KEY:\n");
    printf("GREEN  🟩 = Correct letter, correct position\n");
    printf("YELLOW 🟨 = Correct letter, wrong position\n");
    printf("RED    🟥 = Letter not present in the word\n");

    printf("\nTry to guess the hidden word before\n");
    printf("running out of attempts!\n");
}

// Main game function
void playGame(int mode, int difficulty)
{
    char target[MAX_INPUT];
    char guess[MAX_INPUT];

    // Store previous guesses and their feedback
    char guesses[MAX_ATTEMPTS][MAX_INPUT];
    char feedbacks[MAX_ATTEMPTS][WORD_LENGTH];

    int attempts[] = {0, 10, 7, 6, 5};

    int caseSensitive = (difficulty >= 3);
    int impossible = (difficulty == 4);

    const char *levels[] = {
        "", "EASY", "MEDIUM", "HARD", "IMPOSSIBLE"
    };

    // Select the hidden word
    if (mode == 1)
    {
        strcpy(target, getRandomWord(difficulty));
    }
    else
    {
        printf("\nPLAYER 1: Enter the secret word.\n");
        getWord("Secret word: ", target);

        printf("\nPass the device to PLAYER 2.\n");
        printf("PLAYER 2: Do not look at the previous input!\n");
    }

    // Display game information
    printf("\n====================================\n");
    printf("          %s MODE\n", levels[difficulty]);
    printf("====================================\n");

    showInstructions(caseSensitive);

    printf("\nAttempts available: %d\n", attempts[difficulty]);

    // Guessing loop
    for (int turn = 1; turn <= attempts[difficulty]; turn++)
    {
        printf("\nAttempt %d of %d\n", turn, attempts[difficulty]);
        getWord("Enter your guess: ", guess);

        // Save the current guess
        strcpy(guesses[turn - 1], guess);

        // Calculate feedback
        checkGuess(target, guess, caseSensitive,
                   feedbacks[turn - 1]);

        // Display complete guess history
        displayHistory(guesses, feedbacks, turn, impossible);

        // Check whether the player has won
        int won = 1;

        for (int i = 0; i < WORD_LENGTH; i++)
        {
            if (feedbacks[turn - 1][i] != 'G')
            {
                won = 0;
                break;
            }
        }

        if (won)
        {
            printf("\nCONGRATULATIONS! YOU WON!\n");
            printf("You guessed the word in %d attempts.\n", turn);
            return;
        }

        printf("\nAttempts remaining: %d\n",
               attempts[difficulty] - turn);
    }

    // Display result if all attempts are used
    printf("\nGAME OVER! You ran out of attempts.\n");
    printf("The correct word was: %s\n", target);
}
