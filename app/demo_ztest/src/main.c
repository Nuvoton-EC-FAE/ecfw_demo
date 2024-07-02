/*
 * Copyright (c) 2024 NUVOTON Inc.
 *
 * SPDX-License-Identifier: Apache-2.0

 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <version.h>
#include <app_version.h>
#include <stdio.h>
#include <zephyr/sys/printk.h>
//For ztest
#include <zephyr/ztest.h>



LOG_MODULE_REGISTER(main, CONFIG_EC_LOG_LEVEL);

#include <zephyr/kernel.h>
#include <zephyr/ztest.h>

// Simulated temperature sensor system
static int current_temperature = 20;

static int read_temperature(void) {
    return current_temperature;
}

static void set_temperature(int temp) {
    current_temperature = temp;
}

// Predicate function
static bool should_run_temp_tests(const void *state) {
    return true;  // In real applications, might check hardware status
}

// Suite setup function
static void *temp_suite_setup(void) {
    printk("Setting up temperature test suite\n");
    set_temperature(20);  // Initialize temperature
    return NULL;
}

// Run before each test
static void temp_test_before(void *fixture) {
    printk("Before test: resetting temperature\n");
    set_temperature(20);
}

// Run after each test
static void temp_test_after(void *fixture) {
    printk("After test: temperature is %d\n", read_temperature());
}

// Suite cleanup function
static void temp_suite_teardown(void *fixture) {
    printk("Tearing down temperature test suite\n");
}

// Define test suite
ZTEST_SUITE(temperature_tests, should_run_temp_tests, temp_suite_setup,
            temp_test_before, temp_test_after, temp_suite_teardown);

// Test: Read temperature
ZTEST(temperature_tests, test_read_temperature) {
    zassert_equal(read_temperature(), 20, "Initial temperature should be 20");
}

// Test: Set temperature
ZTEST(temperature_tests, test_set_temperature) {
    set_temperature(25);
    zassert_equal(read_temperature(), 25, "Temperature should be set to 25");
}

void test_main(void) {
    printk("Starting temperature sensor tests\n");
    ztest_run_all(NULL, true, 1, 1);
    printk("All tests completed\n");
}