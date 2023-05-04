#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 2
#define LINE 1024
#define BOOKS_INPUT 5
#define BUFFER 200

// #define MAX_USERS 610
// #define BOOKS_INPUT 9742

char iFiLeName[10] = "test.txt";
char iMovName[12] = "testmov.txt";

char *sparTitle[BOOKS_INPUT + 1];
int sparIndex[BOOKS_INPUT + 1];

struct Tuple {
  float similarity;
  int id;
};

struct Node {
  float rating;
  int index;
  struct Node* next;
};

struct LinkedList { 
  struct Node *root;
  struct Node *tail;
  int size;
  float module;
};

void initLinkedList(struct LinkedList *head) {
  head->root = NULL;
  head->tail = NULL;
  head->size = 0;
  head->module = 0.0f;
}

struct Node* initNewNode(float _rating, int _index) {
  struct Node* node = (struct Node*) malloc(sizeof(struct Node));
  if (node == NULL) {
    perror("Error while trying to init node!\n");
    exit(EXIT_FAILURE);
  }

  node->rating = _rating;
  node->next = NULL;
  node->index = _index;
  return node;
}

void insertNewNode(struct LinkedList **head, float rating, int index) {
  struct Node *node = initNewNode(rating, index);

  if (*head == NULL) {
    *head = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    initLinkedList(*head);
  }

  if ((*head)->root == NULL) {
    (*head)->root = node;
    (*head)->tail = node;
    (*head)->size += 1;
    (*head)->module += pow(node->rating, 2);
  } else {
    (*head)->tail->next = node;
    (*head)->tail = node;
    (*head)->size += 1;
    (*head)->module += pow(node->rating, 2);
  }
}

struct SparseMatrixLinkedList {
  struct LinkedList *indexForRoots[MAX_USERS + 1];
};

void initMatrixLL(struct SparseMatrixLinkedList *root) {
  for (int i = 1; i <= MAX_USERS; ++i) {
    root->indexForRoots[i] = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    initLinkedList(root->indexForRoots[i]);
  }
}

void addItemToMatrix(struct SparseMatrixLinkedList *root, int index, float rating, int node) {
  insertNewNode(&root->indexForRoots[index], rating, node);
}

float dotProductBetween(struct LinkedList *from, struct LinkedList *to) {
  float dot = 0.0f;
  struct Node *prtFrom = from->root;
  struct Node *ptrTo = to->root;

  while (prtFrom != NULL && ptrTo != NULL) {
    if (prtFrom->index == ptrTo->index) {
      dot += prtFrom->rating * ptrTo->rating;
      prtFrom = prtFrom->next;
      ptrTo = ptrTo->next; 
    } else if (prtFrom->index < ptrTo->index) {
      prtFrom = prtFrom->next;
    } else {
      ptrTo = ptrTo->next;
    }
  } 

  return dot;
}

float manhattanBetween(struct LinkedList *from, struct LinkedList *to) {
  float manh = 0.0f;
  struct Node *prtFrom = from->root;
  struct Node *ptrTo = to->root;

  while (prtFrom != NULL && ptrTo != NULL) {
    if (prtFrom->index == ptrTo->index) {
      manh += fabs(prtFrom->rating - ptrTo->rating);
      prtFrom = prtFrom->next;
      ptrTo = ptrTo->next; 
    } else if (prtFrom->index < ptrTo->index) {
      prtFrom = prtFrom->next;
    } else {
      ptrTo = ptrTo->next;
    }
  } 

  free(ptrTo);

  return manh;
}

float queryDotProductBetween(struct SparseMatrixLinkedList *root, int src, int to) {
  return dotProductBetween(root->indexForRoots[src], root->indexForRoots[to]);
}

float queryManhattanBetween(struct SparseMatrixLinkedList *root, int src, int to) {
  return manhattanBetween(root->indexForRoots[src], root->indexForRoots[to]);
}

void freeLL(struct LinkedList *head) {
  struct Node* current = head->root;
  struct Node* next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  free(head);
}

void freeMemory(struct SparseMatrixLinkedList *root) {
  for (int i = 0; i <= MAX_USERS; ++i) {
    freeLL(root->indexForRoots[i]);
  }
}

struct Tuple computeBestCosineSimilarity(struct SparseMatrixLinkedList *root, int src) {
  struct Tuple best = {-1.0f, -1};
  float similarity = -1.1f;

  for (int i = 1; i <= MAX_USERS; ++i) {
    if (src == i) continue;
    similarity = queryDotProductBetween(root, src, i)/(sqrt(root->indexForRoots[src]->module) * sqrt(root->indexForRoots[i]->module));
    best = (similarity > best.similarity) ? (struct Tuple){similarity, i} : best;
  }
 
  return best;
}

struct Tuple computeBestManhattan(struct SparseMatrixLinkedList *root, int src) {
  struct Tuple best = {99999.0f, -1};
  float distance = -1.0f;

  for (int i = 1; i <= MAX_USERS; ++i) {
    if (src == i) continue;
    distance = queryManhattanBetween(root, src, i);
    best = (distance < best.similarity) ? (struct Tuple){distance, i} : best;
  }

  return best;
}

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

/* HELP fx */

void pritInfOfNodes(struct LinkedList **head) {
  struct Node *tmp = (*head)->root;
  while (tmp != NULL) {
    printf(" -> %f", tmp->rating);
    tmp = tmp->next;
  }
  printf("\n");
}

void printInfMatrix(struct SparseMatrixLinkedList *root) {
  for (int i = 1; i <= MAX_USERS; i++) {
    float t = sqrt(root->indexForRoots[i]->module);
    printf("module = %f\t", t);
  }
  
  printf("\n");
  for (int i = 1; i <= MAX_USERS; ++i) {
    pritInfOfNodes(&root->indexForRoots[i]);
  }
}

/* ----- */

int main() {
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
      sparTitle[i] = malloc(strlen(buffer) + 1);
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

  struct SparseMatrixLinkedList matrix;
  initMatrixLL(&matrix);

  int head, node;
  float rating;
  while (fscanf(iFile, "%d\t%d\t%f", &head, &node, &rating) != EOF) {
    if (rating != 0) {
      addItemToMatrix(&matrix, head, rating, node);
    }
  }

  fclose(iFile);

  int option; 
  int idUser;
  struct Tuple res;
  do {
    printf("Select an option: \n1) Cosine Similarity\n2) Manhattan Distance\n4) Exit\n");
    scanf("%d", &option);

    switch (option) {
      case 1:
        printf("Put the User Id: ");
        scanf("%d", &idUser);
        res = computeBestCosineSimilarity(&matrix, idUser);
        printf("cos: %f - id user: %d\n", res.similarity, res.id);
        break;

      case 2:
        printf("Put the User Id: ");
        scanf("%d", &idUser);
        res = computeBestManhattan(&matrix, idUser);
        printf("cos: %f - id user: %d\n", res.similarity, res.id);
        break;
      
      case 4:
        printf("Bye!\n");
        return 0;
      
      default:
        printf("That option doesn't exist!\n");
        break;
    }
  } while (option != 4);

  freeMemory(&matrix);
  return 0;
}