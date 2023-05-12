#include <iostream>
#include <cstring>

#include "ds.hpp"

constexpr int MAX_USERS = 25; //283228 2 25 2
constexpr int LINE = 1024;
constexpr int BOOKS_INPUT = 25; //58098 5 25 5 
constexpr int BUFFER = 200;

constexpr int K = 5;

const char *iFiLeName = "movi-ratings-out.txt"; // movi-ratings-out.txt ratmed.txt coef-rat.txt
const char *iMovName = "movi-ratings-mov.txt";  // movi-ratings-mov.txt movmed.txt coef-mov.txt
const char *iUsers = "movi-ratings-user-name.txt";

const bool areThereNames = false;
char *names[MAX_USERS + 1];

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

  if (areThereNames == true) {
    FILE *iName;
    iName = fopen(iUsers, "r");
    if (iName == NULL) {
      perror("Error while opening iUsers the File!\n");
      exit(EXIT_FAILURE);
    }

    i = 1;
    while (fgets(line, sizeof(line), iName) != NULL && i <= BOOKS_INPUT) {
      if (sscanf(line, "%d\t %[^\n]", &id, buffer) == 2) {
        names[id] = new char[strlen(buffer) + 1];
        strcpy(names[id], buffer);
        ++i;
      }
    }

    fclose(iName);
  }

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

  //auto y = x.fitPearson(1, 2);
  
  
  //auto qe = QueryEngine<float, string>(mt);

  //auto y = qe.pearson(1, 2);
  //printf("%f\n", y);


  int option, idUser, a, b;
  float out;
  pair<float, int> res;
  do {
    printf("Select an option:\n0) Pearson\n1) Cosine Similarity\n2) Manhattan Distance\n3) Eucledian Distance\n4) Exit\n");
    scanf("%d", &option);

    switch (option) {
      case 0:
        printf("Put the User Id: \n");
        scanf("%d", &a);
        res = x.fitBestPearson(a);
        printf("cos: %f - id user: %d\n", res.first, res.second);
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
        printf("Manha: %f - id user: %d\n", res.first, res.second);
        break;
      
      case 3:
        printf("Put the User Id: ");
        scanf("%d", &idUser);
        res = x.fitEucledian(idUser);
        printf("Eucle: %f - id user: %d\n", res.first, res.second);
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
