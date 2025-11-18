#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_SENTENCES 9

char *sentence[NUM_SENTENCES] = {
    "Just staying one day ahead of yesterday is true win for an individual",
    "World of programming is both fascinating and confusing at the same time",
    "Pack my lunch box with five additional water bottles for my friends",
    "My university is one of oldest engineering university of Pakistan",
    "A quick brown fox jumps over the lazy dog",
    "Learning the home row keys is essential for becoming a touch typist",
    "Proper posture and hand alignment are critical for long term comfort",
    "Self care is an important thing which we usually ignore",
    "Karachi is Pakistan's largest city with population more than Northern Cyprus"
};

// -------------------- EDIT DISTANCE (BEST ACCURACY) --------------------

int min(int a, int b) {
    return (a < b) ? a : b;
}

int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

// Levenshtein Edit Distance Algorithm
int editDistance(const char *original, const char *typed) {
    int len1 = strlen(original);
    int len2 = strlen(typed);

    int dp[len1 + 1][len2 + 1];

    // Initialize the first row and column
    for (int i = 0; i <= len1; i++)
        dp[i][0] = i;

    for (int j = 0; j <= len2; j++)
        dp[0][j] = j;

    // Fill DP Table
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (original[i - 1] == typed[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];  // same char ? no penalty
            } else {
                dp[i][j] = 1 + min3(
                    dp[i - 1][j],     // deletion
                    dp[i][j - 1],     // insertion
                    dp[i - 1][j - 1]  // substitution
                );
            }
        }
    }

    return dp[len1][len2];
}

// ------------------------ TIMER FUNCTION ------------------------

double getTimeTaken(char *typed) {
    clock_t start, end;

    printf("\n\t\t\t\tStart typing below\n\t");

    start = clock();
    fgets(typed, 200, stdin);
    end = clock();

    // Remove newline from typed string
    typed[strcspn(typed, "\n")] = '\0';

    return (double)(end - start) / CLOCKS_PER_SEC;
}

// ----------------------------- MAIN -----------------------------

int main() {
    srand(time(NULL));
    int index = rand() % NUM_SENTENCES;

    char typed[200];
    double timeTaken;
    int errors;

    printf("\n\t\t\tWelcome to the TYPING SPEED TEST!\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("\n\tType the following sentence:\n\n\t\"%s\"\n\n", sentence[index]);

    timeTaken = getTimeTaken(typed);
    if (timeTaken <= 0) timeTaken = 0.01;  // avoid divide-by-zero

    // Best accuracy: edit distance
    errors = editDistance(sentence[index], typed);

    int totalCharacters = strlen(sentence[index]);
    double accuracy = ((double)(totalCharacters - errors) / totalCharacters) * 100;
    if (accuracy < 0) accuracy = 0;  // in case errors > total characters

    double words = totalCharacters / 5.0;
    double wpm = (words / timeTaken) * 60;

    // --------------------- RESULTS ---------------------

    printf("\n\t\t\tRESULTS\n");
    printf("-------------------------------------------------------------------------------\n\n");
    printf("  Time Taken   : %.2f seconds\n", timeTaken);
    printf("  Speed        : %.2f words per minute\n", wpm);
    printf("  Accuracy     : %.2f%%\n", accuracy);
    printf("  Errors       : %d (using edit-distance: highest accuracy)\n", errors);
    printf("\n-------------------------------------------------------------------------------\n");

    return 0;
}
