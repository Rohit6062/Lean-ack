#include "gf2matrix.h"
#include <math.h>
uint32_t wordsize = 8 * sizeof(word);
uint32_t wordbitmask = 8 * sizeof(word);
uint32_t wordshift = 5;

void allocate_gf2matrix(gf2matrix *mat, uint32_t n_rows, uint32_t n_cols) {
    printf("allocating %d rows and %d cols\n",n_rows,n_cols);
    mat->n_rows = n_rows;
    mat->n_cols = n_cols;
    mat->n_words = (mat->n_cols + wordsize - 1) >> wordshift;
    mat->rows = (word **)malloc(mat->n_rows * sizeof(word *));

    for (uint32_t i = 0; i <= mat->n_rows; i++) {
        mat->rows[i] = (word *)malloc(mat->n_words);
        memset(mat->rows[i], 0, mat->n_words);
    }
    mat->m_data = (word *)mat->rows[0];
}

void dealloc_gf2matrix(gf2matrix *mat) {
    for (uint32_t i = 0; i <= mat->n_rows; i++)
        free(mat->rows[i]);
    free(mat->rows);
}

uint32_t get_nrows(gf2matrix *mat) { return mat->n_rows; }

uint32_t get_ncols(gf2matrix *mat) { return mat->n_cols; }

uint32_t get_nwords(gf2matrix *mat) { return mat->n_words; }

int get_entry(gf2matrix *mat, int n, int m) {
  return (mat->rows[n][(m / wordbitmask)] >> m) & 1;
}

word *get_word(gf2matrix *mat, int n, int l) {
  return mat->rows[n] + l * sizeof(word);
}

void set_entry(gf2matrix *mat, int n, int m, int val) {
  mat->rows[n][(m / wordbitmask)] ^=
      (-val ^ mat->rows[n][(m / wordbitmask)]) & (1 << m);
}

void swap_rows(gf2matrix *mat, int n, int k) {
  word *tmp = mat->rows[n];
  mat->rows[n] = mat->rows[k];
  mat->rows[k] = tmp;
}

void print_matrix(gf2matrix *mat) {
  uint32_t nrows = get_nrows(mat);
  uint32_t ncols = get_ncols(mat);
    printf("nrows %d cols %d \n",nrows,ncols);
  for (uint32_t i = 0; i < nrows; i++) {
    for (uint32_t j = 0; j < ncols; j++)
      printf("%d ", get_entry(mat, i, j));
    printf("\n");
  }
    printf("\n");
}
void print_matrix2(gf2matrix *mat,char* res) {
  uint32_t nrows = get_nrows(mat);
  uint32_t ncols = get_ncols(mat);
    printf("nrows %d cols %d \n",nrows,ncols);
  for (uint32_t i = 0; i < nrows; i++) {
    for (uint32_t j = 0; j < ncols; j++)
      printf("%d ", get_entry(mat, i, j));
    printf("%d ",res[i]);
    printf("\n");
  }
    printf("\n");
}


void swap_cols(gf2matrix *mat, int m, int k) {
  uint32_t nrows = get_nrows(mat);

  for (uint32_t i = 0; i < nrows; i++) {
    int tmp_val = get_entry(mat, i, m);
    set_entry(mat, i, m, get_entry(mat, i, k));
    set_entry(mat, i, k, tmp_val);
  }
}

void create_identity_matrix(gf2matrix *identity, uint32_t ncols,
                            uint32_t nrows) {
  allocate_gf2matrix(identity, nrows, ncols);
  for (uint32_t i = 0; i < min(ncols, nrows); i++)
    set_entry(identity, i, i, 1);
}

void mat_mul(gf2matrix *matA, gf2matrix *matB, gf2matrix *result) {
  int part;
  uint32_t ncols_A = get_ncols(matA);
  uint32_t nrows_B = get_nrows(matB);

  for (uint32_t i = 0; i < ncols_A; i++) {
    for (uint32_t j = 0; j < nrows_B; j++) {
      part = get_entry(matA, i, 0) * get_entry(matB, 0, j);

      for (uint32_t z = 1; z < ncols_A; z++)
        part = part ^ (get_entry(matA, i, z) * get_entry(matB, z, j));

      set_entry(result, i, j, part);
    }
  }
}

int gaussjordan_inv(gf2matrix *mat) {
  gf2matrix identity;
  create_identity_matrix(&identity, get_ncols(mat), get_nrows(mat));

  uint32_t nrows_mat = get_nrows(mat);
  uint32_t ncols_mat = get_ncols(mat);
  uint32_t nwords_mat = get_nwords(mat);
  for (uint32_t i = 0, j = 0; i < nrows_mat; j = ++i) {
    for (; j < ncols_mat && !get_entry(mat, j, i); j++);

    if (i != j)
      swap_rows(mat, i, j);
    swap_rows(&identity, i, j);

    for (uint32_t k = 0; k < nrows_mat; k++)
      if (k != i && get_entry(mat, k, i))
        for (uint32_t l = 0; l < nwords_mat; l++) {
          mat->rows[k][l] = mat->rows[k][l] ^ mat->rows[i][l];
          (&identity)->rows[k][l] =
              (&identity)->rows[k][l] ^ (&identity)->rows[i][l];
        }
  }

  *mat = identity;
  return 0;
}

uint32_t min(uint32_t a, uint32_t b)
{
  if(a<b)return a;
  return b; 
}