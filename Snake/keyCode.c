#include <detpic32.h>

int main() {
        // while (1) {
        //         printf("%d\n", getChar());
        // }

        int i = 0;
        for (i = 0; i < 256; i++) {
                putChar(i);
        }

        return 0;
}
