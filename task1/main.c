#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 610
#define MAX_BOOKS 193609
#define LINE 1024
#define BOOKS_INPUT 9742
#define BUFFER 200

const int N = 10;
char iFiLeName[8] = "out.txt";
char iMovName[12] = "outmov.txt";

char *titles[MAX_BOOKS + 1];

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

float queryDotProductBetween(struct SparseMatrixLinkedList *root, int src, int to) {
  return dotProductBetween(root->indexForRoots[src], root->indexForRoots[to]);
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

int binarySearch(int *arr, int len, int target) {
  return 1;
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
    perror("Error while opening the File!\n");
    exit(EXIT_FAILURE);
  }
 
  int id;
  int i = 0;
  char line[LINE];
  char buffer[BUFFER];
  while (fgets(line, sizeof(line), iMov) != NULL && i < BOOKS_INPUT) {
    if (sscanf(line, "%d\t %[^\n]", &id, buffer) == 2) {
      titles[id] = malloc(strlen(buffer) + 1);
      strcpy(titles[id], buffer);
      ++i;
    }
  }
  
  fclose(iMov);

  for (int i = 1; i <= MAX_BOOKS; ++i) {
    printf("Index %d : %s\n", i, titles[i]);
  }

  /*

  FILE *iFile;
  iFile = fopen(iFiLeName, "r");
  if (!iFile) {
    perror("Error while opening the File!\n");
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
  //printInfMatrix(&matrix);

  struct Tuple res = computeBestCosineSimilarity(&matrix, 1);
  printf("cos: %f - id user: %d", res.similarity, res.id);

  //freeMemory(&matrix);

  */
  return 0;
}