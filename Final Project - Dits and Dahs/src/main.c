#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <Board.h>
#include <Oled.h>
#include <timers.h>
#include <PWM.h>
#include <ADC.h>
#include <buttons.h>

#include "List.h"
#include <CAPTOUCH.h>

int LEVEL = 1;          // Current player level
int current_letter;     // The current letter being tested as a number (1-26)
char CURRENT_LETTER;    // The current letter being tested as a character ('A' - 'Z')
char display[100] = ""; // Stores the OLED display string

int letters_completed = 0; // Counter to keep track how many letters have been completly learned in the level
int incorrect_guesses = 0; // Counter to keep track of how many times an incorrect guess has been made

int new_letter = 0;    // Set to 1 or 2 depending on if learning the first or second new letter
int random_number = 0; // Stores the random posision of new current letter to be tested
int DECODE = 0;        // Stores the output of DecodeMorse()

int SENSOR = 0; // Current Sensor being used in game
// SENSOR states: -------
#define NO_SENSE 0
#define FLEX 1
#define PESO 2
#define CAP_T 3
// ----------------------

int OLED_STATE = 0;
// OLED STATES: --------------------------------------------
#define WELCOME 0     // Initialization: Welcome Message
#define PICK_SENSOR 1 // Initialization: Pick a Sensor
#define LEARN_FLEX 2  // Learning: Flex Sensor
#define LEARN_PESO 3  // Learning: Peso Sensor
#define LEARN_CAP 4   // Learning: CapTouch Sensor
#define BREAK 5       // Break out of SM
#define TEACH_FLEX 6  // Teaching: Flex Sensor
#define TEACH_PESO 7  // Teaching: Peso Sensor
#define TEACH_CAP 8   // Teaching: CapTouch Sensor
#define CORRECT 9     // Guesses: Correct
#define INCORRECT 10  // Guesses: Incorrect
// ---------------------------------------------------------

// FLEX SENSOR variables -------------------------------------------
int index_f = 0; // stores the reading of the flex sensor for index
int thumb_f = 0; // stores the reading of the flex sensor for thumb
int flex = 0;    // state of flex sensor state
// -----------------------------------------------------------------

uint8_t buttons = 0; // variable to store the state of buttons()

// CAPTOUCH SENSOR variables ---------------------------------------
#define DIT_MIN 10
#define DIT_MAX 100
#define DAH_MIN 300

// Defines Morse code symbols
typedef enum
{
    DIT,
    DAH,
    SPACE
} Morse;

// Defines states for the states CapTouch SM
typedef enum
{
    touching,
    decode,
    wait
} State;

// Defines a structure for the state machine for sensing in CapTouch
typedef struct
{
    State state;
    unsigned int startTime;
    unsigned int endTime;
    unsigned int duration;
    int touch; // 0 for not touched, non-zero for touched
    Morse morse;
} StateMachine;
// -------------------------------------------------------------------

// Function prototype for the Decode function
void CapTouch(StateMachine *CAP, List A);

