/*
 * File   : crack.c
 * Author : DM
 * License: BSD 3-clause license http://gotfu.wordpress.com/bsd-3-clause-license/
 * Copyright (c) 2011 Got-fu? http://gotfu.wordpress.com/
 */
 
#define _XOPEN_SOURCE 500
#include <crypt.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
/*
#include <cs50.h>
*/
 
/* Constants */
#define MAXPASSWORDLENGHT 8
#define DES56KEYLENGTH 13
#define DEFAULT_SUBSETS "aA0#"
#define MAXCHARSETLENGTH 26 + 26 + 10 + 32 + 1 /* note: +1 is inclued here! */
 
/* My Types */
typedef struct STORAGE {
    /* count the number of \0s in the stored dictionary
     * (remember to include the last one just in case there's no \n at the last word) */
    unsigned int wordcount;
    unsigned int size; // size in bytes, aka size of array.
    char *s; // the entire words file
} store_t;
typedef uint8_t length_t;
 
/* Function Prototypes */
int iStart(bool indexExhausted);
void makeCharSet(char *subSets, int *charSetSize, char *charSet);
void makeCharSetError();
void usage(void);
bool bruteForce(char *desString, char salt[], char charSet[], int charSetSize, int bruteUpToX);
bool load_storage(FILE *dfp);
bool load(const char *dict);
void unload();
bool encstreq(const char *str1, const char *str2);
void tryPassword(char *passToTry, char *desString, char salt[]);
 
/* Globals */
int seconds;
char *l33t = "4bcd3fgh1jklmn0pqrstuvwxyz"; //I could go on...
char *alpha = "abcdefghijklmnopqrstuvwxyz";
char *ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *numeric = "0123456789";
char *special = "-_=+*&^%$#@!~\"'`[](){};:\\/?,.<> ";
store_t s = {}; //store the word file here
 
