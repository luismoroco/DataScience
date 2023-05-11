#include <iostream>
#include <cstring>

#include "ds.hpp"

constexpr int MAX_USERS = 25; //283228 2 25
constexpr int LINE = 1024;
constexpr int BOOKS_INPUT = 25; //58098 5 25
constexpr int BUFFER = 200;

constexpr int K = 5;

const char *iFiLeName = "movi-ratings-out.txt";
const char *iMovName = "movi-ratings-mov.txt";

int main(int, char**) {
  FILE *iMov;
  iMov = fopen(iMovName, "r");
  if (iMov == NULL) {
    perror("Error while opening iMov the File!\n");
    exit(EXIT_FAILURE);
  }

  auto oEng = ObjectEngine<string>();

  int id;
  int i = 1;
  char line[LINE];
  char buffer[BUFFER];
  while (fgets(line, sizeof(line), iMov) != NULL && i <= BOOKS_INPUT) {
    if (sscanf(line, "%d\t %[^\n]", &id, buffer) == 2) {
      oEng.add(id, string(buffer));
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

  auto mt = MatrixLLBased<float, string>(MAX_USERS);

  int head, node;
  float rating;
  while (fscanf(iFile, "%d\t%d\t%f", &head, &node, &rating) != EOF) {
    if (rating != 0) mt.add(head, rating, node);
  }

  fclose(iFile);

  auto x = KNN<float, string>(mt, MAX_USERS, K); 

  int option, idUser, a, b;
  float out;
  pair<float, int> res;
  do {
    printf("Select an option:\n0) Pearson\n1) Cosine Similarity\n2) Manhattan Distance\n3) Eucledian Distance\n4) Exit\n");
    scanf("%d", &option);

    switch (option) {
      case 0:
    
        //printf("Put the User Id A and B: \n");
        //scanf("%d %d", &a, &b);
        //out = queryPearsonCoef(&matrix, a, b);
        //printf("pearson coef: %f\n", out);

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

  return EXIT_SUCCESS;
}