/// @brief MorseLibrary appends the binary representation
///         of morse codde of 'letter' to the list 'morseList'
/// @param letter
/// @param morseList
void MorseLibrary(int letter, List morseList)
{
    switch (letter)
    {
    case 1:
        // A
        append(morseList, 1);
        append(morseList, 0);
        break;

    case 2:
        // B
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 1);
        break;

    case 3:
        // C
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 1);
        break;

    case 4:
        // D
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 1);
        break;

    case 5:
        // E
        append(morseList, 1);
        break;

    case 6:
        // F
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 1);
        break;

    case 7:
        // G
        append(morseList, 0);
        append(morseList, 0);
        append(morseList, 1);
        break;

    case 8:
        // H
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 1);
        break;

    case 9:
        // I
        append(morseList, 1);
        append(morseList, 1);
        break;

    case 10:
        // J
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 0);
        append(morseList, 0);
        break;

    case 11:
        // K
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 0);
        break;

    case 12:
        // L
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 1);
        break;

    case 13:
        // M
        append(morseList, 0);
        append(morseList, 0);
        break;

    case 14:
        // N
        append(morseList, 0);
        append(morseList, 1);
        break;

    case 15:
        // O
        append(morseList, 0);
        append(morseList, 0);
        append(morseList, 0);
        break;

    case 16:
        // P
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 0);
        append(morseList, 1);
        break;

    case 17:
        // Q
        append(morseList, 0);
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 0);
        break;

    case 18:
        // R
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 1);
        break;

    case 19:
        // S
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 1);
        break;

    case 20:
        // T
        append(morseList, 0);
        break;

    case 21:
        // U
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 0);
        break;

    case 22:
        // V
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 0);
        break;

    case 23:
        // W
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 0);
        break;

    case 24:
        // X
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 1);
        append(morseList, 0);
        break;

    case 25:
        // Y
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 0);
        append(morseList, 0);
        break;

    case 26:
        // Z
        append(morseList, 0);
        append(morseList, 0);
        append(morseList, 1);
        append(morseList, 1);
        break;

    default:
        // Handle unknown letters or characters
        break;
    }
}

/// @brief MorsePlay will play morse corse corresponging to 'letter'
/// @param letter
void MorsePlay(int letter)
{

    // Sounds will store the binary representation of the current letter
    // from the morseLibrary
    List Sounds = newList();
    MorseLibrary(letter, Sounds);

    // turns off speaker, safety code
    PWM_SetDutyCycle(PWM_0, 0);

    // iterate throuhg the list Sound to determine what gets played
    moveFront(Sounds);
    for (int i = 0; i < length(Sounds); i++)
    {
        // if the sounds is a Dit (1) play short tone
        if (get(Sounds) == 1)
        {
            PWM_SetDutyCycle(PWM_0, 1);
            HAL_Delay(250);
            PWM_SetDutyCycle(PWM_0, 0);
            HAL_Delay(250);
        }

        // if sound is a Dah (0) play long tone
        if (get(Sounds) == 0)
        {
            PWM_SetDutyCycle(PWM_0, 1);
            HAL_Delay(750);
            PWM_SetDutyCycle(PWM_0, 0);
            HAL_Delay(250);
        }

        // move list cursor to the next element
        moveNext(Sounds);
    }

    // turn off speaker
    PWM_SetDutyCycle(PWM_0, 0);
}

/// @brief flex_sensor SM that appends 1/0 to list of user input
///         based on reading from flex sensor
/// @param Combo
void flex_sensor(List Combo)
{
    switch (flex)
    {
    // WAITING STATE
    case (0):

        thumb_f = ADC_Read(ADC_1); // dit
        index_f = ADC_Read(ADC_0); // dah

        // if the sensor was not flexed:
        if ((index_f > 1800) && (thumb_f > 1900))
        {
            flex = 1; // change to state 1: READING STATE
        }
        break;

    // READING STATE
    case (1):

        thumb_f = ADC_Read(ADC_1); // dit
        index_f = ADC_Read(ADC_0); // dah

        // if the thumb was flexed: dah detected
        if (thumb_f < 1500)
        {
            // add a 1 to the binary morse code representation list
            append(Combo, 1);
            flex = 0; // change to state 0: WAITING STATE
        }
        // if the index finger was flexed: dit detected
        else if (index_f < 1500)
        {

            // add a 0 to the binary morse code representation list
            append(Combo, 0);
            flex = 0; // change to state 0: WAITING STATE
        }
        break;

    default:
        break;
    }
}

