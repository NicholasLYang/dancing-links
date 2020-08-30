#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
DataObj* add_data_obj(DataObj** arena, ColumnObj* col);
void print_all_columns(ColumnObj* header);
void print_column(ColumnObj* col);
void print_row(DataObj* row);
void cover_column(ColumnObj* col);
void uncover_column(ColumnObj* col);
int solve(ColumnObj* header, SolutionList** solutionList);
void print_solutions(SolutionList* solutionList);
void print_board(char* board);
void write_solutions(SolutionList* solutionList, char* board);
void add_solution(SolutionList** solutionList, DataObj* obj);
ColumnObj* init_sudoku_links(char* board);
DataObj* add_spacer(DataObj** arena, DataObj* up, DataObj* down);
void run_example_set_cover(void);

void run_example_set_cover() {
  ColumnObj* colArena = calloc(8, sizeof(ColumnObj));
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
  DataObj* obj = calloc(23, sizeof(DataObj));
  add_spacer(&obj, NULL, obj + 3); // Spacer
  // First row
  DataObj* r1c = add_data_obj(&obj, c);
  add_data_obj(&obj, e);
  add_data_obj(&obj, f);
  add_spacer(&obj, r1c, obj + 3); // Spacer
  // Second row
  DataObj* r2a = add_data_obj(&obj, a);
  add_data_obj(&obj, d);
  add_data_obj(&obj, g);

  add_spacer(&obj, r2a, obj + 3); // Spacer

  // Third row
  DataObj* r3b = add_data_obj(&obj, b);
  add_data_obj(&obj, c);
  add_data_obj(&obj, f);

  add_spacer(&obj, r3b, obj + 2); // Spacer

  // Fourth row
  DataObj* r4a = add_data_obj(&obj, a);
  add_data_obj(&obj, d);

  add_spacer(&obj, r4a, obj + 2); // Spacer

  // Fifth row
  DataObj* r5b = add_data_obj(&obj, b);
  add_data_obj(&obj, g);

  add_spacer(&obj, r5b, obj + 3); // Spacer
  // Sixth row
  DataObj* r6d = add_data_obj(&obj, d);
  /*DataObj* r6e = */add_data_obj(&obj, e);
  /*DataObj* r6g = */add_data_obj(&obj, g);

  add_spacer(&obj, r6d, NULL); // Spacer

  solve(header, NULL);
}


// Gets the column for a p_ij node
// Starts by adding one for the header, then doing the ij calc
#define GET_P_COL(i, j) (header + 1 + ((i) * 9) + (j))
// Gets the column for a r_ik node
// + 1 for the header, + 81 for the p columns, then k - 1
// cause k starts  at 1
#define GET_R_COL(i, k) (header + 1 + 81 + ((i) * 9) + (k) - 1)
#define GET_C_COL(j, k) (header + 1 + 162 + ((j) * 9) + (k) - 1)
#define GET_B_COL(x, k) (header + 1 + 243 + ((x) * 9) + (k) - 1)

