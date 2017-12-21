//
// Chicago Crime Lookup via hashing.
//
// <<Zakee Jabbar (zjabba2)>>
// <<Windows || CodeBlocks>>
// U. of Illinois, Chicago
// CS251, Spring 2017
// Project #05
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//struct for storing crimes
typedef struct Crime
{
    char *Case;
    char *IUCR;
    char *DT;
    int City;
    int arrested;
}Crime;
//
// getFileName:
//
// Inputs a filename from the keyboard, make sure the file can be
// opened, and returns the filename if so.  If the file cannot be
// opened, an error message is output and the program is exited.
//
char *getFileName()
{
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

long getFileSizeInBytes(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return -1;
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

//
// Hash function, taking case # as key and hash table size,
// returns hash table index to start probing from:
//
int hash(char *key, int N)
{

  int hashvalue = 0;
  int b = 37;

  char *p = key;
  p++;
  int buck = N / 8;
  int area = atoi(p);
  if(area < 10)
  {
      hashvalue = 0;
  }
  if(area >= 10 && area < 20)
  {
      hashvalue += buck;
  }
  if(area >= 20 && area < 30)
  {
      hashvalue += (buck * 2);
  }
  if(area >= 30 && area < 40)
  {
      hashvalue += (buck * 3);
  }
  if(area >= 40 && area < 50)
  {
      hashvalue += (buck * 4);
  }
  if(area >= 50 && area < 60)
  {
      hashvalue += (buck * 5);
  }
  if(area >= 60 && area < 70)
  {
      hashvalue += (buck * 6);
  }
  if(area >= 70 && area < 78)
  {
      hashvalue += (buck * 7);
  }
  while (*p != '\0')
  {
     hashvalue = (hashvalue * b) + *p;
     p++;
  }

  hashvalue = abs(hashvalue);

  return hashvalue % N;

}


//
// searches for given case #, returns pointer to Crime struct
// if found, otherwise NULL is returned.  We are using linear
// probing, so hash function yields index of where to start our search.
//
Crime *search(Crime *hashtable[], int N, char *caseNo)
{
   int i = hash(caseNo, N);
   //printf(">>hash: %d\n", i);

   while (1)  // true => until a free spot or match is found:
   {
      if (hashtable[i] == NULL)  // empty, license plate not present:
      {
         return NULL;
      }
      else if (strcmp(hashtable[i]->Case, caseNo) == 0) // match!
      {
         return hashtable[i];
      }
      else  // advance i to probe next spot:
      {
         i++;

         if (i == N)  // end of hash table?  Wrap around...
           i = 0;
      }
   }

   return NULL; //not found
}


//
// Given all the data stores the Crime in the array.  We are using linear probing,
// so after we hash the case #, we start to probe --- starting at that index --- to look for a place
// to insert the new crime
//
int numCollisions = 0;
void store(Crime *hashtable[], int N, char *caseNo, char* IUCR, char* DT, int arrested, int city)
{
   int i = hash(caseNo, N);

   if(hashtable[i] != NULL)
   {
       numCollisions++;
   }

   //printf(">>hash: %d\n", i);

   //
   // we are using linear probing, so starting at position i we
   // probe for first free spot or a match...
   //
   while (1)  // true => until a free spot or match is found:
   {
      if (hashtable[i] == NULL)  // free spot:
      {
         hashtable[i] = (Crime *)malloc( sizeof(Crime) );

         hashtable[i]->Case = (char *)malloc(strlen(caseNo) + 1);
         hashtable[i]->IUCR = (char *)malloc(strlen(IUCR) + 1);
         hashtable[i]->DT = (char *)malloc(strlen(DT) + 1);

         strcpy(hashtable[i]->Case, caseNo);
         strcpy(hashtable[i]->IUCR, IUCR);
         strcpy(hashtable[i]->DT, DT);
         hashtable[i]->arrested = arrested;
         hashtable[i]->City = city;
         break;
      }
      else  // advance i to probe next spot:
      {
         i++;

         if (i == N)  // end of hash table?  Wrap around...
           i = 0;
      }
   }

   return;
}


//
// Given a hash table, initializes the table entries to empty (NULL ptrs) and
// then reads in the lines from the file.  Each line is tokened and then passed onto the
// store function which then stores it into the table. Once its stored into the table the
// count of number of crimes is incremented and that value is returned
//
int buildHashTable(Crime *hashtable[], int N, char* fileName)
{
  FILE *Crimes = fopen(fileName, "r");
  char *token;
  char *token2;
  char *token3;
  char *token4;
  char *token5;
  char *token6;
  char *token7;
  int i;
  int totCrimes = 0;
  // initialize to "empty":
  for (i = 0; i < N; ++i)
    hashtable[i] = NULL;

  char  line[512];
  int   linesize = sizeof(line) / sizeof(line[0]);
  fgets(line, linesize, Crimes);

  fgets(line, linesize, Crimes);  // input a line
  line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

  while (!feof(Crimes))
  {
     token = strtok(line, ",");
     token2 = strtok(NULL, ",");
     token3 = strtok(NULL, ",");     // gets all the required info
     token4 = strtok(NULL, ",");
     token5 = strtok(NULL, ",");
     token6 = strtok(NULL, ",");
     token7 = strtok(NULL, ",");

     int arrested = 1;

     if(token7[0] == 'F')
     {
         arrested = 0;
     }

     char *cityCode = token;
     cityCode++;

     int city = atoi(cityCode);

     store(hashtable, N, token, token2, token3, arrested, city);

     fgets(line, linesize, Crimes);  // input a line
     line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:
     totCrimes++;
  }

  fclose(Crimes);
  return totCrimes;
}

// build the Areas Hashtable to be used to look up the area based on
// the number stored inside the Crime struct
void buildAreasTable(char *areas[], char* fileName)
{
  FILE *Areas = fopen(fileName, "r");
  char *token;
  char *token2;

  char  line[512];
  int   linesize = sizeof(line) / sizeof(line[0]);
  fgets(line, linesize, Areas);

  fgets(line, linesize, Areas);  // input a line
  line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

  while (!feof(Areas))  // while not "#":
  {
      token = strtok(line, ",");
      int num = atoi(token);
      token2 = strtok(NULL, ",");

      areas[num] = (char *)malloc(strlen(token2) + 1);
      strcpy(areas[num], token2);

      fgets(line, linesize, Areas);  // input a line
      line[strcspn(line, "\r\n")] = '\0';  // strip EOL(s) char at end:

  }
  fclose(Areas);

}

// checks if the user input is proper and follows
// the right format
int isProper(char *input)
{
    char *p = input;
    p++;
    int cityCode = atoi(p);
    if(strlen(input) < 2)
    {
        if(input[0] == ' ')
        {
            return -1;
        }
    }
    if(strlen(input) != 8)
    {
        return 0;
    }
    if(cityCode > 77)
    {
        return 0;
    }
    if(input[0] != 'R')
    {
        return 0;
    }
    if(!isalpha(input[3]))
    {
        return 0;
    }
    if(!isdigit(input[1]) || !isdigit(input[2]) || !isdigit(input[4])
       || !isdigit(input[5]) || !isdigit(input[6]) || !isdigit(input[7]))
    {
        return 0;
    }


    return 1;
}

// processes the user input and prints out the info required or prints out
// and alternate message if incorrect input was entered
void processInput(int N, Crime *hashtable[], char *Areas[])
{
    char input[64];
    printf("Enter a case number>  ");
    char *token;
    fgets(input, 64, stdin);
    input[strcspn(input, "\r\n")] = '\0';  // strip EOL(s) char at end:
    token = strtok(input, " ");
    while(input[0] != '\0')
    {
        if(isProper(token) == 0)
        {
            printf("** invalid case #, try again...\n\n");
            printf("Enter a case number>  ");
            fgets(input, 64, stdin);
            input[strcspn(input, "\r\n")] = '\0';  // strip EOL(s) char at end:
            token = strtok(input, ",");
            continue;
        }
        if(isProper(token) == -1)
        {
            break;
        }

        printf(">> hash index: %d <<\n", hash(token, N));
        Crime *ans = search(hashtable, N, token);

        if(ans == NULL)
        {
            printf("** Case not found...\n\n");
        }
        else
        {
            printf("%s:\n", ans->Case);
            printf("  date/time: %s \n",ans->DT);
            printf("  city area: %d => %s\n", ans->City, Areas[ans->City]);
            printf("  IUCR Code: %s\n", ans->IUCR);

            if(ans->arrested == 1)
            {
                printf("  arrested:  true\n\n");
            }
            else
            {
                printf("  arrested:  false\n\n");
            }

        }

        printf("Enter a case number>  ");
        fgets(input, 64, stdin);
        input[strcspn(input, "\r\n")] = '\0';  // strip EOL(s) char at end:
        token = strtok(input, ",");
    }

    return;

}

// frees the elements of the pointer array
// and the values inside the struct
void freeHash(Crime *hastable[], int N)
{
    int i;

    for(i = 0; i < N; i++)
    {
        if(hastable[i] == NULL)
        {
            continue;
        }
        else
        {
            free(hastable[i]->Case);
            free(hastable[i]->IUCR);
            free(hastable[i]->DT);
            free(hastable[i]);
        }
    }
}

// frees the areas hashtable
void freeAreas(char *Areas[])
{
    int i;

    for(i = 0; i < 78; i++)
    {
        free(Areas[i]);
    }
}






int main()
{

    printf("** Chicago Crime Lookup **\n");

    char *CrimesFile = getFileName();
    char *AreasFile = getFileName();

    long fileSize = getFileSizeInBytes(CrimesFile);

    printf(">> Crimes file: %ld bytes\n", fileSize);

    int  N = (fileSize / 50) * 5;

    Crime **hashtable = (Crime **)malloc(N * sizeof(Crime));

    if (hashtable == NULL)
    {
        printf("** Error: malloc failed to allocate hash table (%d elements).\n", N);
        exit(-1);
    }

    int totalCrimes = buildHashTable(hashtable, N, CrimesFile);

    printf(">> # crimes:    %d\n", totalCrimes);
    printf(">> HT size:     %d\n", N);
    printf(">> Collisions:  %d\n\n", numCollisions);

    char *Areas[78];

    buildAreasTable(Areas, AreasFile);

    processInput(N, hashtable, Areas);

    printf("\n** Done **\n");

    freeAreas(Areas);
    freeHash(hashtable, N);

    free(hashtable);
    free(CrimesFile);
    free(AreasFile);

    return 0;

}
