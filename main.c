#include <stdio.h>
#include <stdlib.h>

typedef struct DataObj {
  struct DataObj* up;
  struct DataObj* down;
  struct ColumnObj* col;
} DataObj;

typedef struct ColumnObj {
  struct DataObj dataObj;
  struct ColumnObj* left;
  struct ColumnObj* right;
  size_t size;
  char* name;
} ColumnObj;

typedef struct SolutionList {
  struct DataObj* obj;
  struct SolutionList* next;
} SolutionList;

ColumnObj* add_column(ColumnObj** arena, size_t size, char* name);
DataObj* add_data_obj(DataObj** arena, DataObj* up, DataObj* down, ColumnObj* col);
void print_all_columns(ColumnObj* header);
void print_column(ColumnObj* col);
void print_row(DataObj* row);
void cover_column(ColumnObj* col);
void uncover_column(ColumnObj* col);
int solve(ColumnObj* header, SolutionList* solutionList);
void print_solutions(SolutionList* solutionList);
void add_solution(SolutionList** solutionList, DataObj* obj);
void run_example_set_cover(void);
ColumnObj* init_sudoku_columns(void);

void run_example_set_cover() {
  ColumnObj* colArena = malloc(sizeof(ColumnObj) * 8);
  ColumnObj* header = colArena;
  header->name = "Header";
  header->size = 0;
  header->left = header;
  header->right = header;
  colArena += 1;

  ColumnObj* g = add_column(&colArena, 2, "G");
  ColumnObj* f = add_column(&colArena, 2, "F");
  ColumnObj* e = add_column(&colArena, 2, "E");
  ColumnObj* d = add_column(&colArena, 3, "D");
  ColumnObj* c = add_column(&colArena, 2, "C");
  ColumnObj* b = add_column(&colArena, 2, "B");
  ColumnObj* a = add_column(&colArena, 2, "A");
  DataObj* obj = malloc(sizeof(DataObj) * 23);

  add_data_obj(&obj, NULL, obj + 3, (ColumnObj*)1); // Spacer
  // First row
  DataObj* r1c = add_data_obj(&obj, (DataObj*) c, NULL, c);
  DataObj* r1e = add_data_obj(&obj, (DataObj*) e, NULL, e);
  DataObj* r1f = add_data_obj(&obj, (DataObj*) f, NULL, f);

  add_data_obj(&obj, r1c, obj + 3, (ColumnObj*)3); // Spacer
  // Second row
  DataObj* r2a = add_data_obj(&obj, (DataObj*) a, NULL, a);
  DataObj* r2d = add_data_obj(&obj, (DataObj*) d, NULL, d);
  DataObj* r2g = add_data_obj(&obj, (DataObj*) g, NULL, g);

  add_data_obj(&obj, r2a, obj + 3, (ColumnObj*)5); // Spacer

  // Third row
  DataObj* r3b = add_data_obj(&obj, (DataObj*) b, NULL, b);
  /*DataObj* r3c = */add_data_obj(&obj, r1c, (DataObj*) c, c);
  /*DataObj* r3f = */add_data_obj(&obj, r1f, (DataObj*) f, f);

  add_data_obj(&obj, r3b, obj + 2, (ColumnObj*)7); // Spacer

  // Fourth row
  DataObj* r4a = add_data_obj(&obj, r2a, (DataObj*) a, a);
  DataObj* r4d = add_data_obj(&obj, r2d, NULL, d);

  add_data_obj(&obj, r4a, obj + 2, (ColumnObj*)9); // Spacer

  // Fifth row
  DataObj* r5b = add_data_obj(&obj, r3b, (DataObj*) b, b);
  DataObj* r5g = add_data_obj(&obj, r2g, NULL, g);

  add_data_obj(&obj, r5b, obj + 3, (ColumnObj*)11); // Spacer
  // Sixth row
  DataObj* r6d = add_data_obj(&obj, r4d, (DataObj*) d, d);
  /*DataObj* r6e = */add_data_obj(&obj, r1e, (DataObj*) e, e);
  /*DataObj* r6g = */add_data_obj(&obj, r5g, (DataObj*) g, g);

  add_data_obj(&obj, r6d, NULL, (ColumnObj*)13); // Spacer

  print_row(r1c);
  print_row(r2a);
  print_row(r3b);
  print_row(r4a);
  print_row(r5b);
  print_row(r6d);
  solve(header, NULL);
}

ColumnObj* init_sudoku_columns() {
  ColumnObj* colArena = malloc(sizeof(ColumnObj) * 325);
  ColumnObj* header = colArena;
  header->name = "Header";
  header->size = 0;
  colArena += 1;
  for (char x = 0; x < 9; x++) {
    for (char k = 1; k < 10; k++) {
      char* name = malloc(3);
      name[0] = 'b';
      name[1] = x + '0';
      name[2] = k + '0';
      add_column(&colArena, 0, name);
    }
  }
  for (char j = 0; j < 9; j++) {
    for (char k = 1; k < 10; k++) {
      char* name = malloc(3);
      name[0] = 'c';
      name[1] = j + '0';
      name[2] = k + '0';
      add_column(&colArena, 0, name);
    }
  }
  for (char i = 0; i < 9; i++) {
    for (char k = 1; k < 10; k++) {
      char* name = malloc(3);
      name[0] = 'r';
      name[1] = i + '0';
      name[2] = k + '0';
      add_column(&colArena, 0, name);
    }
  }
  for (char i = 0; i < 9; i++) {
    for (char j = 0; j < 9; j++) {
      char* name = malloc(3);
      name[0] = 'p';
      name[1] = i + '0';
      name[2] = j + '0';
      add_column(&colArena, 0, name);
    }
  }

  print_all_columns(header);
  return header;
}

