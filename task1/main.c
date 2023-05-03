#include <stdio.h>
#include <stdlib.h>

#define MAX_USERS 2
#define MAX_BOOKS 193609

const int N = 10;
char iFiLeName[8] = "test.txt";

struct Node {
  float rating;
  int index;
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
  node->index = -1;
  return node;
}

void insertNewNode(struct LinkedList **head, float rating) {
  struct Node *node = initNewNode(rating);

  if (*head == NULL) {
    *head = (struct LinkedList *) malloc(sizeof(struct LinkedList));
    initLinkedList(*head);
  }

  if ((*head)->root == NULL) {
    (*head)->root = node;
    (*head)->tail = node;
    (*head)->size += 1;
  } else {
    (*head)->tail->next = node;
    (*head)->tail = node;
    (*head)->size += 1;
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

void addItemToMatrix(struct SparseMatrixLinkedList *root, int index, float rating) {
  insertNewNode(&root->indexForRoots[index], rating);
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

void pritInfOfNodes(struct LinkedList **head) {
  struct Node *tmp = (*head)->root;
  while (tmp != NULL) {
    printf(" -> %f", tmp->rating);
    tmp = tmp->next;
  }
  printf("\n");
}

void printInfMatrix(struct SparseMatrixLinkedList *root) {
  for (int i = 1; i <= MAX_USERS; ++i) {
    pritInfOfNodes(&root->indexForRoots[i]);
  }
}

int main() {
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
    addItemToMatrix(&matrix, head, rating);
  }


  fclose(iFile);

  printInfMatrix(&matrix);

  freeMemory(&matrix);

  return 0;
}