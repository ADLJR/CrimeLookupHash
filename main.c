//
// Chicago Crime Lookup via hashing.
//
// <<ANTWAN LOVE>>
// <<VISUAL STUDIOS/ CODEBLOCKS>>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project 5
//

// ignore stdlib warnings if working in Visual Studio:
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef char *HTKey;
static int collisions = 0;

// struct to store crimes.csv info
typedef struct Crimes {
	char Case[32];		// storing case number
	char Code[32];		// stporing IUCR code
	char Calendar[32];	// storing date and time
	int  Beat;			// police beat **CAN BE IGNORED
	int  District;		// voting district **CAN BE IGNORED
	int  Ward;			// city ward **CAN BE IGNORED
	char Arrest[32];	// T/F if arrest was made
} Crimes;

// struct to store area.csv info
typedef struct Areas {
	int  Number;		// number for community on list
	char Community[32];	// name of community
} Areas;

// hash the case to create a unique index
int hash(HTKey key, int size) {
	int hashValue = 0;
	int numKey = atoi(key+4);
	int areaKey = atoi(key+1);	// area of the ciry
	int letter = key[strlen(key) - 4];
	//letter = letter - 'A';
	//numKey = numKey + letter;
	char *p = key;

	while (*p != '\0') {
		hashValue = areaKey + numKey + (hashValue * 31) + *p;
		p++;
	}

	hashValue = abs(hashValue);

	return hashValue % size;
}

// used insert with linear probing function via Zyante 3.3.4
void HashInsert(Crimes **base, Crimes* temp, int size) {
	int index = hash(temp->Case, size);
	int counter = 0;

	// where you left off
	while (counter < size) {
		if (base[index] == NULL) {
			base[index] = temp;
			break;
		}

		// increment index
		index = (index + 1) % size;

		// increment collision
		collisions++;
		// increment counter
		++counter;
	}
}

// search plateshash
int searchHash(Crimes **crimesHash, char* userInput, int size) {
	int index = hash(userInput, size);
	int counter = -1;
	int play = 1;
	int i = 0;

	for (i = index; i < size; i++) {
		if (crimesHash[i]->Case == NULL) {
			// show that the plate to search is unavailable
			break;
			// return -1;
		}
		else if (strcmp(crimesHash[i]->Case, userInput) == 0) {
			counter = i;
		}
	}

	return counter;
}


void buildTableC(Crimes **CrimeTable, char *filename, int size, int* sizeTotal) {
	Crimes *temp;		// temp struct to store all tokens
	char *token;		// store everything in file
	int i = 0;			// counter variable 1
	int j = 0;			// counter variable 2

	FILE *ReadFile = fopen(filename, "r");	// open file to be read

	char line[256];
	int linesize = sizeof(line) / sizeof(line[0]);

	// getting value for crimes
	if (fgets(line, linesize, ReadFile) != '\0') {
		// parsing tokens
		while (fgets(line, linesize, ReadFile) != '\0') {
			line[strcspn(line, "\r\n")] = '\0';
			temp = (Crimes *)malloc(sizeof(Crimes));

			token = strtok(line, ",");
			strcpy(temp->Case, token);		// storing Case Number

			token = strtok(NULL, ",");
			strcpy(temp->Code, token);		// storing IUCR code

			token = strtok(NULL, ",");
			strcpy(temp->Calendar, token);	// storing Date and Time

											// storing Beat, District, and Ward
			token = strtok(NULL, ","); // get next token
			temp->Beat = atoi(token);
			token = strtok(NULL, ","); // get next token
			temp->District = atoi(token);
			token = strtok(NULL, ","); // get next token
			temp->Ward = atoi(token);

			token = strtok(NULL, ",");
			strcpy(temp->Arrest, token);		// storing Arrest boolean

			(*sizeTotal)++;
			HashInsert(CrimeTable, temp, size);	// insert temp into hash table
		}
	}

	fclose(ReadFile);       // close file
}

// setting each value to null/ "empty-at-first"
// template from previous homework
Crimes **HashArray(int Sz) {
	Crimes **A = (Crimes **)malloc(Sz * sizeof(Crimes *));

	int i = 0;
	for (i = 0; i < Sz; ++i) {
		A[i] = NULL;
	}

	return A;
}

