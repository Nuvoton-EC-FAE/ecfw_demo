/* Copyright (c) 2025 Your Organization
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LIBDEMO_MATH_OPS_H_
#define LIBDEMO_MATH_OPS_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Calculate sum of an integer array
 *
 * @param arr Pointer to array of int32_t values
 * @param len Length of the array
 * @return Sum of all elements in the array
 *
 * @note Returns 0 if arr is NULL or len is 0
 */
int32_t libdemo_array_sum(const int32_t *arr, size_t len);

/**
 * @brief Calculate average of an integer array
 *
 * @param arr Pointer to array of int32_t values
 * @param len Length of the array
 * @return Average value (integer division)
 *
 * @note Returns 0 if arr is NULL or len is 0
 */
int32_t libdemo_array_average(const int32_t *arr, size_t len);

/**
 * @brief Find maximum value in an integer array
 *
 * @param arr Pointer to array of int32_t values
 * @param len Length of the array
 * @return Maximum value in the array
 *
 * @note Returns INT32_MIN if arr is NULL or len is 0
 */
int32_t libdemo_array_max(const int32_t *arr, size_t len);

/**
 * @brief Find minimum value in an integer array
 *
 * @param arr Pointer to array of int32_t values
 * @param len Length of the array
 * @return Minimum value in the array
 *
 * @note Returns INT32_MAX if arr is NULL or len is 0
 */
int32_t libdemo_array_min(const int32_t *arr, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* LIBDEMO_MATH_OPS_H_ */