/// @brief CapTouch SM that appends 1/0 to list of user inputs
///         based on duration of touch to capacative touch sensor
/// @param CAP
/// @param A
void CapTouch(StateMachine *CAP, List A)
{

    // State machine handle
    switch (CAP->state)
    {
    case wait:
        // If touched, record the start time and transition to the touching state
        if (CAP->touch)
        {
            // start timer
            CAP->startTime = TIMERS_GetMilliSeconds();
            CAP->state = touching;
        }
        break;

    case touching:
        //  If no longer touched, record the end time, calculate the duration, and transition to the decode state
        if (!CAP->touch)
        {
            // end timer
            CAP->endTime = TIMERS_GetMilliSeconds();

            // calculate the duration of a touch
            CAP->duration = CAP->endTime - CAP->startTime;

            // if the duration was greater that static noise readings then real reading was sensed
            if (CAP->duration > DIT_MIN)
                CAP->state = decode;
        }
        break;

    case decode:
        // Based on the duration, determine if it's a DIT or a DAH and print it out

        // if touch was a DIT append 1
        if (CAP->duration > DIT_MIN && CAP->duration < DIT_MAX)
        {
            CAP->morse = DIT;
            append(A, 1);
            CAP->state = wait;
        }
        // if touch was a DAH append 0
        else if (CAP->duration > DAH_MIN)
        {
            CAP->morse = DAH;
            append(A, 0);
            CAP->state = wait;
        }
        else
        {
            // If the touch doesn't fall within the given range reset
            CAP->morse = SPACE;
            CAP->state = wait;
        }

        // Reset the values for the next touch
        CAP->duration = 0;
        CAP->startTime = 0;
        CAP->endTime = 0;
        break;

    default:
        printf("ERROR\n");
        CAP->state = wait;
        break;
    }
}

/// @brief DecodeMorse compares the user input to the letter being
///         tested by comparing the lists with the binary representation
///         of the lists corresponding to the letter being tested and the letter the used input
/// @return position if letter is correct, 100 otherwise
int DecodeMorse()
{

    int decoded_morse = 0; // will store the letter to be returned

    List Combo = newList(); // Store the input from the user

    // Stores the combination of the correct letter
    List morseList = newList();
    MorseLibrary(current_letter, morseList);

    // based on the sensor chosen by user, run corresponding algorithm
    if (SENSOR == FLEX)
    {

        flex = 0; // set the first state to Initialize flex SM

        // obtain reading of flex sensors until the user has input enough dits and dahs
        while (length(Combo) != length(morseList))
        {
            flex_sensor(Combo); // Combo list to store user input
        }

        // compare the user input list to the correct list
        if (equals(Combo, morseList))
        {
            // return the correct letter
            return current_letter;
        }
        else
        {
            // letters did not match
            return 100;
        }
    }
    else if (SENSOR == PESO)
    {
        // Initialize SM variables
        StateMachine CAP = {wait, 0, 0, 0, 0, SPACE};

        // Main loop
        while (length(Combo) != length(morseList))
        {
            // determine if cap was touched
            CAP.touch = CAPTOUCH_IsTouched();

            // Process the state machine
            CapTouch(&CAP, Combo);
        }

        // compare the user input list to the correct list
        if (equals(Combo, morseList))
        {
            // return the correct letter
            return current_letter;
        }
        else
        {
            // letters did not match
            return 100;
        }
    }
    else if (SENSOR == CAP_T)
    {
        // Initialize SM variables
        StateMachine CAP = {wait, 0, 0, 0, 0, SPACE};

        // Main loop
        while (length(Combo) != length(morseList))
        {
            // determine if cap was touched
            CAP.touch = CAPTOUCH_IsTouched();

            // Process the state machine
            CapTouch(&CAP, Combo);
        }

        // compare the user input list to the correct list
        if (equals(Combo, morseList))
        {
            // return the correct letter
            return current_letter;
        }
        else
        {
            // letters did not match
            return 100;
        }
    }

    return decoded_morse;
}

