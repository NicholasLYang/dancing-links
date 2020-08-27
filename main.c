#include <stdio.h>
#include <stdlib.h>

typedef struct DataObj {
  struct DataObj* left;
  struct DataObj* right;
  struct DataObj* up;
  struct DataObj* down;
  struct ColumnObj* col;
} DataObj;

typedef struct ColumnObj {
  struct DataObj dataObj;
  size_t size;
  char* name;
} ColumnObj;

typedef struct SolutionList {
  struct DataObj* obj;
  struct SolutionList* next;
} SolutionList;

ColumnObj* add_column(size_t size, char* name, ColumnObj* header);
DataObj* add_data_obj(DataObj* left, DataObj* right, DataObj* up, DataObj* down, ColumnObj* col);
void print_all_columns(ColumnObj* header);
void print_column(ColumnObj* col);
void print_row(DataObj* row);
void cover_column(ColumnObj* col);
void uncover_column(ColumnObj* col);
int solve(ColumnObj* header, SolutionList* solutionList);
void print_solutions(SolutionList* solutionList);
void add_solution(SolutionList** solutionList, DataObj* obj);

int main() {
  ColumnObj* header = malloc(sizeof(ColumnObj));
  header->dataObj.left = (DataObj*)header;
  header->dataObj.right = (DataObj*)header;
  header->name = "Header";
  header->size = 0;
  ColumnObj* g = add_column(2, "G", header);
  ColumnObj* f = add_column(2, "F", header);
  ColumnObj* e = add_column(2, "E", header);
  ColumnObj* d = add_column(3, "D", header);
  ColumnObj* c = add_column(2, "C", header);
  ColumnObj* b = add_column(2, "B", header);
  ColumnObj* a = add_column(2, "A", header);


  // First row
  DataObj* r1c = add_data_obj(NULL, NULL, (DataObj*) c, NULL, c);
  DataObj* r1e = add_data_obj(r1c, NULL, (DataObj*) e, NULL, e);
  DataObj* r1f = add_data_obj(r1e, r1c, (DataObj*) f, NULL, f);

  // Second row
  DataObj* r2a = add_data_obj(NULL, NULL, (DataObj*) a, NULL, a);
  DataObj* r2d = add_data_obj(r2a, NULL, (DataObj*) d, NULL, d);
  DataObj* r2g = add_data_obj(r2d, r2a, (DataObj*) g, NULL, g);

  // Third row
  DataObj* r3b = add_data_obj(NULL, NULL, (DataObj*) b, NULL, b);
  DataObj* r3c = add_data_obj(r3b, NULL, r1c, (DataObj*) c, c);
  /*DataObj* r3f = */add_data_obj(r3c, r3b, r1f, (DataObj*) f, f);

  // Fourth row
  DataObj* r4a = add_data_obj(NULL, NULL, r2a, (DataObj*) a, a);
  DataObj* r4d = add_data_obj(r4a, r4a, r2d, NULL, d);

  // Fifth row
  DataObj* r5b = add_data_obj(NULL, NULL, r3b, (DataObj*) b, b);
  DataObj* r5g = add_data_obj(r5b, r5b, r2g, NULL, g);

  // Sixth row
  DataObj* r6d = add_data_obj(NULL, NULL, r4d, (DataObj*) d, d);
  DataObj* r6e = add_data_obj(r6d, NULL, r1e, (DataObj*) e, e);
  /*DataObj* r6g = */add_data_obj(r6e, r6d, r5g, (DataObj*) g, g);

  /*******************/
  /* print_row(r1c); */
  /* print_row(r2a); */
  /* print_row(r3b); */
  /* print_row(r4a); */
  /* print_row(r5b); */
  /* print_row(r6d); */
  /*******************/
  int res = solve(header, NULL);
  printf("RES: %i\n", res);
  return 0;

}

/*
 * Adds a column obj right after header
 */
ColumnObj* add_column(size_t size, char* name, ColumnObj* header) {
  ColumnObj* obj = malloc(sizeof(ColumnObj));
  obj->size = size;
  obj->name = name;
  obj->dataObj.left = (DataObj*)header;
  obj->dataObj.right = header->dataObj.right;
  header->dataObj.right->left = (DataObj*)obj;
  header->dataObj.right = (DataObj*)obj;
  return obj;
}