/*
 * Adds a column obj. Assumes that there is a column obj to the left
 * that is initialized.
 */
ColumnObj* add_column(ColumnObj** arena, size_t size, char* name) {
  ColumnObj* obj = *arena;
  obj->left = obj - 1;
  obj->right = obj->left->right;
  obj->left->right = obj;
  obj->right->left = obj;
  obj->size = size;
  obj->name = name;
  *arena += 1;
  return obj;
}

/*
 * All inputs can be null. If they aren't, we connect them both ways
 */
DataObj* add_data_obj(DataObj** arena, DataObj* up, DataObj* down, ColumnObj* col) {
  DataObj* obj = *arena;
  if (up != NULL) {
    obj->up = up;
    obj->up->down = obj;
  }
  if (down != NULL) {
    obj->down = down;
    obj->down->up = obj;
  }
  obj->col = col;
  *arena += 1;
  return obj;
}



void print_all_columns(ColumnObj* header) {
  ColumnObj* p = header;
  do {
    printf("PTR: %p, NAME: %s, SIZE: %zu\n", (void*)p, p->name, p->size);
    if (p != header) {
      print_column(p);
    }
    p = p->right;
  } while (p != header);
}

void print_row(DataObj* row) {
  DataObj* r = row;
  printf("------\n");
  do {
    printf("PTR: %p, COLUMN: %s\n", (void*)r, r->col->name);
    r = r + 1;
  } while (((int)r->col & 1) == 0);
  printf("------\n");
}

void print_column(ColumnObj* col) {
  DataObj* p = col->dataObj.down;
  if (p == NULL) {
    return;
  }
  printf("-----\n");
  do {
    printf("PTR: %p\n", (void*)p);
    p = p->down;
  } while (p != (DataObj*)col);
  printf("-----\n");
}

void cover_column(ColumnObj* col) {
  col->right->left = col->left;
  col->left->right = col->right;
  DataObj* obj = col->dataObj.down;
  while (obj != (DataObj*)col) {
    DataObj* rowObj = obj + 1;
    while (rowObj != obj) {
      // If this is a spacer node
      if ((int)rowObj->col & 1) {
	printf("COL: %p\n",(void*) rowObj->col);
	// Go to first elem in row
	rowObj = rowObj->up;
      }
      if (rowObj == obj) {
	break;
      }
      rowObj->down->up = rowObj->up;
      rowObj->up->down = rowObj->down;
      rowObj->col->size = rowObj->col->size - 1;
      rowObj += 1;
      printf("\n\n");
      printf("obj: %p\n", obj);
      printf("row: %p\n", rowObj);
      printf("\n\n");
    }
    obj = obj->down;
  }
}

void uncover_column(ColumnObj* col) {
  DataObj* obj = col->dataObj.up;
  while (obj != (DataObj*)col) {
    DataObj* rowObj = obj - 1;
    while (rowObj != obj) {
      // If this is a spacer node
      if ((int)rowObj->col & 1) {
	// Go to last elem in row
	rowObj = rowObj->down;
      }
      if (rowObj == obj) {
	break;
      }
      rowObj->col->size = rowObj->col->size + 1;
      rowObj->down->up = rowObj;
      rowObj->up->down = rowObj;
      rowObj += -1;
    }
    obj = obj->up;
  }
  col->right->left = col;
  col->left->right = col;
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
      obj = obj + 1;
      // If this is a spacer node
      if ((int)obj->col & 1) {
	// Go to first elem in row
	obj = obj->up;
      }
    } while (obj != solutionList->obj);
    printf("\n");
    solutionList = solutionList->next;
  }
}

// Solved = 1
int solve(ColumnObj* header, SolutionList* solutionList) {
  if (header->right == header) {
    print_solutions(solutionList);
    return 1;
  }
  ColumnObj* col = header->right;
  cover_column(col);
  DataObj* row = col->dataObj.down;
  while (row != (DataObj*)col) {
    add_solution(&solutionList, row);
    DataObj* rowObj = row + 1;
    while (rowObj != row) {
      // If this is a spacer node
      if ((int)rowObj->col & 1) {
	// Go backwards to first elem in row
	rowObj = rowObj->up;
      }
      if (rowObj == row) {
	break;
      }
      cover_column(rowObj->col);
      rowObj += 1;
    }
    if (solve(header, solutionList) == 1) {
      return 1;
    }
    SolutionList* old_sol = solutionList;
    solutionList = solutionList->next;
    free(old_sol);
    rowObj = rowObj - 1;
    while (rowObj != row) {
      // If this is a spacer node
      if ((int)rowObj->col & 1) {
	// Go to last elem in row
	rowObj = rowObj->down;
      }
      if (rowObj == row) {
	break;
      }
      uncover_column(rowObj->col);
      rowObj = rowObj - 1;
    }
    row = row->down;
  }
  uncover_column(col);
  return 0;
}

int main() {
  run_example_set_cover();
}