// create Areas struct
Areas *AreaStruct(char* filename) {
	Areas *A = (Areas *)malloc(78 * sizeof(Areas));

	char *token;		// store everything in file
	int i = 0;			// counter variable 1
	int j = 0;			// counter variable 2

	FILE *ReadFile = fopen(filename, "r");	// open file to be read

	char line[256];
	int linesize = sizeof(line) / sizeof(line[0]);

	// getting value for crimes
	if (fgets(line, linesize, ReadFile) != '\0') {
		// parsing tokens
		while (fgets(line, linesize, ReadFile) != '\0') {
			line[strcspn(line, "\r\n")] = '\0';
			//A = (Areas *)malloc(sizeof(Areas));

			token = strtok(line, ","); // get next token
			A[i].Number = atoi(token);

			token = strtok(NULL, ",");
			strcpy(A[i].Community, token);		// storing Arrest boolean

			i++;
		}
	}

	fclose(ReadFile);       // close file

	return A;
}

// getFileName: [TEMPLATE FROM PREVIOUS PROJECT]
// Inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be
// opened, an error message is output and the program is exited.
char *getFileName() {
	char filename[512];
	int  fnsize = sizeof(filename) / sizeof(filename[0]);

	// input filename from the keyboard:
	fgets(filename, fnsize, stdin);
	filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

												 // make sure filename exists and can be opened:
	FILE *infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("**Error: unable to open '%s'\n\n", filename);
		exit(-1);
	}

	fclose(infile);

	// duplicate and return filename:
	char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(s, filename);

	return s;
}

// OBTAIN SIZE OF FILENAME
long getFileSizeInBytes(char *filename) {
	FILE *file = fopen(filename, "r");
	if (file == NULL) {
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fclose(file);
	return size;
}

// MAIN FUNCTION ----
int main() {
	// Header
	printf("** Chicago Crime Lookup **\n");
	// get names of files
	char *CrimesFileName = getFileName();
	char *AreasFileName = getFileName();

	// size of each filename
	int bytesC = getFileSizeInBytes(CrimesFileName);
	// size for hash tables
	int sizeC = (bytesC / 50) * 10;
	int sizeTotal = 0;

	Crimes **CrimesTable = HashArray(sizeC);
	buildTableC(CrimesTable, CrimesFileName, sizeC, &sizeTotal);

	Areas *AreasBody = AreaStruct(AreasFileName);

	// OUTPUT
	printf(">> Crimes file: %d bytes\n", bytesC);
	printf(">> # crimes: %d\n", sizeTotal);
	printf(">> HT size: %d\n", sizeC);
	printf(">> Collisions: %d\n\n", collisions);

	// prompt user for index
	char userCase[80];
	int i = 0;
	int j = 0;
	int numCase = 0;
	int element = 0;
	int areaC = 0;

	printf("Enter a case number> ");
	fgets(userCase, 32, stdin);  // input user input
	userCase[strcspn(userCase, "\r\n")] = '\0';  // strip EOL(s) char at end:

												 // searching for case
	while (strlen(userCase) > 0) {
		if (strlen(userCase) == 8) {
			numCase = hash(userCase, sizeC);
			element = searchHash(CrimesTable, userCase, sizeC);
			if (element != -1) {
				areaC = atoi(CrimesTable[element]->Case + 1);
				for (i = 0; i < 78; i++) {
					if (areaC == AreasBody[i].Number) {
						j = i;
					}
				}
				printf(">> hash index: %d <<\n", numCase);
				printf("%s:\n", userCase);
				// print output
				printf("   date/time: %s\n", CrimesTable[element]->Calendar);
				printf("   city area: %d => %s\n", AreasBody[j].Number
					, AreasBody[j].Community);
				printf("   IUCR code: %d\n", atoi(CrimesTable[element]->Code));
				if (strcmp(CrimesTable[element]->Arrest, "T") == 0) {
					printf("   arrested: true \n\n");
				}
				else {
					printf("   arrested: false \n\n");
				}
			}
			else {
				printf(">> hash index: %d <<\n", numCase);
				printf("** Case not found...\n\n");
			}
		}
		else {
			printf("** invalid case #, try again...\n\n");
		}

		// prompt user again
		printf("Enter a case number> ");
		fgets(userCase, 32, stdin);  // input user input
		userCase[strcspn(userCase, "\r\n")] = '\0';  // strip EOL(s) char at end:
	}

	// finished
	printf("\n** Done **");

	return 0;
}