/// @brief allGuessed will return FALSE if the guesses for the letter haven't been met.
///         For example if the random number is 3 and the guesses list is [0, 2, 1, 3, 4]
///         then there is one more one correct guess to be made for the third letter in the list
///         therefore all guesses have not been made for that letter
/// @param L
/// @param rand_num
/// @return bool based on if the all correct guesses have been made or not
bool allGuessed(List L, int rand_num)
{
    // iterate through the list and determine if the value corresponging to the guesses is 0
    moveFront(L);
    for (int i = 0; i <= rand_num; i++)
    {
        if (indexL(L) == rand_num)
        {
            if (get(L) == 0)
            {
                return TRUE;
            }
        }

        moveNext(L);
    }
    return FALSE;
}

/// @brief moveToCorrectPosition moves the cursor for the lists to the position pos
/// @param LB
/// @param G
/// @param pos
void moveToCorrectPosition(List LB, List G, int pos)
{

    moveFront(LB);
    moveFront(G);

    // move the cursor until the index reaches 'pos'
    while (indexL(LB) != pos)
    {
        moveNext(LB);
        moveNext(G);
    }

    return;
}

/// @brief randNum generates a random number within range of size of our letterbank
///         also ensures that the random number isn't one from a letter that is no longer being tested
/// @param LB
/// @param G
/// @return random number
void randNum(List LB, List G, int *result)
{
    int lower_bound = 0;
    int upper_bound = length(LB) - 1;
    *result = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
}

/// @brief changeGuess takes the list of guesses and changes (by +- point)
/// @param G
/// @param point
void changeGuess(List G, int point)
{
    int score = get(G);
    set(G, score + point);
}

/// @brief NumberToLetter will take 'number' and return the
///         letter corresponging to that position in the aplahbet
/// @param number
/// @return char corresponging to letter
char NumberToLetter(int number)
{
    if (number >= 1 && number <= 26)
    {
        // Convert number to corresponding letter by adding the ASCII value of 'A' minus 1
        return 'A' + number - 1;
    }
    else
    {
        // Return 0 for numbers outside the valid range
        return 0;
    }
}

/// @brief LetterToNumber return the position of 'letter' in the US aplhabet
/// @param letter
/// @return int corresponding to position of letter in alphabet
int LetterToNumber(char letter)
{
    if (isalpha(letter))
    {
        // Convert letter to uppercase if necessary
        letter = toupper(letter);
        // Calculate position by subtracting the ASCII value of 'A' and adding 1
        return letter - 'A' + 1;
    }
    else
    {
        // Return 0 for non-alphabetic characters
        return 0;
    }
}

/// @brief insertLetter will insert the letter and the corresponding
///         required guesses into thier respective lists
/// @param LB   list bank
/// @param G    guesses list
/// @param letter   letter to be appended
/// @param guess    guesses req to be appended
void insertLetter(List LB, List G, char let, int guess)
{

    int letter = LetterToNumber(let);
    printf("Inserting Letter: %d\r\n", letter);
    append(LB, letter);
    append(G, guess);
}