int main(int argc, char *argv[]) {
    int c = 0;
    char *desString = NULL;
    char *subSets = NULL;
    //char *charSet = NULL;
    char charSet[MAXCHARSETLENGTH] = {'\0'};
    char *dictFile = NULL;
    int bruteUpToX = MAXPASSWORDLENGHT;
    int charSetSize = 0;
    char salt[3];
    bool bflag = false;
    bool kflag = false;
    bool sflag = false;
    bool Sflag = false;
    bool dflag = false;
    seconds = time(NULL);
    opterr = 0; //unistd.h
 
    while ((c = getopt(argc, argv, "b::k:s:S:d:")) != -1)
    {
        switch (c)
        {
            case 'b': // indicates whether the user wants to use brute force.
                bflag = true;
                if (optarg != 0)
                {
                    bruteUpToX = atoi(optarg);
                }
                break;
            case 'k': // the DES string
                kflag = true;
                desString = optarg;
                break;
            case 's': // string can contain a, A, # and 0. indicates the subsets of the predefined charsets to use
                sflag = true;
                bflag = true; // implied
                subSets = optarg;
                break;
            case 'S': // the characters to brute force with, literally entered
                Sflag = true;
                bflag = true; // implied
                //free(charSet);
                memcpy(&charSet, optarg, strlen(optarg));
                break;
            case 'd': // TODO the dictionary file
                dflag = true;
                dictFile = optarg;
                break;
            case '?':
                if (optopt == 'k' || optopt == 'd' || optopt == 's')
                    fprintf(stderr, "ERROR: Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "ERROR: Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "ERROR: Unknown option character `\\x%x'.\n", optopt);
                usage();
                return 1;
            default:
                exit(EXIT_FAILURE);
        }
    }
 
    //sanity checks
    if (sflag && Sflag)
    {
        printf("ERROR: -s and -S are mutually exclusive.\n");
        usage();
    }
    if (!kflag)
    {
        printf("ERROR: Missing DES56 key.\n");
        usage();
    }
    if (strlen(desString) != DES56KEYLENGTH) // it's always 13 chars
    {
        printf("ERROR: That's not a DES56 key!\n");
        usage();
    }
    // extract the salt, from the key...
    salt[0] = desString[0];
    salt[1] = desString[1];
    salt[2] = '\0';
 
    if (dflag) // perform a dictionary attack
    {
        //load the dictionary
        if (!load(dictFile))
        {
            printf("Failed to load the dictionary.\n");
            exit(EXIT_FAILURE);
        }
 
        printf("\n");
        uint32_t cur = 0;
        char *seek_storage;
        char *enc;
        while (cur < s.size) // move word by word across s.s until the end.
        {
            seek_storage = &s.s[cur];
            length_t len = (length_t) strlen(seek_storage);
            if (len == 0) // this will never happen, made sure of it in load_storage.
            {
                cur++;
                continue;
            }
            //fluff
            printf("\r%-45s", seek_storage);
            fflush(stdout);
 
            tryPassword(seek_storage, desString, salt);
            cur += len + 1; // the + 1 is for the \0, now seek_storage (will) points to the first letter of the next word
        }
 
        printf("Dictionary attack failed. None of the dictionary entries matched the password. Try another dictionary.\n");
    }
 
    if (bflag) // perform a brute force attack
    {
        // makes sure the bruteUpToX is within reasonable bounds (1-8)
        bruteUpToX =
                        (bruteUpToX <= 0 || bruteUpToX > MAXPASSWORDLENGHT)
                        ? MAXPASSWORDLENGHT
                        : bruteUpToX;
 
        if (Sflag) // charset is typed in by the user
        {
            charSetSize = strlen(charSet);
            for (int i = charSetSize; i > 0; i--)
            {
                //bubble-move the '\0' (nul termination char) to the begining
                char tmp = charSet[i];
                charSet[i] = charSet[i - 1];
                charSet[i - 1] = tmp;
            }
            charSetSize++; // this is no longer a valid 'string' but an array of chars of size 'charSetSize'
        }
        else
        {
            // charset is calculated
            bool alphaused = false;
            bool ALPHAused = false;
            bool numericused = false;
            bool specialused = false;
            bool leetused = false;
            charSet[0] = '\0'; // all charsets start with '\0'
            charSetSize = 1;
            printf("charSetSize: %d\n", charSetSize);
            if (sflag)
            {
                //user used the -s flag
                printf("\nsubSets=%s\n", subSets);
            }
            else
            {
                subSets = DEFAULT_SUBSETS;
            }
            int len = strlen(subSets);
            int sublen;
            for (int i = 0; i < len; i++)
            {
                switch (subSets[i])
                {
                    case 'a':
                        if (alphaused) makeCharSetError(__LINE__); // only one appearance allowed
                        alphaused = true;
                        sublen = strlen(alpha);
                        for (int j = 0; j < sublen; j++, charSetSize++)
                        {
                            charSet[charSetSize] = alpha[j];
                        }
                        break;
                    case 'A':
                        if (ALPHAused) makeCharSetError(__LINE__); // only one appearance allowed
                        ALPHAused = true;
                        sublen = strlen(ALPHA);
                        for (int j = 0; j < sublen; j++, charSetSize++)
                        {
                            charSet[charSetSize] = ALPHA[j];
                        }
                        break;
                    case '0':
                        if (numericused) makeCharSetError(__LINE__); // only one appearance allowed
                        numericused = true;
                        sublen = strlen(numeric);
                        for (int j = 0; j < sublen; j++, charSetSize++)
                        {
                            charSet[charSetSize] = numeric[j];
                        }
                        break;
                    case '#':
                        if (specialused) makeCharSetError(__LINE__); // only one appearance allowed
                        specialused = true;
                        sublen = strlen(special);
                        for (int j = 0; j < sublen; j++, charSetSize++)
                        {
                            charSet[charSetSize] = special[j];
                        }
                        break;
                    case 'l':
                        if (leetused || numericused || ALPHAused || alphaused) makeCharSetError(__LINE__);
                        // only allow the 'l33t' charset on its own or with the special charset.
                        leetused = true;
                        sublen = strlen(l33t);
                        for (int j = 0; j < sublen; j++, charSetSize++)
                        {
                            charSet[charSetSize] = l33t[j];
                        }
                        break;
                    default:
                        makeCharSetError(__LINE__);
                }
            }
            //if subSets length was 0, or none of the markers matched.
            if (len == 0 || !(alphaused && ALPHAused && numericused && specialused))
                makeCharSetError(__LINE__);
        }
        printf("\n");
        bruteForce(desString, salt, charSet, charSetSize, bruteUpToX);
        printf("\nBrute Force attack failed. Try a different character set");
        if (bruteUpToX < 8) printf(" or a longer length limit.");
        printf(".\n");
    }
 
    unload();
    return (EXIT_SUCCESS);
}
 
