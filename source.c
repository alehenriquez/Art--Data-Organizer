/* Lab 1: Art Data Organizer

  This program reads in an input file with a certain format and stores the data into struct arrays.
  
  By: Huy Nguyen (11183413)
  Date: 4/28/12
  CIS 26B 

  Compiler: GCC
  OS: Linux Mint 11
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
 {
   char* name;
   char* art;
   int rating;
 }CASE;
 
typedef struct
 {
   char* name;
   int count;
 }ARTCOUNT;
  

void allocateStructMem (CASE** all);
void readFile(FILE* FPin, CASE** all, CASE** pLast);
FILE* openFile(FILE* FPin);
CASE* sortArray(CASE* all, CASE* pLast);
void countPrintArt(CASE* all, CASE* pLast);
void printBestArt(CASE* all, CASE* pLast);
void freeCaseMem(CASE* all, CASE* pLast);


int main (void)
{
  CASE* all;
  CASE* pLast;
  FILE* FPin;
  CASE* walker;

  FPin = openFile(FPin);
  allocateStructMem(&all);
  readFile(FPin, &all, &pLast);
  all = sortArray(all, pLast); 
  countPrintArt(all, pLast);
  printBestArt(all, pLast);
  freeCaseMem(all, pLast);

  fclose(FPin);
  return 0;
}


/* freeCaseMem: free memory for CASE struct 
   pre: all is the array of struct
        pLast is the last element in all
   post: memory allocated to all is freed
*/
void freeCaseMem(CASE* all, CASE* pLast)
{
  CASE* walker;
  for(walker=all; walker<pLast; walker++)
  {
    free(walker->name);
  }
  free(all);
  return;
}


/* printBestArt: the best art(s), by rating, is printed
   pre: all is a sorted array
        pLast is the last element of all  
   post: best art(s) is printed to output file
*/
void printBestArt(CASE* all, CASE* pLast)
{
  CASE* walker;
  int highestRating = 0;
  char input[51];
  FILE* spOut;

  printf("File name for best rated art(s): ");
  scanf("%50s", input);
  if((spOut = fopen(input, "w")) == NULL)
  {
    printf("Error creating file.");
    exit(100);  
  }


  for(walker = all; walker <= pLast; walker++)
  {
    if(walker->rating > highestRating)
      highestRating = walker->rating;
  }

  for(walker = all; walker <= pLast; walker++)
  {
   if(walker->rating == highestRating)
     {
       fprintf(spOut, "%s", walker->name);
       fputs(walker->art, spOut);
     }
  }

  fclose(spOut);
 
  return;
}


/* countPrintArt: counts and prints number of pic an artist has and stores it in new struct
   pre: all is a sorted array
        pLast is the last element of all
   post: count, artist and pics are printed to file
*/
void countPrintArt(CASE* all, CASE* pLast)
{
  CASE* walker = all;
  char nameBuffer[100];
  int count = 0;
  FILE* spOut;
  char input[51];
  CASE* picStart = all;

  printf("File name for artist count output: ");
  scanf("%50s", input);
  if((spOut = fopen(input, "w")) == NULL)
  {
    printf("Error creating file.");
    exit(100);  
  } 

   strcpy(nameBuffer, walker->name);

   for(walker = all; walker <= pLast; walker++)
   {

     if(strcmp(nameBuffer, walker->name) == 0)
     {
        count++;
     }
     else
     {
       fprintf(spOut, "%d %s", count, nameBuffer);

       for(picStart; picStart<walker; picStart++)
       {
         fputs(picStart->art, spOut);
         fprintf(spOut, "# %d\n", picStart->rating); 
       }

       strcpy(nameBuffer, walker->name);
       count = 1;

     }
  }  

  fclose(spOut);
  return;
}


/* sortArray sorts the array by name
   pre: all is the unsorted array
        pLast points to the last element in all
   post: all is now sorted by name
*/
CASE* sortArray(CASE* all, CASE* pLast)
{
  CASE* current;
  CASE* walker;
  CASE* smallest;
  CASE temp;

  for(current = all; current < pLast; current++)
  {
    smallest = current;
    
    for (walker = current + 1; walker <= pLast; walker++)
    {
      if(strcmp(walker->name, smallest->name) < 0 )
       smallest = walker; 
    }    
    
   temp = *current;
   *current = *smallest;
   *smallest = temp; 
  }
  

  return all;
}

/* openFile: opens the input file
   pre: FPin is a file pointer pointing at NULL
   post: FPin points to a file and is returned to calling function
*/
FILE* openFile(FILE* FPin) 
{
  char input[51];
  printf("Name of input file: ");
  scanf("%50s", input);
  
  if((FPin = fopen(input, "r" )) == NULL)
  {
    printf("Error opening file.");
    exit(100);  
  }
  
  return FPin;
}


/* readFile: reads the input file and stores information in struct array
   pre: FPin is a file pointer
        all is an empty struct array
        pLast is null
   post: all has data
         pLast points to last element in all
         nothing is returned
*/
void readFile(FILE* FPin, CASE** all, CASE** pLast)
{ 
  CASE* walker = *all;

  char artBuffer[1600];
  char nameBuffer[100];
  int rate;


  /*Reads in the name*/

  while((fscanf(FPin, "%[^\n]", nameBuffer)) != EOF)
  {

    walker->name = (char*)malloc(sizeof(char)*(strlen(nameBuffer)+1));
    strcpy(walker->name, nameBuffer);
 
    /*Reads in the art*/
    fscanf(FPin, "%[^#]", artBuffer);

    /*Copy art from buffer to struct*/
    walker->art = (char*)malloc(sizeof(char)*(strlen(artBuffer)+1));
    strcpy(walker->art, artBuffer);

    /*Reads in the rating*/  
    fscanf(FPin, "%*c%d\n", &walker->rating);
 
    walker++;
  } 
  *pLast = walker - 1;
 
  *all = realloc(*all, (*pLast-*all + 1)*sizeof(CASE));
  return;
}


/* allocateStructMem: allocates memory for the CASE struct array
   pre: all is pointing at NULL
   post: memory allocated to all
         nothing is returned
*/

void allocateStructMem (CASE** all)
{
  if((*all = (CASE*)malloc(sizeof(CASE)*1000)) == NULL)
    {
      printf("Fatal memory error!\n");
      exit(1);
    }

  return;
}



/* Program output:


Name of input file: art.txt
File name for artist count output: count.txt
File name for best rated art(s): best.txt


*/
