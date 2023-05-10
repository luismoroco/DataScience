#include <iostream>
#include <cstring>
#include "ds.hpp"

constexpr int MAX_USERS = 283228;
constexpr int LINE = 1024;
constexpr int BOOKS_INPUT = 58098;
constexpr int BUFFER = 200;

const char *iFiLeName = "ratmed.txt";
const char *iMovName = "movmed.txt";

char *sparTitle[BOOKS_INPUT + 1];
int sparIndex[BOOKS_INPUT + 1];

int binarySearch(int target) {
  int left = 0;
  int right = BOOKS_INPUT;
  int middle;

  while (left <= right) {
    middle = (left + right)/2;
    if (sparIndex[middle] == target) {
      return middle;  
    } else if (sparIndex[middle] < target) {
      left = middle + 1;
    } else {
      right = middle - 1;
    }
  }
  return -1;
}

void free() {
  for (int i = 1; i <= BOOKS_INPUT; ++i)
    delete[] sparTitle[i];
}

int main(int, char**) {
  FILE *iMov;
  iMov = fopen(iMovName, "r");
  if (iMov == NULL) {
    perror("Error while opening iMov the File!\n");
    exit(EXIT_FAILURE);
  }
 
  int id;
  int i = 1;
  char line[LINE];
  char buffer[BUFFER];
  while (fgets(line, sizeof(line), iMov) != NULL && i <= BOOKS_INPUT) {
    if (sscanf(line, "%d\t %[^\n]", &id, buffer) == 2) {
      sparTitle[i] = new char[strlen(buffer) + 1];
      sparIndex[i] = id;
      strcpy(sparTitle[i], buffer);
      ++i;
    }
  }
  
  fclose(iMov);

  FILE *iFile;
  iFile = fopen(iFiLeName, "r");
  if (!iFile) {
    perror("Error while opening iFile the File!\n");
    exit(EXIT_FAILURE);
  }

  MatrixLLBased<float> mt = MatrixLLBased<float>(MAX_USERS);

  int head, node;
  float rating;
  while (fscanf(iFile, "%d\t%d\t%f", &head, &node, &rating) != EOF) {
    if (rating != 0) mt.add(head, rating, node);
  }

  fclose(iFile);

  Main<float> x = Main<float>(mt, MAX_USERS); 

  pair<float, int> a = x.fitCosine(23);
  printf("%f . %d\n", a.first, a.second);


  free();
  return EXIT_SUCCESS;
}