/**
 * used in bruteForce as a way to climb up string lengths
 * @param indexExhausted
 * @return
 */
int iStart(bool indexExhausted) {
    if (indexExhausted)
        return 1;
    else
        return 0;
}
 
/**
 * prints usage and exits
 */
void usage(void) {
    printf("Usage: ./crack -k<DES56string> OPTIONS\n"
                    "OPTION -b[N] enable brute force. optionally specify N characters to attempt.\n"
                    "     ie.-b4 will brute force only up to 4 characters.\n"
                    "     Default N=8, Max N=8 (DES56 only uses 8 chars anyway).\n"
                    "OPTION -s<SETS> specify which predefined character sets to brute force with. Implies -b\n"
                    "     SETS can be: a, A, 0, #.\n"
                    "       a: %s\n"
                    "       A: %s\n"
                    "       0: %s\n"
                    "       #: %s (Special characters. Includes space)\n"
                    "       l: %s \n(The l33t charset may only be used alone, or with the Special characters (#) set.)\n"
                    "OPTION -S<LITERAL_CHARACTERS> specify exact characters to use with brute force. Implies -b\n"
                    "If neither -s or -S are specified -saA0# is assumed.\n"
                    "OPTION -d<dictionary file> specify a dictionary file to use in attack.\n"
                    "If dictionary file is specified and brute force is enabled, dictionary attack is executed first, brute second.\n",
                    alpha, ALPHA, numeric, special, l33t);
    exit(EXIT_FAILURE);
}
 
/**
 * performs brute force attack
 * ....easiest way to do this is by having n for loops that iterate the charSet range.
 * Unlike a 'prettier' iterator, this iterator keeps the last character stable while
 * looping through the first n-1 characters. The end result is the same.
 * NOTE: you may think of charSet as a char* or a char[] but it is NOT a valid C string, hence
 * the need for charSetSize. (first element is nil, last element is _not_ nil)
 * @param desString
 * @param salt
 * @param charSet
 * @param charSetSize int charSetSize the number of chars in the charset
 * @param bruteUpToX int bruteUpToX up to how many characters to brute force to.
 * @return true on success, false on failure.
 */
