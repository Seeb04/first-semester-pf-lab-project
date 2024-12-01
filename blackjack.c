#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define DECK_SIZE 13


int deck[] = {11, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
int userPoints = 1000;


int drawCard();//draws a card
int calculateTotal(int cards[], int size); //calculate total score of hand
void printCards(const char *player, int cards[], int size, int score);//print all cards currently in hand
int checkBlackjack(int score);//check if player has blackjack
float winMultiplier();//calculate winning pot
int betWin(int userBet, float multiplier);//calculate winning amount
void winPoints(int winAmount); //updates points if win
void losePoints(int userBet);//updates points if lose
void clearScreen(); //to clear console


void clearScreen() {
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux/macOS
#endif
}


void losePoints(int userBet) {
    printf("\nYou lost %d points!\n", userBet);
    userPoints -= userBet;
    printf("\nYour total points are : %d\n", userPoints);
}

void winPoints(int winAmount) {
    printf("\nYou won %d points!\n", winAmount);
    userPoints += winAmount;
    printf("\nYour total points are : %d\n", userPoints);
}

int betWin(int userBet, float multiplier) {
    return (userBet * multiplier);
}

float winMultiplier() {
    int lootPool = (rand() % 111) + 100; //%111 so that it goes upto 110

    if (lootPool < 150) {
        lootPool += 50;
    }

    printf("--------------------------------------------------\n");
    printf("This round's winning pot is : %d%%", lootPool);
    printf("\n--------------------------------------------------\n\n");
    
    float mult = (float)lootPool/100;

    return mult;
}

// Function to draw a random card
int drawCard() {
    return deck[rand() % DECK_SIZE];
}

int calculateTotal(int cards[], int size) {
    int total = 0, aceCount = 0;

    for (int i = 0; i < size; i++) {
        total += cards[i]; //totalling the score of cards
        if (cards[i] == 11) { // checking if any of the cards are aces
            aceCount++;
        }
    }

    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }

    return total;
}

void printCards(const char *player, int cards[], int size, int score) { //const char *player so that string can be passed as input and cant be modified accidentally
    
    printf("\n%s's cards: ", player);
    for (int i = 0; i < size; i++) {
        printf("%d ", cards[i]);
    }
    printf("\n%s's score: %d\n", player, score);
}

int checkBlackjack(int score) {
    return score == 21;
}

void blackjackGame() {
    int userCards[10], compCards[10]; //used to store cards of user and computer
    int userCardCount = 0, compCardCount = 0; //used to keep count for the amount of cards in hand for each player
    int userTotal = 0, compTotal = 0;
    int userBet, invalidBet = 0;
    char userChoice;

    // Initialize the random number generator
    srand(time(NULL));

    float multiplier = winMultiplier();
    
    printf("Your current points : %d\n", userPoints);

    printf("Enter bet : ");
    scanf("%d", &userBet);

    do {
        if (userBet < 0 || userBet > userPoints) {
            invalidBet = 1;
            printf("\n--------------------------------------------------\n");
            printf("Insufficient points!");
            // printf("\n\nYou have %d Points", userPoints);
            printf("\n--------------------------------------------------\n");
            printf("\nEnter Correct bet : ");
            scanf("%d", &userBet);
        } else {
            invalidBet = 0;
            printf("\n--------------------------------------------------\n");
            printf("Points bet : %d", userBet);
            printf("\n--------------------------------------------------\n");
        }
    } while (invalidBet == 1);
    
    int winAmount = betWin(userBet, multiplier);

    // Initial deal: two cards for each player
    for (int i = 0; i < 2; i++) {
        userCards[userCardCount++] = drawCard();
        compCards[compCardCount++] = drawCard();
    }

    userTotal = calculateTotal(userCards, userCardCount);
    compTotal = calculateTotal(compCards, compCardCount);

    printf("\nYour cards: ");
    printCards("User", userCards, userCardCount, userTotal);
    printf("\nComputer's first card: %d\n", compCards[0]);

    // Check for immediate Blackjack
    if (checkBlackjack(userTotal) || checkBlackjack(compTotal)) {
        if (userTotal == 21) {
            printf("\n--------------------------------------------------\n");
            printf("\nYou won with a Blackjack!\n");
            winPoints((winAmount*1.5));
        } else {
            printf("\n--------------------------------------------------\n");
            printf("\nComputer has a Blackjack, You lost!\n");
            losePoints(userBet);
        }
        return;
    }

    // User's turn
    do {
        //Prompting user whether they would like to hit or stand
        printf("\n--------------------------------------------------\n");
        printf("Would you like to hit? Type 'h' or 's': ");
        scanf(" %c", &userChoice);
        printf("--------------------------------------------------\n");

        if (userChoice == 'h') {
            userCards[userCardCount++] = drawCard(); 
            userTotal = calculateTotal(userCards, userCardCount);
            printCards("User", userCards, userCardCount, userTotal);

            if (userTotal > 21) {
                printf("\n--------------------------------------------------\n");
                printf("You went over 21. You lost!\n");
                losePoints(userBet);
                printf("--------------------------------------------------\n");
                return;
            }
        }
    } while (userChoice == 'h');

    // Computer's turn
    while (compTotal <= 16) {
        compCards[compCardCount++] = drawCard();
        compTotal = calculateTotal(compCards, compCardCount);
    }

    printCards("User", userCards, userCardCount, userTotal);
    printCards("Computer", compCards, compCardCount, compTotal);

    // Determining the winner
    if (compTotal > 21) {
        printf("\n--------------------------------------------------\n");
        printf("Computer went over 21. You won!\n");
        winPoints(winAmount);
        printf("--------------------------------------------------\n");
    } else if (userTotal > compTotal) {
        printf("\n--------------------------------------------------\n");
        printf("Your score is higher. You won!\n");
        winPoints(winAmount);
        printf("--------------------------------------------------\n");
    } else if (compTotal > userTotal) {
        printf("\n--------------------------------------------------\n");
        printf("Computer's score is higher. You lost.\n");
        losePoints(userBet);
        printf("--------------------------------------------------\n");
    } else {
        printf("\n--------------------------------------------------\n");
        printf("It's a draw.\n");
        losePoints((userBet/2)); //player loses only half the points
        printf("--------------------------------------------------\n");
    }
}

int main() {
    char playAgain;

    do {
        int playerLost = 0; //used for case checking

        blackjackGame();

        if (userPoints <= 0) {
            printf("\n--------------------------------------------------\n");
            printf("GAME OVER, YOU'RE OUT OF POINTS!");
            printf("\n--------------------------------------------------\n");
            playerLost = 1;
        }

        if (playerLost != 1) { //implies that game is not over yet
            printf("\nWould you like to play another round? Type 'y' or 'n': ");
            scanf(" %c", &playAgain);
            clearScreen();
        } else { //implies that the game is over and resets the user's points to 1000
            userPoints = 1000;
            printf("\nWould you like to play again? Type 'y' or 'n': "); //display a more appropriate message
            scanf(" %c", &playAgain);
            clearScreen();
        }

    } while (playAgain == 'y' && userPoints > 0);

    if (playAgain == 'n') { //only displays if player chooses to exit the game
        printf("\nExiting game... Goodbye!\n");
    }

    return 0;
}
