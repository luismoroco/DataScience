#include <stdio.h>
#include <stdlib.h>

#define MAX_USERS 10
#define MAX_BOOKS 20

const int N = 10;
char iFiLeName[8] = "test.txt";

struct Node {
  float rating;
  struct Node* next;
};

struct LinkedList { 
  struct Node *root;
  struct Node *tail;
  int size;
  float moduleOf;
};

void initLinkedList(struct LinkedList *head) {
  head->root = NULL;
  head->tail = NULL;
  head->size = 0;
  head->moduleOf = 0.0f;
}

struct Node* initNewNode(float _rating) {
  struct Node* node = (struct Node*) malloc(sizeof(struct Node));
  if (node == NULL) {
    perror("Error while trying to init node!\n");
    exit(EXIT_FAILURE);
  }

  node->rating = _rating;
  node->next = NULL;
  return node;
}

void insertNewNode(struct LinkedList *head, float rating) {
  struct Node *node = initNewNode(rating);

  if (head->root == NULL) {
    head->root = node;
    head->tail = node;
    head->size += 1;
  } else {
    head->tail->next = node;
    head->tail = node;
    head->size += 1;
  }
}

struct SparseMatrixLinkedListBased {
  double *reultsByIndex;
  struct Node* headForUsers[MAX_BOOKS + 1];
};

void initMatrix(struct SparseMatrixLinkedListBased *matrix) {
  matrix->reultsByIndex = (double *) malloc(MAX_USERS + 1 * sizeof(double));

  for (int i = 1; i <= MAX_BOOKS; ++i) {
    matrix->headForUsers[i] = (struct Node *) malloc(sizeof(struct Node));
    matrix->headForUsers[i]->rating = 0.0;
    matrix->headForUsers[i]->next = NULL;
  }
}

int main() {
  FILE *iFile;
  iFile = fopen(iFiLeName, "r");
  if (!iFile) {
    perror("Error while opening the File!\n");
    exit(EXIT_FAILURE);
  }

  int head, node;
  float rating;
  while (fscanf(iFile, "%d\t%d\t%f", &head, &node, &rating) != EOF) {
    printf("%d\t%d\t%f\n", head, node, rating);
  }
  





  fclose(iFile);

  struct SparseMatrixLinkedListBased matrix;
  initMatrix(&matrix);


  return 0;
}