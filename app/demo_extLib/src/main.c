/* Copyright (c) 2025 Your Organization
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <libdemo/math_ops.h>

LOG_MODULE_REGISTER(demo_extLib, LOG_LEVEL_INF);

int main(void)
{
	LOG_INF("=================================");
	LOG_INF("libdemo Demo Application Starting");
	LOG_INF("=================================");

	/* Test data */
	int32_t numbers[] = {10, 20, 30, 40, 50};
	size_t count = ARRAY_SIZE(numbers);

	/* Display test data */
	LOG_INF("Test array: [10, 20, 30, 40, 50]");
	LOG_INF("Array size: %zu elements", count);
	LOG_INF("");

	/* Test: Sum */
	int32_t sum = libdemo_array_sum(numbers, count);
	LOG_INF("Sum: %d", sum);

	/* Test: Average */
	int32_t avg = libdemo_array_average(numbers, count);
	LOG_INF("Average: %d", avg);

	/* Test: Max */
	int32_t max = libdemo_array_max(numbers, count);
	LOG_INF("Max: %d", max);

	/* Test: Min */
	int32_t min = libdemo_array_min(numbers, count);
	LOG_INF("Min: %d", min);

	LOG_INF("");
	LOG_INF("=================================");
	LOG_INF("libdemo Demo Completed!");
	LOG_INF("=================================");

	/* Test with edge cases */
	LOG_INF("");
	LOG_INF("Edge case tests:");

	/* Empty array */
	int32_t empty_sum = libdemo_array_sum(NULL, 0);
	LOG_INF("Sum of NULL/empty array: %d (expected: 0)", empty_sum);

	/* Single element */
	int32_t single[] = {42};
	int32_t single_sum = libdemo_array_sum(single, 1);
	int32_t single_avg = libdemo_array_average(single, 1);
	LOG_INF("Single element [42] - Sum: %d, Avg: %d", single_sum, single_avg);

	/* Negative numbers */
	int32_t negatives[] = {-10, -20, -30};
	int32_t neg_sum = libdemo_array_sum(negatives, 3);
	int32_t neg_min = libdemo_array_min(negatives, 3);
	LOG_INF("Negative array [-10,-20,-30] - Sum: %d, Min: %d", neg_sum, neg_min);

	LOG_INF("");
	LOG_INF("All tests completed successfully!");

	return 0;
}
