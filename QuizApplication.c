#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a question
struct Question {
    char question[100];
    char answer[100];
    struct Question* next;
};

// Structure to represent a player
struct Player {
    char name[50];
    int score;
    struct Player* next;
};

// Function to create a new question node
struct Question* createQuestion(char* question, char* answer) {
    struct Question* newQuestion = (struct Question*)malloc(sizeof(struct Question));
    strcpy(newQuestion->question, question);
    strcpy(newQuestion->answer, answer);
    newQuestion->next = NULL;
    return newQuestion;
}

// Function to create a new player node
struct Player* createPlayer(char* name) {
    struct Player* newPlayer = (struct Player*)malloc(sizeof(struct Player));
    strcpy(newPlayer->name, name);
    newPlayer->score = 0;
    newPlayer->next = NULL;
    return newPlayer;
}

// Function to add a question to the quiz
void addQuestion(struct Question** headRef, char* question, char* answer) {
    struct Question* newQuestion = createQuestion(question, answer);
    if (*headRef == NULL) {
        *headRef = newQuestion;
    } else {
        struct Question* temp = *headRef;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newQuestion;
    }
}

// Function to add a player to the game
void addPlayer(struct Player** headRef, char* name) {
    struct Player* newPlayer = createPlayer(name);
    if (*headRef == NULL) {
        *headRef = newPlayer;
    } else {
        struct Player* temp = *headRef;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPlayer;
    }
}

// Function to display all players and their scores
void displayPlayers(struct Player* head) {
    struct Player* temp = head;
    while (temp != NULL) {
        printf("Player: %s | Score: %d\n", temp->name, temp->score);
        temp = temp->next;
    }
}

// Function to find the winner(s) based on the highest score
void findWinner(struct Player* head) {
    struct Player* temp = head;
    int maxScore = 0;

    // Find the maximum score
    while (temp != NULL) {
        if (temp->score > maxScore) {
            maxScore = temp->score;
        }
        temp = temp->next;
    }

    // Display the winner(s)
    temp = head;
    printf("\nWinner(s):\n");
    while (temp != NULL) {
        if (temp->score == maxScore) {
            printf("%s\n", temp->name);
        }
        temp = temp->next;
    }
}

// Function to conduct the quiz
void conductQuiz(struct Question* head, struct Player* players, int negativeMarking, int timer) {
    struct Question* temp = head;
    char userAnswer[100];
    
    while (temp != NULL) {
        printf("\n                     Quiz Question:\n");
        printf("%s\n", temp->question);

        struct Player* currentPlayer = players;
        while (currentPlayer != NULL) {
            if (players->next != NULL) {  // Display "Player's turn" only in multiplayer mode
                printf("\n%s's turn:\n", currentPlayer->name);
            }

            printf("Your Answer: ");
            fflush(stdout);
            
            time_t startTime = time(NULL);
            if (scanf("%s", userAnswer) != 1) {
                // Invalid input, clear the buffer and ask again
                printf("Invalid input. Please enter a valid answer.\n");
                while (getchar() != '\n');
                continue;
            }
            time_t endTime = time(NULL);
            double timeElapsed = difftime(endTime, startTime);
            printf("Time taken: %.1lf seconds\n", timeElapsed);
            
            if (timer && timeElapsed > 10) {
                printf("TIMES'S UP!!! %s took more than 10 seconds to answer.\n", currentPlayer->name);
            } else {
                if (strcasecmp(userAnswer, temp->answer) == 0) {
                    currentPlayer->score++;
                } else {
                    if (negativeMarking) {
                        currentPlayer->score--;
                    }
                }
            }
            currentPlayer = currentPlayer->next;
        }
        
        if (temp->next != NULL) {
            printf("Press Enter to continue to the next question...");
            fflush(stdout);
            while (getchar() != '\n');
        }
        
        temp = temp->next;
    }
    printf("\nQuiz completed!!!\n");
}

