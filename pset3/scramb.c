/***************************************************************************
 * scramble.c
 *
 * Problem Set 3
 *
 * Implements Scramble with CS50.
 *
 * Usage: scramble [#]
 *
 * where # is an optional grid number.
 ***************************************************************************/
 
#include "cs50.h"
#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// duration of a game in seconds
#define DURATION 30

// grid's dimensions
#define DIMENSION 10

// maximum number of words in any dictionary
#define WORDS 172806

// maximum number of letters in any word
#define LETTERS 29

// default dictionary
// http://www.becomeawordgameexpert.com/wordlists.htm
#define DICTIONARY "words"

// for logging
FILE* logfile;

// grid
char grid[DIMENSION][DIMENSION];

// flags with which we can mark grid's letters while searching for words
bool marks[DIMENSION][DIMENSION];

// defines a word as having an array of letters plus a flag
// indicating whether word has been found on grid by the user.
// also keeps track of which words are on the grid, including
// those the user has not yet found
typedef struct
{
    bool found;
    bool onGrid;
    int length;
    char letters[LETTERS + 1];
}
word;

// defines a dictionary as having a size and an array of words
struct
{
    int size;
    word words[WORDS];
}
dictionary;

// prototypes
void clear(void);
bool crawl(string letters, int x, int y);
void draw(void);
bool find(string s);
void initialize(void);
bool load(string s);
word* lookup(string s);
void find_all(void);
void print_inspiration(void);
int get_score(word* w);

// This is Scramble.
int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc > 2)
    {
        printf("Usage: %s [#]\n", basename(argv[0]));
        return 1;
    }

    // seed pseudorandom number generator
    if (argc == 2)
    {
        int seed = atoi(argv[1]);
        if (seed <= 0)
        {
            printf("Invalid grid.\n");
            return 1;
        }
        srand(seed);
    }
    else
        srand(time(NULL));

    // determine path to dictionary
    string directory = dirname(argv[0]);
    char path[strlen(directory) + 1 + strlen(DICTIONARY) + 1];
    sprintf(path, "%s/%s", directory, DICTIONARY);

    // load dictionary
    if (!load(path))
    {
        printf("Could not open dictionary.\n");
        return 1;
    }

    // initialize the grid
    initialize();
   
    // crawl around for all possible correct answers on the grid
    find_all();
   
    // initialize user's score
    int score = 0;

    // calculate time of game's end
    int end = time(NULL) + DURATION;

    // open log
    logfile = fopen("log.txt", "w");
    if (logfile == NULL)
        return 1;
 
    // last-entered word
    word* w = NULL;
 
    // accept words until timer expires
    while (true) {
   
        clear();
        draw();

        // log board
        int row;
        for (row = 0; row < DIMENSION; row++) {
            int col;
            for (col = 0; col < DIMENSION; col++)
                fprintf(logfile, "%c", grid[row][col]);
            fprintf(logfile, "\n");
        }

        // get current time
        int now = time(NULL);

        // report score
        printf("Score: %d\n", score);
        fprintf(logfile, "%d\n", score);

        // check for game's end
        if (now >= end) {
            printf("Time:  %d\n\n", 0);
            break;
        }

        // report time remaining
        printf("Time:  %d\n\n", end - now);
       
        // if the last-entered word entered is "inspiration", give some hints
        if (NULL != w && 0 == strcmp("inspiration", w->letters)) {
            print_inspiration();
        }
       
        // prompt for word
        printf("> ");
        string input = GetString();

        // quit playing if user hits ctrl-d
        if (input == NULL)
            break;

        // log word
        fprintf(logfile, "%s\n", input);
       
        // look for word on grid and in dictionary
        w = lookup(input);
        if (NULL != w && w->onGrid && !w->found) {
            score += get_score(w);
        }
    }

    // close log
    fclose(logfile);

    return 0;
}

/**
 * Anytime  the user  types INSPIRATION, up  to  three words are displayed,
 * one of length 5 (if any), one of length 4 (if any), and one of length 3
 *(if any), all of which are in the dictionary and in the grid but not
 * yet found.
 */
void print_inspiration(void) {

    bool hint5displayed = false;
    bool hint4displayed = false;
    bool hint3displayed = false;
   
    printf("psst... what about: ");
   
    int wordNum;
    word* w;
    for (wordNum = 0; wordNum < WORDS; wordNum++) {
        w = &dictionary.words[wordNum];
   
        // don't give useless inspiration
        if (!w->onGrid || w->found) {
            continue;
        }
   
        // display hint for each word length only once
       
        if (!hint5displayed && 5 == w->length) {
            printf("%s ", w->letters);
            hint5displayed = true;
        }
       
        if (!hint4displayed && 4 == w->length) {
            printf("%s ", w->letters);
            hint4displayed = true;
        }
       
        if (!hint3displayed && 3 == w->length) {
            printf("%s ", w->letters);
            hint3displayed = true;
        }
    }
    printf("\n\n");
   
    free(w);
}


/**
 * Compute the score for a given word.
 */
int get_score(word* w) {

    int score = 0;
   
    int i;
    for (i = 0; i < strlen(w->letters); i++) {
        switch (w->letters[i]) {
            case 'A':
            case 'E':
            case 'R':
            case 'S':
            case 'T':
            case 'O':
            case 'I':
                score += 1;
                break;
            case 'L':
            case 'U':
            case 'N':
            case 'D':
                score += 2;
                break;
            case 'Y':
            case 'H':
            case 'G':
                score += 3;
                break;
            case 'W':
            case 'M':
            case 'B':
            case 'C':
            case 'P':
            case 'F':
                score += 4;
                break;
            case 'V':
            case 'K':
                score += 5;
                break;
            case 'X':
                score += 8;
                break;
            case 'Z':
            case 'J':
            case 'Q':
                score += 10;
                break;
        }
    }
   
    return score;
}

