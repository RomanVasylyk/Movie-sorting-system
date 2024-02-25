#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Uloha 1
typedef struct {
    char *nazov;
    int rokVysielania;
    int pocetSerialov;
    int pocetDielov;
} TSerial;

int pocetSerialov;

TSerial* nacitajSerialy(FILE *subor) {
    fscanf(subor, "%d", &pocetSerialov);

    TSerial *seriale = (TSerial*)malloc(pocetSerialov * sizeof(TSerial));

    for (int i = 0; i < pocetSerialov; i++) {
        seriale[i].nazov = (char*)malloc(256);
        fscanf(subor, "%s %d %d %d", seriale[i].nazov, &seriale[i].rokVysielania, &seriale[i].pocetSerialov, &seriale[i].pocetDielov);
    }

    return seriale;
}

void vypisSerialy(TSerial *seriale) {
    printf("Načítané údaje o seriáloch:\n");
    for (int i = 0; i < pocetSerialov; i++) {
        printf("Názov: %s, Rok vysielania: %d, Počet sérií: %d, Počet dielov: %d\n",
               seriale[i].nazov, seriale[i].rokVysielania, seriale[i].pocetSerialov, seriale[i].pocetDielov);
    }
}

// Uloha 2
int porovnajSerialy(const void *a, const void *b) {
    int rokPorovnanie = ((TSerial*)a)->rokVysielania - ((TSerial*)b)->rokVysielania;

    if (rokPorovnanie != 0) {
        return rokPorovnanie;
    }

    return ((TSerial*)a)->pocetDielov - ((TSerial*)b)->pocetDielov;
}

// Uloha 3
int compareYears(const void *targetYear, const void *serial) {
    int year = *((int *)targetYear);
    int serialYear = ((TSerial *)serial)->rokVysielania;

    return (year - serialYear);
}

void searchAndListSeriesByYear(TSerial *seriale, int targetYear) {
    TSerial *result = (TSerial *)bsearch(&targetYear, seriale, pocetSerialov, sizeof(TSerial), compareYears);

    if (result == NULL) {
        printf("No series found for the year %d\n", targetYear);
    } else {
        printf("Series created in %d:\n", targetYear);
        while (result->rokVysielania == targetYear) {
            printf("%s\n", result->nazov);
            result++;
        }
    }
}

// Uloha 4
void writeSerialsToFileBeforeYear(TSerial *seriale, const char *outputFileName, char startLetter) {
    FILE *outputFile = fopen(outputFileName, "w");

    if (outputFile == NULL) {
        fprintf(stderr, "Chyba pri otváraní výstupného súboru.\n");
        return;
    }

    fprintf(outputFile, "Serials with names starting with '%c' created before the year 2000:\n", startLetter);

    for (int i = 0; i < pocetSerialov; i++) {
        if (seriale[i].nazov[0] == startLetter && seriale[i].rokVysielania < 2000) {
            fprintf(outputFile, " %s %d %d %d\n",
                    seriale[i].nazov, seriale[i].rokVysielania, seriale[i].pocetSerialov, seriale[i].pocetDielov);
        }
    }

    fclose(outputFile);

    printf("Serials with names starting with '%c' created before the year 2000 written to %s\n", startLetter, outputFileName);
}



// Hlavná funkcia
int main() {
    int fileChoice;

    printf("Choose a file to work with:\n");
    printf("1. serialy.txt\n");
    printf("2. serialy1.txt\n");
    printf("3. serialy2.txt\n");
    printf("4. serialy3.txt\n");
    printf("Enter your choice: ");

    scanf("%d", &fileChoice);

    char fileName[256];

    switch (fileChoice) {
        case 1:
            strcpy(fileName, "serialy.txt");
            break;
        case 2:
            strcpy(fileName, "serialy1.txt");
            break;
        case 3:
            strcpy(fileName, "serialy2.txt");
            break;
        case 4:
            strcpy(fileName, "serialy3.txt");
            break;
        default:
            printf("Invalid choice. Exiting.\n");
            return 0;
    }

    FILE *subor = fopen(fileName, "r");

    if (subor == NULL) {
        fprintf(stderr, "Chyba pri otváraní súboru.\n");
        return 1;
    }

    TSerial *seriale = nacitajSerialy(subor);

    fclose(subor);

    // Uloha 2
    qsort(seriale, pocetSerialov, sizeof(TSerial), porovnajSerialy);

    // Uloha 1
    vypisSerialy(seriale);

    // Uloha 3
    int targetYear;
    printf("Enter the year to search for: ");
    scanf("%d", &targetYear);

    searchAndListSeriesByYear(seriale, targetYear);

    // Uloha 4
    char outputFileName[256];
    char startLetter;

    printf("Enter the output file name: ");
    scanf("%s", outputFileName);

    printf("Enter the starting letter: ");
    scanf(" %c", &startLetter);


    writeSerialsToFileBeforeYear(seriale, outputFileName, startLetter);

    // Uvoľnenie pamäti
    free(seriale);

    return 0;
}