// Function to free the memory allocated for the quiz questions
void freeQuestions(struct Question* head) {
    struct Question* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to free the memory allocated for the players
void freePlayers(struct Player* head) {
    struct Player* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to choose difficulty level
int chooseDifficultyLevel() {
    int level;
    printf("Choose Difficulty Level\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Enter your choice: ");
    while (scanf("%d", &level) != 1 || level < 1 || level > 3) {
        // Invalid input, clear the buffer and ask again
        printf("Invalid input. Please enter a number between 1 and 3.\n");
        while (getchar() != '\n');
    }
    return level;
}

// Function to choose whether to play with negative marking
int chooseNegativeMarking() {
    int choice;
    printf("Choose whether to play with negative marking:\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Enter your choice: ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        // Invalid input, clear the buffer and ask again
        printf("Invalid input. Please enter 1 for Yes or 2 for No.\n");
        while (getchar() != '\n');
    }
    return choice;
}

// Function to choose whether to play with a timer for each question
int chooseTimer() {
    int choice;
    printf("Choose whether to play with a timer of 10 seconds for each question:\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Enter your choice: ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        // Invalid input, clear the buffer and ask again
        printf("Invalid input. Please enter 1 for Yes or 2 for No.\n");
        while (getchar() != '\n');
    }
    return choice;
}

// Function to add questions based on difficulty level
void addQuestionsBasedOnDifficulty(struct Question** headRef, int difficulty) {
    // Clear existing questions
    freeQuestions(*headRef);
    *headRef = NULL;
    int mcqfib;
    printf("Choose the question format");
    printf("\n1.Single word answer questions");
    printf("\n2.Multiple choice questions\n");
    while (scanf("%d", &mcqfib) != 1 || (mcqfib != 1 && mcqfib != 2)) {
        // Invalid input, clear the buffer and ask again
        printf("Invalid input. Please enter 1 or 2.\n");
        while (getchar() != '\n');
    }

    if (mcqfib == 1) {
        switch (difficulty) {
            case 1: // Easy
                addQuestion(headRef, "What is the capital of France?", "Paris");
                addQuestion(headRef, "What is 2 + 2?", "4");
                addQuestion(headRef, "What is the powerhouse of the cell?", "Mitochondria");
                break;
            case 2: // Medium
                addQuestion(headRef, "What is the largest planet in our solar system?", "Jupiter");
                addQuestion(headRef, "What is the chemical symbol for water?", "H2O");
                addQuestion(headRef, "Who wrote 'To Kill a Mockingbird'?", "Harper Lee");
                break;
            case 3: // Hard
                addQuestion(headRef, "What is the speed of light in a vacuum?", "299,792,458 m/s");
                addQuestion(headRef, "Who developed the theory of relativity?", "Albert Einstein");
                addQuestion(headRef, "What is the smallest bone in the human body?", "Stapes");
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    if (mcqfib == 2) {
        switch (difficulty) {
            case 1: // Easy
                addQuestion(headRef, "What is the capital of France?\n1.Paris\n2.Berlin\n3.Madrid\n4.Rome\n", "1");
                addQuestion(headRef, "What is 2 + 2?\n1.5\n2.4\n3.-9\n4.10\n", "2");
                addQuestion(headRef, "What is the powerhouse of the cell?\n1.Nucleus\n2.Rybosome\n3.Mitochondria\n4.Amoeba", "3");
                break;
            case 2: // Medium
                addQuestion(headRef, "What is the largest planet in our solar system?\n1.Mars\n2.Jupiter\n3.Saturn\n4.Pluto", "2");
                addQuestion(headRef, "What is the chemical symbol for water?\n1.HCl\n2.H2O\n3.H2SO4\n4.H2", "2");
                addQuestion(headRef, "Who wrote 'To Kill a Mockingbird'?\n1.Shakespear\n2.Einstein\n3.Harper Lee\n4.Chetan bhagat", "3");
                break;
            case 3: // Hard
                addQuestion(headRef, "What is the speed of light in a vacuum?\n1.345\n2.728\n3.380\n4.300", "4");
                addQuestion(headRef, "Who developed the theory of relativity?\n1.Stephen hawking\n2.Albert einstein\n3.John dalton\n4.Newton", "2");
                addQuestion(headRef, "What is the smallest bone in the human body?1.Thigh\n2.Elbow\n3.Stapes\n4.Knuckle", "3");
                break;
            default:
                printf("Invalid choice\n");
        }
    }
}

void correctanswers(int difficulty){
    if (difficulty==1){
        printf("CORRECT ANSWERS\n");
        printf("1.Paris\n2.4\n3.Mitochondria\n");
    }
    else if (difficulty==2){
        printf("CORRECT ANSWERS\n");
        printf("1.Jupiter\n2.H2O\n3.Harper Lee\n");
    }
    else{
        printf("CORRECT ANSWERS\n");
        printf("1.300 m/s\n2.Albert Einstein\n3.Stapes\n");
    }
}

// Function to choose whether to play single or multiplayer
int chooseGameMode() {
    int choice;
    printf("Choose the game mode:\n");
    printf("1. Single Player\n");
    printf("2. Multiplayer\n");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2)) {
        // Invalid input, clear the buffer and ask again
        printf("Invalid input. Please enter 1 for Single Player or 2 for Multiplayer.\n");
        while (getchar() != '\n');
    }
    return choice;
}

int main() {
    struct Question* quiz = NULL;
    struct Player* players = NULL;
    int difficulty = chooseDifficultyLevel();
    int negativeMarkingChoice = chooseNegativeMarking();
    int negativeMarking = (negativeMarkingChoice == 1) ? 1 : 0;
    int timerChoice = chooseTimer();
    int timer = (timerChoice == 1) ? 1 : 0;

    int gameMode = chooseGameMode();
    
    if (gameMode == 2) { // Multiplayer
        int numPlayers;
        printf("Enter the number of players: ");
        while (scanf("%d", &numPlayers) != 1 || numPlayers <= 0) {
            // Invalid input, clear the buffer and ask again
            printf("Invalid input. Please enter a positive integer for the number of players.\n");
            while (getchar() != '\n');
        }

        for (int i = 0; i < numPlayers; i++) {
            char playerName[50];
            printf("Enter player %d's name: ", i + 1);
            scanf("%s", playerName);
            addPlayer(&players, playerName);
        }
    } else { // Single Player
        char playerName[50];
        printf("Enter your name: ");
        scanf("%s", playerName);
        addPlayer(&players, playerName);
    }

    // Add questions based on difficulty level
    addQuestionsBasedOnDifficulty(&quiz, difficulty);

    // Conduct the quiz
    printf("\n                     Quiz Begins!\n");
    conductQuiz(quiz, players, negativeMarking, timer);

    // Display players and scores
    printf("\n                     Final Scores:\n");
    displayPlayers(players);
    
    if (gameMode == 2) {  // Display winner(s) only in multiplayer mode
        findWinner(players);
    }
    
    correctanswers(difficulty);

    // Free allocated memory
    freeQuestions(quiz);
    freePlayers(players);

    return 0;
}