// ALF : arad96/macos-terminal-snake-game-c
// C program to build the complete snake game   
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

// TODO: Implement a hard mode where the game speeds up as the tail gets longer
// TODO: Once game over condition is hit give user option to play again.
// TODO: ability to do profile monitoring, performance monitoring
// TODO: add ability to keep a high score

int row, col, height = 20, width = 20; 
int gameover, score; 
int x, y;                       // current position
int tailX[100], tailY[100];     // memory for all tail segments 
int fruitx, fruity;             // fruit position
int nTail = 1;                    
int flag;                       // direction flag
char ch; 


// Function to generate the fruit within the boundary 
void setup() { 
    gameover = 0; 
    x = width / 2; 
    y = height / 2;

    srand(time(0));  // Seed the random number generator
    do {
        fruitx = rand() % (width - 1) + 1;  // generate fruit x,y so always inside boarders 
        fruity = rand() % (height - 1) + 1;   // and not on snakes head
    } while (fruitx == x && fruity == y);

    score = 0; 

} 


// Function to draw the boundaries 
void draw() { 
    
    // wclear(stdscr);     // clear window
    werase(stdscr);

    // print game board
    for (row = 0; row <= height; row++) { 
        for (col = 0; col <= width; col++) { 
            if (row == 0 || row == height || col == 0 || col == width) {
                // draw boarder 
                printw("#");
            } 
            else if (col == x && row == y){
                    // draw head
                    printw("0");
                }   
            else if (col == fruitx && row == fruity){
                    // draw fruit
                    printw("*");
                } 
            else {
                // check to see if cell is occupied by tail
                int print = 0;
                for(int k = 0; k < nTail; k++){
                    if(tailX[k] == col && tailY[k] == row){
                        // draw tail segments
                        printw("o");
                        print = 1;
                    }        
                }
                if (! print){
                    // not occupied
                    printw(" ");
                }
            }        
        } 
        printw("\n");
    } 
  
    // Print the score after the game ends 
    printw("Score = %d", score); 
    printw("\n"); 
    printw("press X to quit the game"); 
    printw("\n");
    refresh();          // render graphics

}


// Function to take the input 
void input() { 
    
    // Get the keyboard input
    int ch = getch();

    // case 97: Handles the 'a' key press.
    // case 115: Handles the 's' key press.
    // case 100: Handles the 'd' key press.
    // case 119: Handles the 'w' key press.
    // case 120: Handles the 'x' key press.
    
    // Check if a key was pressed
    if (ch != ERR) {
        
        // Get the character code of the key pressed
        int key = ch & 0xFF;

        // Check if the key pressed was a special key, such as an wasd key
        switch (key) { 
            case 'a':       // left
                flag = 1; 
                break; 
            case 's':       // down
                flag = 2; 
                break; 
            case 'd':       // right
                flag = 3; 
                break; 
            case 'w':       // up
                flag = 4; 
                break; 
            case 'x': 
                gameover = 1; 
                break; 
        } 
    }
} 
  

// Function for the logic behind each movement 
void logic() {

    if (nTail - 1 == 100){
        gameover = 1;
        printw("CongratulationS! You Ate %d Fruits!: !!YOU WIN!! :)", score / 10); 
        printw("\n");
        refresh();
        sleep(4);
        return;
    }
    
    // store head from previous iteration 
    int prevX = tailX[0];          
    int prevY = tailY[0];
    int prev2X, prev2Y;

    // update x, y based on input direction wasd
    switch (flag) {   
        case 1: 
            x--;    // Move left
            break; 
        case 2: 
            y++;    // Move down
            break; 
        case 3: 
            x++;    // Move right
            break; 
        case 4: 
            y--;    // Move up
            break; 
        default: 
            break; 
    } 

    // Update the position of the head in the tail arrays   
    tailX[0] = x;
    tailY[0] = y;

    // update position of tail segments
    for (int ix = 1; ix < nTail; ix++) {
        prev2X = tailX[ix];
        prev2Y = tailY[ix];
        tailX[ix] = prevX;
        tailY[ix] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
  
    // check boarder collision
    if (x < 1 || x > width - 1 || y < 1 || y > height - 1){ // (subtract 1 bc boarders)
        gameover = 1;
        printw("Boundary hit: GAME OVER");
        printw("\n");
        refresh();
        sleep(2);
        return;
    }

    // check self collision
    for(int k = 1; k < nTail; k++){
        if(tailX[k] == x && tailY[k] == y){
            gameover = 1;
            printw("Self hit: GAME OVER");
            printw("\n");
            refresh();
            sleep(3);
            return;
        }
    }

    // check for fruit collision  
    if (x == fruitx && y == fruity) {  
        
        int fruit_on_snake;

        // After eating the above fruit generate new fruit on non occupied space
        do {
            fruit_on_snake = 0;
            fruitx = rand() % (width - 1) + 1;  // generate fruit x,y so always inside boarders and non occupied spot 
            fruity = rand() % (height - 1) + 1;
            for(int k = 0; k < nTail; k++){
                if(tailX[k] == fruitx && tailY[k] == fruity){
                    fruit_on_snake = 1;
                    // printw("Fruit on Snake"); printw("\n");refresh();
                }        
            }
        } while (fruit_on_snake);

        nTail++;
        score += 10;

    } 
}


// Driver Code 
int main() { 
    
    // init screen, Enable keypad mode, unbuffer input   
    initscr();              // Start curses mode
    cbreak();               // Line buffering disabled
    noecho();               // Don't echo() while we do getch
    nodelay(stdscr, TRUE);  // Non-blocking input
    keypad(stdscr, TRUE); 
    timeout(300);           // Set getch to wait x ms for input

    // Generate boundary 
    setup(); 
  
    // Until the game is over 
    while (!gameover) { 
        draw(); 
        input(); 
        logic();
    }

    // Disable keypad mode End curses mode
    keypad(stdscr, FALSE); 
    endwin();

return 0;

} 