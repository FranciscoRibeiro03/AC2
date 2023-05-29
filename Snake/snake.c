#include <stdlib.h>
#include <string.h>
#include <detpic32.h>

#define VECTOR_UART2 32
#define VECTOR_ADC 27

#define keycode_ESC 27
#define keycode_UP 119
#define keycode_DOWN 115
#define keycode_LEFT 97
#define keycode_RIGHT 100

#define board_length 60
#define board_height 20

struct coordinate
{
    int x;
    int y;
    int direction;
};

typedef struct coordinate coordinate;

volatile coordinate head, bend[500], food;
volatile int score, length;

volatile int speedIdx = 0;

void showCursor() { printStr("\e[?25h"); }
void hideCursor() { printStr("\e[?25l"); }

void delay(unsigned int ms) {
        resetCoreTimer();
        while (readCoreTimer() < 20000 * ms);
}

char* getDirectionAsString() {
        switch (head.direction) {
                case keycode_UP:
                        return "UP";
                case keycode_DOWN:
                        return "DOWN";
                case keycode_LEFT:
                        return "LEFT";
                case keycode_RIGHT:
                        return "RIGHT";
                default:
                        return "UNKNOWN";
        }
}

void configureUart2() {
        // Configure UART2: 115200, N, 8, 1
        U2BRG = ((PBCLK + 8 * 115200) / (16 * 115200)) - 1;
        U2MODEbits.BRGH = 0;
        U2MODEbits.PDSEL = 0; // 8 bits sem paridade
        U2MODEbits.STSEL = 0; // 1 stop bit
        U2STAbits.UTXEN = 1; // Enable do módulo de transmissão
        U2STAbits.URXEN = 1; // Enable do módulo de receção
        U2MODEbits.ON = 1;

        // Configure UART2 interrupts, with RX interruprs enabled and TX interrupts disabled
        IEC1bits.U2RXIE = 1;
        IEC1bits.U2TXIE = 0;
        IPC8bits.U2IP = 1;
        IFS1bits.U2RXIF = 0;
        U2STAbits.URXISEL = 0;
}

void configureADC() {
        TRISBbits.TRISB4 = 1;
        AD1PCFGbits.PCFG4 = 0;
        AD1CON1bits.SSRC = 7;
        AD1CON1bits.CLRASAM = 1;
        AD1CON3bits.SAMC = 16;
        AD1CON2bits.SMPI = 7;
        AD1CHSbits.CH0SA = 4;
        AD1CON1bits.ON = 1;

        // Configure ADC interrupts
        IPC6bits.AD1IP = 2;
        IEC1bits.AD1IE = 1;
        IFS1bits.AD1IF = 0;
}

void clearFullScreen() {
        printStr("\e[H\e[J");
}

void moveCursorToPosition(unsigned int line, unsigned int column) {
        printf("\e[%d;%dH", line, column);
}

int initText() {
        moveCursorToPosition(10,12);
        printf("\tWelcome to the pic32 Snake game. Press any key to continue.\n");
        getChar();
        clearFullScreen();
        printf("\tGame instructions:\n");
        printf("\n-> Use WASD to move the snake.\n-> You will be provided foods at several coordinates of the screen which you have to eat. Everytime you eat a food the length of the snake will be increased by 1 element and thus the score.\n-> Here you are provided with three lives. Your life will decrease as you hit the wall or snake's body.\n-> You can pause the game by pressing any key. To continue the paused game press any key once again\n-> If you want to exit press ESC.\n");
        printf("\nPress any key to play game...");
        if (getChar() == keycode_ESC)
                exit(0);
        return 1;
}

void drawBorder() {
        int i;
        for (i = 10; i < board_height + 12; i++) {
                moveCursorToPosition(i, 10);
                putChar('#');
                moveCursorToPosition(i, board_length + 11);
                putChar('#');
        }
        for (i = 10; i < board_length + 12; i++) {
                moveCursorToPosition(10, i);
                putChar('#');
                moveCursorToPosition(board_height + 11, i);
                putChar('#');
        }
}