bool bruteForce(char *desString, char salt[], char charSet[], int charSetSize, int bruteUpToX) {
    /* declare variables ONCE, here, it's faster. */
    //holds the result of crypt()
    char *enc = malloc((DES56KEYLENGTH + 1) * sizeof (char)); //13+1=14
    //bool sevenExhausted = false; // really not needed
    bool sixExhausted = false;
    bool fiveExhausted = false;
    bool fourExhausted = false;
    bool threeExhausted = false;
    bool twoExhausted = false;
    bool oneExhausted = false;
    bool zeroExhausted = false;
    char *crack = malloc((MAXPASSWORDLENGHT + 1) * sizeof (char));
    for (int i = 0; i < MAXPASSWORDLENGHT + 1; i++)
    {
        crack[i] = '\0'; // zero out all characters.
    }
    //unsigned int counter = 0;
    for (short int iSeven = iStart(sixExhausted); iSeven < charSetSize; iSeven++)
    {
        crack[7] = charSet[iSeven];
        for (short int iSix = iStart(fiveExhausted); iSix < charSetSize; iSix++)
        {
            crack[6] = charSet[iSix];
            for (short int iFive = iStart(fourExhausted); iFive < charSetSize; iFive++)
            {
                crack[5] = charSet[iFive];
                for (short int iFour = iStart(threeExhausted); iFour < charSetSize; iFour++)
                {
                    crack[4] = charSet[iFour];
                    for (short int iThree = iStart(twoExhausted); iThree < charSetSize; iThree++)
                    {
                        crack[3] = charSet[iThree];
                        ////this should help to empirically see how long this thing will take...
                        //printf("\r%s", crack);
                        //fflush(stdout);
                        for (short int iTwo = iStart(oneExhausted); iTwo < charSetSize; iTwo++)
                        {
                            crack[2] = charSet[iTwo];
                            for (short int iOne = iStart(zeroExhausted); iOne < charSetSize; iOne++)
                            {
                                crack[1] = charSet[iOne];
                                //this should help to empirically see how long this thing will take...
                                printf("\r%s", crack);
                                fflush(stdout);
                                for (short int iZero = iStart(true); iZero < charSetSize; iZero++)
                                {
                                    crack[0] = charSet[iZero];
                                    ////this should help to empirically see how long this thing will take...
                                    //printf("\r%s", crack);
                                    //fflush(stdout);
 
                                    tryPassword(crack, desString, salt);
                                }
                                zeroExhausted = true;
                                if (bruteUpToX <= 1) return false;
                            }
                            oneExhausted = true;
                            if (bruteUpToX <= 2) return false;
                        }
                        twoExhausted = true;
                        if (bruteUpToX <= 3) return false;
                    }
                    threeExhausted = true;
                    if (bruteUpToX <= 4) return false;
                }
                fourExhausted = true;
                if (bruteUpToX <= 5) return false;
            }
            fiveExhausted = true;
            if (bruteUpToX <= 6) return false;
        }
        sixExhausted = true;
        if (bruteUpToX <= 7) return false;
    }
    return false;
}
 
void makeCharSetError(int line) {
    printf("ERROR%d: invalid SETS provided.\n", line);
    usage();
}
 
/**
 * --modified from pset6 dictionary.c--
 * loads the entire dictionary file into RAM, but changes '\n' to '\0', and some other minor stuff.
 * parts of this taken from CS50.c's GetString()
 * @param dfp
 *              FILE *dfp must be a valid file pointer.
 * @return boolean false on failure.
 */
bool load_storage(FILE *dfp) { //done
    //paranoia
    if (s.size || s.s || s.wordcount)
    {
        printf("store structure was already initialized\n");
        return false;
    }
 
    // capacity of store
    unsigned int capacity = 0;
 
    while (true)
    {
        char c = fgetc(dfp);
 
        // grow buffer if necessary
        if (s.size + 1 > capacity)
        {
            // determine new capacity: start at 32 then double
            if (capacity == 0)
                capacity = 32;
            else if (capacity <= (UINT_MAX / 2))
                capacity *= 2;
            else
            {
                if (s.s) free(s.s);
                return false;
            }
 
            // extend buffer's capacity
            char *temp = realloc(s.s, capacity * sizeof (char));
            if (temp == NULL)
            {
                if (s.s) free(s.s);
                return false;
            }
            s.s = temp;
        }
 
        if (feof(dfp)) //reached eof
        {
            if (s.size == 0)
            {
                if (s.s) free(s.s);
                return false;
            }
            //at the end of it all make sure there is a '\0'
            if (s.s[ s.size - 1 ] != '\0') // the last word had no \n and it preceeded EOF
            {
                s.s[ s.size ] = '\0';
                s.wordcount++;
            }
            else // the previous word ended with a \n, so it was terminated just fine.
            {
                s.size--;
            }
            break; // done with the loop
        }
 
        if (c == '\n')
        {
            //if char is \n
            if (s.size > 0 && s.s[ s.size - 1 ] != '\0') // don't bother loading blank lines (blank 'words') into storage
            {
                s.s[ s.size ] = '\0';
                s.wordcount++;
                s.size++;
            }
        }
        else
        {
            // append current character to s.s
            s.s[ s.size ] = c;
            s.size++;
        }
    } /*while (true)*/
 
    s.size = s.size + 1; // don't forget the 0th index!
 
    // minimize buffer ... this may be a bad idea to do for very large buffers.
    char *minimal = malloc(s.size * sizeof (char));
    memcpy(minimal, s.s, s.size);
    free(s.s);
    s.s = minimal;
 
    return true;
}
 
