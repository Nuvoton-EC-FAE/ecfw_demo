/*
 * Copyright (c) 2024 NUVOTON Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/debug/coredump.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <coredump_other.h>


#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

LOG_MODULE_REGISTER(demo_coredump, CONFIG_EC_LOG_LEVEL);


static struct k_thread thread1;
static struct k_thread thread2;
static K_THREAD_STACK_DEFINE(stack1, STACK_SIZE);
static K_THREAD_STACK_DEFINE(stack2, STACK_SIZE);

static int counter1 = 0;
static int counter2 = 0;
static int error;

void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *pEsf)
{
    ARG_UNUSED(reason);
    ARG_UNUSED(pEsf);
    uint8_t *buf;
    LOG_ERR("Coredump triggered! Reason: %u\n", reason);
    // Simulate coredump output
    for (size_t i = 0; i < sizeof(*pEsf); i++) {
        // LOG_ERR("%s", ((uint8_t *)pEsf)[i]);
        buf = (uint8_t *)pEsf;
    }
    // LOG_ERR("\n");
    uint8_t log_ptr = 0;
	size_t remaining = sizeof(*pEsf);
	size_t i = 0;

    while (remaining > 0) {
		if (hex2char(buf[i] >> 4, &log_buf[log_ptr]) < 0) {
			error = -EINVAL;
			break;
		}
		log_ptr++;

		if (hex2char(buf[i] & 0xf, &log_buf[log_ptr]) < 0) {
			error = -EINVAL;
			break;
		}
		log_ptr++;

		i++;
		remaining--;

		if ((log_ptr >= LOG_BUF_SZ) || (remaining == 0)) {
			log_buf[log_ptr] = '\0';
			LOG_ERR("%s", log_buf);
			log_ptr = 0;
		}
	}


    // Reset the error status to avoid halt
    reason = 0;
}

void thread1_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        counter1++;
        printk("Thread 1: counter1 = %d\n", counter1);
        if (counter1 >= 20) {
            coredump_cmd(COREDUMP_CMD_CLEAR_ERROR, NULL);
            coredump_query(COREDUMP_QUERY_HAS_STORED_DUMP, NULL);
            k_oops();
        }
        k_sleep(K_MSEC(1000));
    }
}

void thread2_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        counter2++;
        printk("Thread 2: counter2 = %d\n", counter2);
        if (counter2 >= 10) {
            coredump_cmd(COREDUMP_CMD_CLEAR_ERROR, NULL);
            coredump_query(COREDUMP_QUERY_HAS_STORED_DUMP, NULL);
            k_oops();
        }
        k_sleep(K_MSEC(500));
    }
}

void main(void)
{
    k_tid_t tid1 = k_thread_create(&thread1, stack1, STACK_SIZE, thread1_entry, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    k_tid_t tid2 = k_thread_create(&thread2, stack2, STACK_SIZE, thread2_entry, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);

    k_thread_join(tid1, K_FOREVER);
    k_thread_join(tid2, K_FOREVER);
}