/// @brief OLED_DISPLAY_GAME determines what will be displayed to the user on the OLED
/// @param STATE
void OLED_DISPLAY_GAME(int STATE)
{
    switch (STATE)
    {
    case (0):
        // INITIALIZATION: Welcome Message
        OledDrawString("Hello! Welcome to \r\n    Dits & Dahs :)\r\n press any button!");
        OledUpdate();
        break;

    case (1):
        // INITIALIZATION: Pick a Sensor
        OledDrawString("Select your sensor \r\nusing the buttons:\r\n\n1)Flex 2)Peso 3)Cap");
        OledUpdate();
        break;

    case (2):
        // LEARNING: Sensor is Flex
        CURRENT_LETTER = NumberToLetter(current_letter);
        sprintf(display, "Sensor: FLEX \r\nLevel: %d  \r\n Learning Letter: %c", LEVEL, CURRENT_LETTER);
        OledDrawString(display);
        OledUpdate();
        break;

    case (3):
        // LEARNING: Sensor is Peso
        CURRENT_LETTER = NumberToLetter(current_letter);
        sprintf(display, "Sensor: PESO \r\nLevel: %d  \r\n Learning Letter: %c", LEVEL, CURRENT_LETTER);
        OledDrawString(display);
        OledUpdate();
        break;

    case (4):
        // LEARNING: Sensor is Cap
        CURRENT_LETTER = NumberToLetter(current_letter);
        sprintf(display, "Sensor: CAP \r\nLevel: %d  \r\n Learning Letter: %c", LEVEL, CURRENT_LETTER);
        OledDrawString(display);
        OledUpdate();
        break;

    case (5):
        break;

    case (6):
        // Testing: Sensor is Flex
        CURRENT_LETTER = NumberToLetter(current_letter);
        sprintf(display, "Sensor: FLEX \r\nLevel: %d  \r\n Testing Letter: %c", LEVEL, CURRENT_LETTER);
        OledDrawString(display);
        OledDrawString(display);
        OledUpdate();
        break;

    case (7):
        // Testing: Sensor is Peso
        CURRENT_LETTER = NumberToLetter(current_letter);
        sprintf(display, "Sensor: PESO \r\nLevel: %d  \r\n Testing Letter: %c", LEVEL, CURRENT_LETTER);
        OledDrawString(display);
        OledDrawString(display);
        OledUpdate();
        break;

    case (8):
        // Testing: Sensor is Cap
        CURRENT_LETTER = NumberToLetter(current_letter);
        sprintf(display, "Sensor: CAP \r\nLevel: %d  \r\n Testing Letter: %c", LEVEL, CURRENT_LETTER);
        OledDrawString(display);
        OledUpdate();
        break;

    case (9):
        OledDrawString("Correct! :D");
        OledUpdate();
        HAL_Delay(1000);
        break;

    case (10):
        OledDrawString("Incorrect! :'(");
        OledUpdate();
        HAL_Delay(1000);
        break;

    default:
        break;
    }
}

