const int digits[10][5][5] = {
    {{1, 1, 1, 1, 1},
     {1, 0, 0, 0, 1},
     {1, 0, 0, 0, 1},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 1}},
    {{0, 0, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0}},
    // digits 2
    {{1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {1, 1, 1, 1, 1},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 1, 1}},
    // Digit 3
    {{1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {1, 1, 1, 1, 1}},
    // Digit 4
    {{1, 0, 0, 0, 1},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {0, 0, 0, 0, 1}},
    // Digit 5
    {{1, 1, 1, 1, 1},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {1, 1, 1, 1, 1}},

    // Digit 6
    {{1, 1, 1, 1, 1},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 1}},

    {{1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {0, 0, 0, 0, 1},
     {0, 0, 0, 0, 1},
     {0, 0, 0, 0, 1}},
    // Digit 8
    {{1, 1, 1, 1, 1},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 1},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 1}},
    // Digit 9
    {{1, 1, 1, 1, 1},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 0, 1},
     {1, 1, 1, 1, 1}},

    // Add representations for digits 2 to 9 similarly
};

const int character[14][5][5] = {
   {{1, 1, 1, 1, 1},  // S
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}},

    {{1, 1, 1, 1, 1},  // C
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}},
    // O
    {{0, 1, 1, 1, 0},
     {1, 0, 0, 0, 1},
     {1, 0, 0, 0, 1},
     {1, 0, 0, 0, 1},
     {0, 1, 1, 1, 0}},

    // R
    {{1, 1, 1, 1, 0},
     {1, 0, 0, 0, 1},
     {1, 1, 1, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 0, 1}},

    // E
    {{1, 1, 1, 1, 1},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 1, 1},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 1, 1}},


    {{1, 0, 0, 0, 0},  // L
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}},

    {{1, 1, 1, 1, 1},  // I
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1}},

    {{1, 0, 0, 0, 1},  // V
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0}},

    {{1, 1, 1, 1, 1},  // E
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1}},


    {{1, 1, 1, 1, 1},  // T
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0}},


    {{1, 1, 1, 1, 1},  // m
    {1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1}},


};

void drawCharacter(int m, int x, int y, int scale, unsigned int color) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (character[m][i][j] == 1) {
                for (int k = 0; k < scale; k++) {
                    for (int l = 0; l < scale; l++) {
                        drawPixel(x + j * scale + l, y + i * scale + k, color);
                    }
                }
            }
        }
    }
}

// Function to draw a digit at position (x, y) with given scale
void drawDigit(int digit, int x, int y, int scale, unsigned int color) {
    if (digit < 0 || digit > 9) return; // Only handle digits 0 to 9
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (digits[digit][i][j] == 1) {
                for (int k = 0; k < scale; k++) {
                    for (int l = 0; l < scale; l++) {
                        drawPixel(x + j * scale + l, y + i * scale + k, color);
                    }
                }
            }
        }
    }
}

// Function to draw a number at position (x, y) with given scale
void drawNumber(int number, int x, int y, int scale, unsigned int color) {
    if (number < 0) return;
    if (number == 0) {
        drawDigit(0, x, y, scale, color);
        return;
    }
    int posX = x;
    while (number > 0) {
        int digit = number % 10;
        drawDigit(digit, posX, y, scale, color);
        number /= 10;
        posX -= (6 * scale); // Space between digits
    }
}
// Function to draw a string at position (x, y) with given scale
void drawString(const char* str, int x, int y, int scale, unsigned int color) {
    int len = 0;
    // Calculate the length of the string manually
    while (str[len] != '\0') {
        len++;
    }

    int posX = x;

    // Loop through each character in the string
    for (int i = 0; i < len; i++) {
        char ch = str[i];
        int m;

        switch(ch) {
            case 'S':
                m = 0;
                break;
            case 'C':
                m = 1;
                break;
            case 'O':
                m = 2;
                break;
            case 'R':
                m = 3 ;
                break;
            case 'E':
                m = 4;
                break;
            case 'L':
                m = 5;
                break;
            case 'I':
                m = 6;
                break;
            case 'V':
                m = 7;
                break;
            case 'T':
                m = 9;
                break;
            case 'M':
                m = 10;
                break;
            default:
                // Handle other characters or leave blank for empty space
                break;
        }

        if (m != 29) {
            drawCharacter(m, posX, y, scale, color);
        }

        posX += (6 * scale); // Space between characters
    }
}