void generateFood() {
        food.x = (rand() % board_length) + 11;
        food.y = (rand() % board_height) + 11;
        for (int i = 0; i < length; i++) {
                if (bend[i].x == food.x && bend[i].y == food.y) {
                        generateFood();
                }
        }
        moveCursorToPosition(food.y, food.x);
        printStr("\e[1;30;41mF\e[0m");
}

void checkFood() {
        if (head.x == food.x && head.y == food.y) {
                length++;
                generateFood();
        }
}

void initGame() {
        length = 5;
        head.x = 25;
        head.y = 20;
        score = 0;
        head.direction = keycode_RIGHT;
        drawBorder();
        generateFood();
        checkFood();
}

void gameLoop() {
        int i;
        int x = 0, y = 0, prevx = 0, prevy = 0;
        for (i = 0; i < length; i++) {
                x = bend[i].x;
                y = bend[i].y;
                moveCursorToPosition(y, x);
                printStr("\e[1;33;42m \e[0m");
        }
        moveCursorToPosition(bend[length - 1].y, bend[length - 1].x);
        putChar(' ');
        switch (head.direction) {
                case keycode_UP:
                        head.y--;
                        break;
                case keycode_DOWN:
                        head.y++;
                        break;
                case keycode_LEFT:
                        head.x--;
                        break;
                case keycode_RIGHT:
                        head.x++;
                        break;
                default:
                        break;
        }
        if (head.x == 10 || head.x == board_length + 11 || head.y == 10 || head.y == board_height + 11) {
                clearFullScreen();
                printf("You lost! Your score was %d\n", score);
                exit(0);
        }
        for (i = 0; i < length; i++) {
                if (head.x == bend[i].x && head.y == bend[i].y) {
                        clearFullScreen();
                        printf("You lost! Your score was %d\n", score);
                        exit(0);
                }
        }
        if (head.x == food.x && head.y == food.y) {
                length++;
                score++;
                generateFood();
        }
        moveCursorToPosition(head.y, head.x);
        printStr("\e[1;30;42mO\e[0m");
        for (i = 0; i < length; i++) {
                prevx = bend[i].x;
                prevy = bend[i].y;
                bend[i].x = x;
                bend[i].y = y;
                x = prevx;
                y = prevy;
        }
        bend[0] = head;
}

int main() {
        configureUart2();
        configureADC();
        EnableInterrupts();
        AD1CON1bits.ASAM = 1;

        speedIdx = 0;
        int speeds[] = { 400, 350, 300, 250, 200, 150, 100, 50 };

        clearFullScreen();
        printStr("Press any key to start the game...");
        while (inkey() == '\0');
        srand(readCoreTimer());

        clearFullScreen();

        initGame();
        bend[0] = head;

        moveCursorToPosition(0, 0);

        while (1) {
                hideCursor();
                gameLoop();
                moveCursorToPosition(1, 0);
                printf("\e[K");
                moveCursorToPosition(1, 0);
                printf("Direction: %s\n", getDirectionAsString());
                moveCursorToPosition(2, 0);
                printf("\e[K");
                moveCursorToPosition(2, 0);
                printf("Score: %d\n", score);
                moveCursorToPosition(3, 0);
                printf("\e[K");
                moveCursorToPosition(3, 0);
                printf("Speed: %d\n", speedIdx);
                showCursor();
                delay(speeds[speedIdx]);
        }

        moveCursorToPosition(board_height + 15, 0);
        return 0;
}

void _int_(VECTOR_UART2) isr_uart2(void) {
        if (IFS1bits.U2RXIF == 1) {
                char c = U2RXREG;
                if (c == keycode_UP || c == keycode_DOWN || c == keycode_LEFT || c == keycode_RIGHT) 
                        head.direction = c;
                if (c == keycode_ESC) {
                        clearFullScreen();
                        exit(0);
                }
                IFS1bits.U2RXIF = 0;
        }
}

void _int_(VECTOR_ADC) isr_adc(void) {
        int *p = (int *)(&ADC1BUF0);
        int i;
        int valor = 0;
        for (i = 0; i < 8; i++) {
                valor += p[i * 4];
        }
        valor = valor / 8;
        speedIdx = ( (valor * 7) + 511 ) / 1023;
        IFS1bits.AD1IF = 0;
        AD1CON1bits.ASAM = 1;
}