/**
 * Looks up string, s, in dictionary.  Iff found, returns an instance of the
 * word struct with the same letters as the entered string. If not found,
 * returns NULL.
 */
word* lookup(string s) {

    // Return value
    word* w = NULL;
   
    // check each word
    int wordNum;
    for (wordNum = 0; wordNum < WORDS; wordNum++) {
       
        // is this the word?
        if (0 == strcmp(dictionary.words[wordNum].letters, s)) {
            w = &dictionary.words[wordNum];
            break;
        }
    }
   
    return w;
}

/**
 * Search the grid for every word in the dictionary to keep track of
 * all the possible correct answers.
 */
void find_all(void) {
    int wordNum;
    word* w;
    for (wordNum = 0; wordNum < WORDS; wordNum++) {
        w = &dictionary.words[wordNum];
        w->onGrid = find(w->letters);
    }
}

/**
 * Clears screen.
 */
void clear()
{
    // linux
    //    printf("\033[2J");
    //    printf("\033[%d;%dH", 0, 0);

    // windows
    system("cls");
}

/**
 * Crawls grid recursively for letters starting at grid[x][y].
 * Returns true iff all letters are found.
 */
bool crawl(string letters, int x, int y)
{
    // if out of letters, then we must've found them all!
    if (strlen(letters) == 0)
        return true;

    // don't fall off the grid!
    if (x < 0 || x >= DIMENSION)
        return false;
    if (y < 0 || y >= DIMENSION)
        return false;

    // been here before!
    if (marks[x][y])
        return false;

    // check grid[x][y] for current letter
    if (grid[x][y] != letters[0])
        return false;

    // mark location
    marks[x][y] = true;

    // look left and right for next letter
    int i;
    for (i = -1; i <= 1; i++)
    {
        // look down and up for next letter
        int j;
        for (j = -1; j <= 1; j++)
        {
            // check grid[x + i][y + j] for next letter
            if (crawl(&letters[1], x + i, y + j))
                return true;
        }
    }

    // unmark location
    marks[x][y] = false;

    // fail
    return false;
}

/**
 * Prints the grid in its current state.
 */
void draw(void)
{
    // give it some padding at the top
    printf("\n\n");
       
    int i;
    int j;
    for (i = 0; i < DIMENSION; i++) {
        printf("    ");
        for (j = 0; j < DIMENSION; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

/**
 * Returns true iff word, s, is found in grid.
 */
bool find(string s)
{
    // word must be at least 2 characters in length
    if (strlen(s) < 2)
        return false;

    // search grid for word
    int row;
    for (row = 0; row < DIMENSION; row++) {
        int col;
        for (col = 0; col < DIMENSION; col++) {
            // reset marks
            int i;
            for (i = 0; i < DIMENSION; i++) {
                int j;
                for (j = 0; j < DIMENSION; j++) {
                    marks[i][j] = false;
                }
            }
            // search for word starting at grid[i][j]
            if (crawl(s, row, col))
                return true;
        }
    }
    return false;
}

/**
 * Initializes grid with letters.
 */
void initialize(void)
{
    // http://en.wikipedia.org/wiki/Letter_frequency
    float frequencies[] = {
     8.167,  // a
     1.492,  // b
     2.782,  // c
     4.253,  // d
     12.702, // e
     2.228,  // f
     2.015,  // g
     6.094,  // h
     6.966,  // i
     0.153,  // j
     0.747,  // k
     4.025,  // l
     2.406,  // m
     6.749,  // n
     7.507,  // o
     1.929,  // p
     0.095,  // q
     5.987,  // r
     6.327,  // s
     9.056,  // t
     2.758,  // u
     1.037,  // v
     2.365,  // w
     0.150,  // x
     1.974,  // y
     0.074   // z
    };
    int n = sizeof(frequencies) / sizeof(float);

    // iterate over grid
    int row;
    for (row = 0; row < DIMENSION; row++)
    {
        int col;
        for (col = 0; col < DIMENSION; col++)
        {  
            // generate pseudorandom double in [0, 1]
            double d = rand() / (double) RAND_MAX;

            // map d onto range of frequencies
            int k;
            for (k = 0; k < n; k++)
            {
                d -= frequencies[k] / 100;
                if (d < 0.0 || k == n - 1)
                {
                    grid[row][col] = 'A' + k;
                    break;
                }
            }
        }
    }
}

/**
 * Loads words from dictionary with given filename, s, into a global array.
 */
bool load(string s)
{
    // open dictionary
    FILE* file = fopen(s, "r");
    if (file == NULL)
        return false;

    // initialize dictionary's size
    dictionary.size = 0;

    // load words from dictionary
    char buffer[LETTERS + 2];
    while (fgets(buffer, LETTERS + 2, file))
    {
        // overwrite \n with \0
        buffer[strlen(buffer) - 1] = '\0';

        // capitalize word
        int i;
        int n;
        for (i = 0, n = strlen(buffer); i < n; i++)
            buffer[i] = toupper(buffer[i]);

        // copy word into dictionary
        dictionary.words[dictionary.size].found = false;
        dictionary.words[dictionary.size].onGrid = false;
        dictionary.words[dictionary.size].length = strlen(buffer);
        strncpy(dictionary.words[dictionary.size].letters, buffer, LETTERS + 1);
        dictionary.size++;
    }

    // success!
    return true;
}