// Board is a 9x9 = 81 length array. If a block is 0, it is not filled in
// otherwise it is
ColumnObj* init_sudoku_links(char* board) {
  ColumnObj* colArena = calloc(325, sizeof(ColumnObj));
  ColumnObj* header = colArena;
  header->name = "Header";
  header->size = 0;
  header->left = header;
  header->right = header;
  colArena += 1;
  for (char i = 0; i < 9; i++) {
    for (char j = 0; j < 9; j++) {
      char* name = malloc(3);
      name[0] = 'p';
      name[1] = i + '0';
      name[2] = j + '0';
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
  for (char j = 0; j < 9; j++) {
    for (char k = 1; k < 10; k++) {
      char* name = malloc(3);
      name[0] = 'c';
      name[1] = j + '0';
      name[2] = k + '0';
      add_column(&colArena, 0, name);
    }
  }
    for (char x = 0; x < 9; x++) {
    for (char k = 1; k < 10; k++) {
      char* name = malloc(3);
      name[0] = 'b';
      name[1] = x + '0';
      name[2] = k + '0';
      add_column(&colArena, 0, name);
    }
  }
  DataObj* nodeArena = calloc(3645, sizeof(DataObj));
  DataObj* prevRow = NULL;
  for (char i = 0; i < 9; i++) {
    for (char j = 0; j < 9; j++) {
      char x = 3 * (i / 3) + (j / 3);
      if (board[(i * 9) + j] != 0) {
	char k = board[(i * 9) + j];
	add_spacer(&nodeArena, prevRow, nodeArena + 4);
	prevRow = add_data_obj(&nodeArena, GET_P_COL(i, j)); // p_ij
	add_data_obj(&nodeArena, GET_R_COL(i, k)); // r_ik
	add_data_obj(&nodeArena, GET_C_COL(j, k)); // c_jk
	add_data_obj(&nodeArena, GET_B_COL(x, k));
      } else {
	for (char k = 1; k < 10; k++) {
	  add_spacer(&nodeArena, prevRow, nodeArena + 4);
	  prevRow = add_data_obj(&nodeArena, GET_P_COL(i, j)); // p_ij
	  add_data_obj(&nodeArena, GET_R_COL(i, k)); // r_ik
	  add_data_obj(&nodeArena, GET_C_COL(j, k)); // c_jk
	  add_data_obj(&nodeArena, GET_B_COL(x, k));
	}
      }
    }
  }
  add_spacer(&nodeArena, prevRow, NULL);
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
  obj->dataObj.up = (DataObj*)obj;
  obj->dataObj.down = (DataObj*)obj;
  *arena += 1;
  return obj;
}

/*
 * All inputs can be null. If they aren't, we connect them both ways
 */
DataObj* add_data_obj(DataObj** arena, ColumnObj* col) {
  DataObj* obj = *arena;
  obj->up = col->dataObj.up;
  obj->up->down = obj;
  obj->down = (DataObj*)col;
  col->dataObj.up = obj;
  obj->col = col;
  *arena += 1;
  return obj;
}


DataObj* add_spacer(DataObj** arena, DataObj* up, DataObj* down) {
  DataObj* spacer = *arena;
  spacer->up = up;
  spacer->down = down;
  spacer->col = (ColumnObj*)1;
  *arena += 1;
  return spacer;
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
  SolutionList* newNode = calloc(1, sizeof(SolutionList));
  newNode->obj = obj;
  newNode->next = *solutionList;
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

void print_board(char* board) {
  for (int i = 0; i < 81; i++) {
    printf("%i", board[i]);
    if ((i + 1) % 9 == 0) {
      printf("\n");
    }
  }
}

void write_solutions(SolutionList* solutionList, char* board) {
  while (solutionList != NULL) {
    DataObj* obj = solutionList->obj;
    // Initialize to 32 cause these are not valid values for i, j, value
    // If I initialized to NULL aka 0, that would be a valid value for
    // i and j
    char i = 32;
    char j = 32;
    char value = 32;
    do {
      if (value != 32 && i != 32 && j != 32) {
	board[(i * 9) + j] = value;
	break;
      }
      switch (obj->col->name[0]) {
	case 'p':
	  i = obj->col->name[1] - '0';
	  j = obj->col->name[2] - '0';
	  break;
	case 'r':
	case 'c':
	case 'b':
	  value = obj->col->name[2] - '0';
	  break;
	default:
	  fprintf(stderr, "Invalid format for column\n");
      }
      obj = obj + 1;
      // If this is a spacer node
      if ((int)obj->col & 1) {
	// Go to first elem in row
	obj = obj->up;
      }
    } while (obj != solutionList->obj);
    solutionList = solutionList->next;
  }
}

// Solved = 1
int solve(ColumnObj* header, SolutionList** solutionList) {
  if (header->right == header) {
    return 1;
  }
  ColumnObj* col = header->right;
  cover_column(col);
  DataObj* row = col->dataObj.down;
  while (row != (DataObj*)col) {
    add_solution(solutionList, row);
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
    SolutionList* old_sol = *solutionList;
    *solutionList = (*solutionList)->next;
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
  char* board = calloc(81, sizeof(char));
  board[0] = 5;
  ColumnObj* header = init_sudoku_links(board);
  SolutionList* solutionList;
  solve(header, &solutionList);
  write_solutions(solutionList, board);
  print_board(board);
}