/// @brief LowLevel SM that iterates through one level starting with teaching the user
///         two new letters then testing the user on the letters within the letter bank at random
/// @param LetterBank
/// @param Guesses
/// @param O_STATE
void LowLevel(List LetterBank, List Guesses, int O_STATE)
{

    // State will remain in TWO_NEW_LETTERS until user has learned both letters
    new_letter = 1;

    ///// TWO_NEW_LETTERS ////////////////////////////////////////////////////////////////////
    while (TRUE)
    {

        // If learning the first letter
        if (new_letter == 1)
        {
            // Moves the cursor to the back for both Lists to obtain the last element
            moveBack(LetterBank);
            moveBack(Guesses);

            // get the value of the element at the end of LetterBank
            current_letter = get(LetterBank);

            // Display the letter to the OLED depending on the sensor being tested
            if (SENSOR == FLEX)
            {
                // FLEX
                O_STATE = 2;
                OLED_DISPLAY_GAME(O_STATE);
            }
            else if (SENSOR == PESO)
            {
                // PESO
                O_STATE = 3;
                OLED_DISPLAY_GAME(O_STATE);
            }
            else if (SENSOR == CAP_T)
            {
                // CAP
                O_STATE = 4;
                OLED_DISPLAY_GAME(O_STATE);
            }

            // Play the letter
            MorsePlay(current_letter);

            // Obtain and decode the user input
            DECODE = DecodeMorse();

            if (DECODE == current_letter)
            {
                // Display Correct on OLED
                O_STATE = 9;
                OledClear(0);
                OledUpdate();
                OLED_DISPLAY_GAME(O_STATE);

                // letter being learned is the second one
                new_letter = 2; // move on to second letter
            }
            else
            {
                //  Display Incorrect on OLED
                O_STATE = 10;
                OledClear(0);
                OledUpdate();
                OLED_DISPLAY_GAME(O_STATE);

                // The input was not the current letter
                new_letter = 1; // restart learning current letter
            }
        }

        // If learning the second letter
        if (new_letter == 2)
        {

            // get the value of the element at the second to last
            movePrev(LetterBank);
            movePrev(Guesses);

            // move the cursor to the second to last letter
            moveToCorrectPosition(LetterBank, Guesses, (length(LetterBank) - 2));
            current_letter = get(LetterBank);

            // Display the letter to the OLED
            if (SENSOR == FLEX)
            {
                // FLEX
                O_STATE = 2;
                OLED_DISPLAY_GAME(O_STATE);
            }
            else if (SENSOR == PESO)
            {
                // PESO
                O_STATE = 3;
                OLED_DISPLAY_GAME(O_STATE);
            }
            else if (SENSOR == CAP_T)
            {
                // CAP
                O_STATE = 4;
                OLED_DISPLAY_GAME(O_STATE);
            }

            // Play the letter
            MorsePlay(current_letter);

            // Obtain and decode the user input
            DECODE = DecodeMorse();
            if (DECODE == current_letter)
            {
                // Display Correct on OLED
                O_STATE = 9;
                OledClear(0);
                OledUpdate();
                OLED_DISPLAY_GAME(O_STATE);

                // All new letters were learned
                new_letter = 1; // reset for next level
                break;          // leave while loop
            }
            else
            {
                // The input was not the current letter
                // second letter has not been learned
                O_STATE = 10;
                OledClear(0);
                OledUpdate();
                OLED_DISPLAY_GAME(O_STATE);
                new_letter = 2;
            }
        }
    }

    ///// START_LEVEL ///////////////////////////////////////////////////////////////////////

    // Resets all required guesses to 3
    moveFront(Guesses);
    for (int i = 0; i < length(LetterBank); i++)
    {
        set(Guesses, 3);
        moveNext(Guesses);
    }

    // initialize variables for the teaching stage
    letters_completed = 0;
    incorrect_guesses = 0;

    ///// TWO STATES INSIDE LOOP !
    while (letters_completed < length(LetterBank))
    {

        ///// PROMPT_LETTER ////////////////////////////////////////////////////////////////

        // Generating random number
        randNum(LetterBank, Guesses, &random_number);

        // First check if completed the level
        if (letters_completed == length(LetterBank))
        {
            break;
        }

        // Check if the previous guess was correct and assign current letter to random letter in LetterBank
        if (incorrect_guesses == 0)
        {
            moveFront(Guesses);

            // safety check to see if level was completed
            int LC = 0;
            for (int i = 0; i > length(LetterBank); i++)
            {
                if (get(Guesses) <= 0)
                {
                    LC++;
                }
                moveNext(Guesses);
            }
            if (LC == length(Guesses))
            {
                break;
            }

            // move the lists' cursor to that position to assign the current letter
            moveToCorrectPosition(LetterBank, Guesses, random_number);
            current_letter = get(LetterBank);
        }
        else if (incorrect_guesses > 0)
        {
            // previous guess was incorrect so current_letter stays the same
            current_letter = current_letter;
        }

        // Display the letter to the OLED
        if (SENSOR == FLEX)
        {
            // FLEX
            O_STATE = 6;
            OLED_DISPLAY_GAME(O_STATE);
        }
        else if (SENSOR == PESO)
        {
            // PESO
            O_STATE = 7;
            OLED_DISPLAY_GAME(O_STATE);
        }
        else if (SENSOR == CAP_T)
        {
            // CAP
            O_STATE = 8;
            OLED_DISPLAY_GAME(O_STATE);
        }

        ///// USER_GUESSES ////////////////////////////////////////////////////////////////

        // Obtain and decode the user input
        DECODE = DecodeMorse();
        if (DECODE == current_letter)
        {
            // Display Correct on OLED
            O_STATE = 9;
            OledClear(0);
            OledUpdate();
            OLED_DISPLAY_GAME(O_STATE);

            // decrease the requires guesses needed
            moveToCorrectPosition(LetterBank, Guesses, random_number);
            changeGuess(Guesses, -1);
            moveToCorrectPosition(LetterBank, Guesses, random_number);

            // if letter has been guessed the required times increase counter
            if (get(Guesses) == 0)
            {
                letters_completed += 1;
            }

            // checks if level has been completed
            if (letters_completed == length(Guesses))
            {
                // exit
                break;
            }

            incorrect_guesses = 0;

            // GO TO PROMPT STATE
        }
        else
        {
            // Letter Incorrect
            O_STATE = 10;
            OledClear(0);
            OledUpdate();
            OLED_DISPLAY_GAME(O_STATE);

            // increase the requires guesses needed
            moveToCorrectPosition(LetterBank, Guesses, random_number);
            changeGuess(Guesses, 1);

            // increment to flag
            incorrect_guesses += 1;

            // if the user has guessed the letter incorreclty 3 times or more play sound to help them
            if (incorrect_guesses >= 3)
            {
                // Play the letter
                MorsePlay(current_letter);
            }

            // GO TO PROMPT STATE
        }
        //--------
    }
}

