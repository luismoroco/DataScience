#include <iostream>
#include <cstring>

#include "ds.hpp"

constexpr int MAX_USERS = 2; //283228
constexpr int LINE = 1024;
constexpr int BOOKS_INPUT = 5; //58098
constexpr int BUFFER = 200;

constexpr int K = 5;

const char *iFiLeName = "test.txt";
const char *iMovName = "testmov.txt";

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

  KNN<float> x = KNN<float>(mt, MAX_USERS, K); 

  int option, idUser, a, b;
  float out;
  pair<float, int> res;
  do {
    printf("Select an option:\n0) Pearson\n1) Cosine Similarity\n2) Manhattan Distance\n3) Eucledian Distance\n4) Exit\n");
    scanf("%d", &option);

    switch (option) {
      case 0:
        /*
        printf("Put the User Id A and B: \n");
        scanf("%d %d", &a, &b);
        out = queryPearsonCoef(&matrix, a, b);
        printf("pearson coef: %f\n", out);
        */
        break;

      case 1:
        printf("Put the User Id: ");
        scanf("%d", &idUser);
        res = x.fitCosine(idUser);
        printf("cos: %f - id user: %d\n", res.first, res.second);
        break;

      case 2:
        printf("Put the User Id: ");
        scanf("%d", &idUser);
        res = x.fitManhattan(idUser);
        printf("cos: %f - id user: %d\n", res.first, res.second);
        break;
      
      case 3:
        printf("Put the User Id: ");
        scanf("%d", &idUser);
        res = x.fitEucledian(idUser);
        printf("cos: %f - id user: %d\n", res.first, res.second);
        break;
      
      case 4:
        printf("Bye!\n");
        return 0;
      
      default:
        printf("That option doesn't exist!\n");
        break;
    }
  } while (option != 4);

  free();
  return EXIT_SUCCESS;
}