/*
 * All inputs can be null. If they aren't, we connect them both ways
 */
DataObj* add_data_obj(DataObj* left, DataObj* right, DataObj* up, DataObj* down, ColumnObj* col) {
  DataObj* obj = malloc(sizeof(DataObj));
  if (left != NULL) {
    obj->left = left;
    obj->left->right = obj;
  }
  if (right != NULL) {
    obj->right = right;
    obj->right->left = obj;
  }
  if (up != NULL) {
    obj->up = up;
    obj->up->down = obj;
  }
  if (down != NULL) {
    obj->down = down;
    obj->down->up = obj;
  }
  obj->col = col;
  return obj;
}


void print_all_columns(ColumnObj* header) {
  ColumnObj* p = header;
  do {
    printf("PTR: %p, NAME: %s, SIZE: %zu\n", (void*)p, p->name, p->size);
    if (p != header) {
      print_column(p);
    }
    p = (ColumnObj*)p->dataObj.right;
  } while (p != header);
}

void print_row(DataObj* row) {
  DataObj* r = row;
  printf("------\n");
  do {
    printf("PTR: %p, COLUMN: %s\n", (void*)r, r->col->name);
    r = r->right;
  } while (r != row);
  printf("------\n");
}

void print_column(ColumnObj* col) {
  DataObj* p = col->dataObj.down;
  printf("-----\n");
  do {
    printf("PTR: %p\n", (void*)p);
    p = p->down;
  } while (p != (DataObj*)col);
  printf("-----\n");
}

void cover_column(ColumnObj* col) {
  col->dataObj.right->left = col->dataObj.left;
  col->dataObj.left->right = col->dataObj.right;
  DataObj* obj = col->dataObj.down;
  while (obj != (DataObj*)col) {
    DataObj* rowObj = obj->right;
    while (rowObj != obj) {
      rowObj->down->up = rowObj->up;
      rowObj->up->down = rowObj->down;
      rowObj->col->size = rowObj->col->size - 1;
      rowObj = rowObj->right;
    }
    obj = obj->down;
  }
}

void uncover_column(ColumnObj* col) {
  DataObj* obj = col->dataObj.up;
  while (obj != (DataObj*)col) {
    DataObj* rowObj = obj->left;
    while (rowObj != obj) {
      rowObj->col->size = rowObj->col->size + 1;
      rowObj->down->up = rowObj;
      rowObj->up->down = rowObj;
      rowObj = rowObj->left;
    }
    obj = obj->up;
  }
  col->dataObj.right->left = (DataObj*)col;
  col->dataObj.left->right = (DataObj*)col;
}

void add_solution(SolutionList** solutionList, DataObj* obj) {
  SolutionList* newNode = malloc(sizeof(SolutionList));
  newNode->obj = obj;
  if (*solutionList != NULL) {
    newNode->next = *solutionList;
  }
  *solutionList = newNode;
}

void print_solutions(SolutionList* solutionList) {
  while (solutionList != NULL) {
    DataObj* obj = solutionList->obj;
    do {
      printf("%s ", obj->col->name);
      obj = obj->right;
    } while (obj != solutionList->obj);
    printf("\n");
    solutionList = solutionList->next;
  }
}

// Solved = 1
int solve(ColumnObj* header, SolutionList* solutionList) {
  if (header->dataObj.right == (DataObj*) header) {
    print_solutions(solutionList);
    return 1;
  }
  ColumnObj* col = (ColumnObj*) header->dataObj.right;
  cover_column(col);
  DataObj* row = col->dataObj.down;
  while (row != (DataObj*)col) {
    add_solution(&solutionList, row);
    DataObj* rowObj = row->right;
    while (rowObj != row) {
      cover_column(rowObj->col);
      rowObj = rowObj->right;
    }
    if (solve(header, solutionList) == 1) {
      return 1;
    }
    solutionList = solutionList->next;
    rowObj = rowObj->left;
    while (rowObj != row) {
      uncover_column(rowObj->col);
      rowObj = rowObj->left;
    }
    row = row->down;
  }
  uncover_column(col);
  return 0;
}