/**
 * --modified from pset6 dictionary.c--
 * Loads dict into memory.  Returns true if successful else false.
 * @param dict
 *           const char *dict the /path/to/filename of the dictionary
 * @return boolean false on failure.
 */
bool load(const char *dict) {
    static bool calledme = false;
    if (calledme)
    {
        printf("Warning, load may only be called once.\n");
        return false;
    }
    calledme = true;
    //initialize store
    s.s = NULL;
    s.size = 0;
    s.wordcount = 0;
 
    /* open file
     * load entire file into ram, convert \n to \0 in the process.
     * also count the number of words (where len > 0) during this phase!
     * close file
     */
 
    FILE *dfp;
    //paranoia
    if (!(dict) || !(dict[0]) || (dfp = fopen(dict, "r")) == NULL)
    {
        return false;
    }
    //load the entire dictionary into RAM, for easier management
    if (!(load_storage(dfp)))
    {
        fclose(dfp);
        return false;
    }
    fclose(dfp);
 
    //printf("\nDone loading dictionary\n");
 
    return true;
}
 
/**
 * Unloads (frees memory of) the storage.
 */
void unload() {
    if (s.s) free(s.s);
}
 
/**
 * quick and dirty string equality check -- only usable in the context of DES56KEYLENGTH length strings.
 * this *might* be faster than strcmp, since it doesn't weigh chars, and it breaks as soon as it differs.
 * @param str1
 *              const char *str1 the first string
 * @param str2
 *              const char *str2 the second string
 * @return boolean false on mismatch true on match
 */
bool encstreq(const char *str1, const char *str2) {
    for (int i = 0; i < DES56KEYLENGTH; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}
 
void tryPassword(char *passToTry, char *desString, char salt[]) {
    char *enc = (char *) crypt(passToTry, salt);
    if (enc == NULL)
    {
        printf("\nERROR, crypt() returned NULL for: %s\n", passToTry);
        unload();
        exit(EXIT_FAILURE);
    }
    if (strlen(enc) == DES56KEYLENGTH)
    {
        if (encstreq(desString, enc))
        {
            printf("\n\nSuccess!\n\nThe key was: %s\nThe plaintext is: %s\n", desString, passToTry);
            printf("\nFinish time=%d, took: %d seconds\n", (int) time(NULL), (int) time(NULL) - seconds);
            unload();
            exit(EXIT_SUCCESS);
        }
    }
}
 
 
/*
 * CS50 fall2010 hacker2 target:
 * crack these passwords:
 * guest:50Bt2CexZzo7k
 * jcaesar:50zPJlUFIYY0o
 * cpisonis:HAR0lc1egPNKQ
 * pskroob:50Bpa7n/23iug
 * mscott:50q.zrL5e0Sak
 * gcostanza:50vfotBdeBr.o
 * bvigenere:505YXx3Mz50bg
 * dmalan:50SRu3A71mm4A
 */
 
//gcc -ggdb -std=c99 -lcrypt -o brute brute.c
//gcc -O3 -std=c99 -lcrypt -o brute brute.c
 
//gcc.exe -std=c99   -c -g -Werror -I. -MMD -MP -MF build/Debug/Cygwin-Windows/crack.o.d -o build/Debug/Cygwin-Windows/crack.o crack.c
//gcc.exe -std=c99    -o dist/Debug/Cygwin-Windows/hacker2_brute build/Debug/Cygwin-Windows/crack.o -lcrypt