/// @brief State Machine to iterate through all levels of the game
///        Each level contains two new letters that will be added to the letter bank
void HighLevel(List LetterBank, List Guesses, int O_STATE)
{
    // for each level:
    // 1. insert two new letters
    // 2. enter the low level SM to play the level

    switch (LEVEL)
    {

    case (1):

        insertLetter(LetterBank, Guesses, 'E', 3);
        insertLetter(LetterBank, Guesses, 'T', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (2):

        insertLetter(LetterBank, Guesses, 'A', 3);
        insertLetter(LetterBank, Guesses, 'N', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (3):
        insertLetter(LetterBank, Guesses, 'I', 3);
        insertLetter(LetterBank, Guesses, 'M', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (4):
        insertLetter(LetterBank, Guesses, 'U', 3);
        insertLetter(LetterBank, Guesses, 'D', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (5):
        insertLetter(LetterBank, Guesses, 'S', 3);
        insertLetter(LetterBank, Guesses, 'O', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (6):
        insertLetter(LetterBank, Guesses, 'R', 3);
        insertLetter(LetterBank, Guesses, 'K', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (7):
        insertLetter(LetterBank, Guesses, 'W', 3);
        insertLetter(LetterBank, Guesses, 'G', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (8):
        insertLetter(LetterBank, Guesses, 'Q', 3);
        insertLetter(LetterBank, Guesses, 'F', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (9):
        insertLetter(LetterBank, Guesses, 'P', 3);
        insertLetter(LetterBank, Guesses, 'X', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (10):
        insertLetter(LetterBank, Guesses, 'Y', 3);
        insertLetter(LetterBank, Guesses, 'L', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (11):
        insertLetter(LetterBank, Guesses, 'V', 3);
        insertLetter(LetterBank, Guesses, 'B', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (12):
        insertLetter(LetterBank, Guesses, 'Z', 3);
        insertLetter(LetterBank, Guesses, 'H', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    case (13):
        insertLetter(LetterBank, Guesses, 'C', 3);
        insertLetter(LetterBank, Guesses, 'J', 3);
        LowLevel(LetterBank, Guesses, O_STATE);
        break;

    default:
        // Handle unknown letters or characters
        break;
    }
}

int main(void)
{
    // Initializing random seed
    srand(time(0));

    // Initializing all necessary libraries
    BOARD_Init();
    OledInit();
    PWM_Init();
    TIMER_Init();
    ADC_Init();
    CAPTOUCH_Init();

    List LettBank = newList();   // Letter Bank to hold the list of letters to be learned and tested
    List GuessesReq = newList(); // Guesses requiered for every letter in the LettBank for user to guess
    // Example at begining of Level 2:
    // LettBank:    [E, T, A, N]
    // GuessesReq:  [3, 3, 3, 3]

    // Initialize all necesally states and variables
    LEVEL = 1;
    SENSOR = NO_SENSE;
    OLED_STATE = WELCOME;
    buttons = 0;
    incorrect_guesses = 0;

    // Clear the oled display
    OledClear(OLED_COLOR_BLACK);
    OledUpdate();

    PWM_SetFrequency(415);

    // In loop to stay in the game
    while (TRUE)
    {

        // Initializing the messages to the user
        while (TRUE)
        {

            // Logic to have user pick what sensor they would like to test

            // Initially: Welcome Message
            OLED_DISPLAY_GAME(OLED_STATE);

            // Checks if buttons have been pressed
            buttons = buttons_state();

            // If welcome message displayed and any button is pressed:
            if ((OLED_STATE == 0) && ((((buttons >> 3) & 0x1) == 0) || (((buttons >> 2) & 0x1) == 0) || (((buttons >> 1) & 0x1) == 0) || (((buttons >> 0) & 0x1) == 0)))
            {
                // In State 0: Welcome Message
                // finished = 0;
                OledClear(0);
                OledUpdate();
                OLED_STATE = 1; // GO TO -> State 1: Pick a Sensor
            }
            else if ((OLED_STATE == 1))
            {
                // IN STATE 1: Pick a Sensor

                if (((buttons >> 0) & 0x1) == 0)
                {
                    // FLEX
                    OledClear(0);
                    OledUpdate();
                    OLED_STATE = 2; // GO TO -> State 2: Sensor is Flex
                    SENSOR = FLEX;
                }
                if (((buttons >> 1) & 0x1) == 0)
                {
                    // PESO
                    OledClear(0);
                    OledUpdate();
                    OLED_STATE = 3; // GO TO -> State 3: Sensor is Peso
                    SENSOR = PESO;
                }
                if (((buttons >> 2) & 0x1) == 0)
                {
                    // CAP
                    OledClear(0);
                    OledUpdate();
                    OLED_STATE = 4; // GO TO -> State 4: Sensor is CapTouch
                    SENSOR = CAP_T;
                }
                if (((buttons >> 3) & 0x1) == 0)
                {
                    OledClear(0);
                    OledUpdate();
                    OLED_STATE = 1; // GO TO -> State 1: Pick a Sensor
                }
            }
            else if (((buttons >> 3) & 0x1) == 0)
            {
                OledClear(0);
                OledUpdate();
                OLED_STATE = 0; // GO TO -> State 0: Welcome Message
            }

            // If the sensor has been picked then break out of first while loop
            if (SENSOR > 0)
            {
                break;
            }
        }

        // Playing the game
        while (TRUE)
        {

            // Sensor: Flex
            if ((SENSOR == FLEX))
            {
                OLED_STATE = 2;
                OLED_DISPLAY_GAME(OLED_STATE);
                HighLevel(LettBank, GuessesReq, OLED_STATE); // start the game
                LEVEL++;                                     // Move ot the next level

                // OLED will display pick sensor
                OLED_STATE = 1;
                SENSOR = NO_SENSE; // resets sensor picked flag
                break;
            }
            // Sensor: Peso
            else if ((SENSOR == PESO))
            {

                OLED_STATE = 3;
                OLED_DISPLAY_GAME(OLED_STATE);
                HighLevel(LettBank, GuessesReq, OLED_STATE); // start the game
                LEVEL++;                                     // Move ot the next level

                // OLED will display pick sensor
                OLED_STATE = 1;
                SENSOR = NO_SENSE; // resets sensor picked flag
                break;
            }
            // Sensor: Cap
            else if ((SENSOR == CAP_T))
            {
                OLED_STATE = 4;
                OLED_DISPLAY_GAME(OLED_STATE);
                HighLevel(LettBank, GuessesReq, OLED_STATE); // start the game
                LEVEL++; // move to the next level

                // OLED will display pick sensor
                OLED_STATE = 1;
                SENSOR = 0; // resets sensor picked flag
                break;
            }

            OLED_DISPLAY_GAME(OLED_STATE);
        }
    }

    printf("FINISHED!");
}
