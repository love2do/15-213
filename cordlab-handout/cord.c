/*
 * @file cord.c
 * @brief Implementation of cords library
 *
 * 15-513 Introduction to Computer Systems
 *
 * TODO: fill in your name and Andrew ID below
 * @author XXX <XXX@andrew.cmu.edu>
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cord-interface.h"
#include "xalloc.h"

/***********************************/
/* Implementation (edit this part) */
/***********************************/

/**
 * @brief Checks if a cord is valid
 * @param[in] R
 * @return
 */
bool is_cord(const cord_t *R) {
  if (!R)
    return true;
  if (R->len == 0)
    return false;
  if (!R->left && !R->right && R->data && R->len == strlen(R->data))
    return true;

  if (R->left && R->right && R->len == R->left->len + R->right->len &&
      is_cord(R->left) && is_cord(R->right))
    return true;
  return false;
}

/**
 * @brief Returns the length of a cord
 * @param[in] R
 * @return
 */
size_t cord_length(const cord_t *R) {
  if (R)
    return R->len;
  return 0;
}

/**
 * @brief Allocates a new cord from a string
 * @param[in] s
 * @return
 */
const cord_t *cord_new(const char *s) {
  if (!s || strlen(s) == 0)
    return NULL;

  cord_t *newt = xcalloc(1, sizeof(cord_t));
  newt->left = newt->right = NULL;
  newt->len = strlen(s);
  char *target = xmalloc(newt->len + 1);
  strcpy(target, s);
  newt->data = target;
  return newt;
}

/**
 * @brief Concatenates two cords into a new cord
 * @param[in] R
 * @param[in] S
 * @return
 */
const cord_t *cord_join(const cord_t *R, const cord_t *S) {
  if (!R)
    return S;
  if (!S)
    return R;
  cord_t *newt = xcalloc(1, sizeof(cord_t));
  newt->left = R;
  newt->right = S;
  newt->len = R->len + S->len;
  return newt;
}

/**
 * @brief Converts a cord to a string
 * @param[in] R
 * @return
 */
char *cord_tostring(const cord_t *R) {
  char *result = xcalloc(1, cord_length(R) + 1);
  if (!R)
    return result;
  if (!R->left && !R->right) {
    strcpy(result, R->data);
    return result;
  }
  char *left_s = cord_tostring(R->left), *right_s = cord_tostring(R->right);
  strcpy(result, left_s);
  strcat(result, right_s);
  return result;
}

/**
 * @brief Returns the character at a position in a cord
 *
 * @param[in] R  A cord
 * @param[in] i  A position in the cord
 * @return The character at position i
 *
 * @requires i is a valid position in the cord R
 */
char cord_charat(const cord_t *R, size_t i) {
  if (!R->left && !R->right)
    return R->data[i];
  if (i < R->left->len)
    return cord_charat(R->left, i);
  else
    return cord_charat(R->right, i - R->left->len);
}

/**
 * @brief Gets a substring of an existing cord
 *
 * @param[in] R   A cord
 * @param[in] lo  The low index of the substring, inclusive
 * @param[in] hi  The high index of the substring, exclusive
 * @return A cord representing the substring R[lo..hi-1]
 *
 * @requires lo and hi are valid indexes in R, with lo <= hi
 */
const cord_t *cord_sub(const cord_t *R, size_t lo, size_t hi) {
  assert(lo <= hi && hi <= cord_length(R));

  size_t len = hi - lo;
  if (len == 0)
    return NULL;
  else if (len == R->len)
    return R;
  else if (!R->left && !R->right) {
    void *tmp = xmalloc(len + 1);
    memcpy(tmp, R->data + lo, len);
    const cord_t *ans = cord_new(tmp);
    return ans;
  }
  if (hi <= R->left->len)
    return cord_sub(R->left, lo, hi);
  if (lo >= R->left->len)
    return cord_sub(R->right, lo - R->left->len, hi - R->left->len);

  return cord_join(cord_sub(R->left, lo, R->left->len),
                   cord_sub(R->right, 0, hi - R->left->len));
}
