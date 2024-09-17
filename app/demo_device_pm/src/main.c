/*
 * Copyright (c) 2018 Intel Corporation.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/device_runtime.h>
#include "dummy_driver.h"

#define STACK_SIZE 1024
#define THREAD_A_PRIORITY 5
#define THREAD_B_PRIORITY 4

K_THREAD_STACK_DEFINE(thread_a_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_b_stack, STACK_SIZE);

struct k_thread thread_a_data;
struct k_thread thread_b_data;

const struct device *dummy_dev;

void thread_a_entry(void *p1, void *p2, void *p3)
{
    struct dummy_driver_api *api;
    int ret;
    uint32_t val;

    while (1) {
        printk("Thread A: Active\n");

        api = (struct dummy_driver_api *)dummy_dev->api;
        ret = api->open(dummy_dev);
        if (ret) {
            printk("Failed to open device\n");
            continue;
        }

        val = 10;
        ret = api->write(dummy_dev, val);
        ret = api->read(dummy_dev, &val);
        ret = api->close(dummy_dev);

        printk("Thread A: Entering PM state\n");
        pm_device_runtime_put(dummy_dev);
        k_sleep(K_SECONDS(20));
    }
}

void thread_b_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        k_sleep(K_SECONDS(10));
        enum pm_device_state state;
        pm_device_state_get(dummy_dev, &state);
        if (state != PM_DEVICE_STATE_ACTIVE) {
            printk("Thread B: Waking up Thread A\n");
            pm_device_runtime_get(dummy_dev);
        }
    }
}

int main(void)
{
    printk("Device PM sample app start\n");

    dummy_dev = device_get_binding(DUMMY_DRIVER_NAME);
    if (!dummy_dev) {
        printk("Error: Device %s not found\n", DUMMY_DRIVER_NAME);
        return -1;
    }

    k_thread_create(&thread_a_data, thread_a_stack, STACK_SIZE,
                    thread_a_entry, NULL, NULL, NULL,
                    THREAD_A_PRIORITY, 0, K_NO_WAIT);

    k_thread_create(&thread_b_data, thread_b_stack, STACK_SIZE,
                    thread_b_entry, NULL, NULL, NULL,
                    THREAD_B_PRIORITY, 0, K_NO_WAIT);

    return 0;
}
