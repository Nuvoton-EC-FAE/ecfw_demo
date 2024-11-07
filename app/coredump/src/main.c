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
// #include <coredump_other.h>


#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

LOG_MODULE_REGISTER(demo_coredump, CONFIG_EC_LOG_LEVEL);


static struct k_thread thread1;
static K_THREAD_STACK_DEFINE(stack1, STACK_SIZE);

static int counter1 = 0;


void func_3(uint32_t *addr)
{
#if defined(CONFIG_BOARD_M2GL025_MIV) || \
	defined(CONFIG_BOARD_HIFIVE1) || \
	defined(CONFIG_BOARD_LONGAN_NANO) || \
	defined(CONFIG_BOARD_LONGAN_NANO_LITE) || \
	defined(CONFIG_BOARD_QEMU_XTENSA) || \
	defined(CONFIG_SOC_FAMILY_INTEL_ADSP)
	ARG_UNUSED(addr);
	/* Call k_panic() directly so Renode doesn't pause execution.
	 * Needed on ADSP as well, since null pointer derefence doesn't
	 * fault as the lowest memory region is writable. SOF uses k_panic
	 * a lot, so it's good to check that it causes a coredump.
	 */
	k_panic();
#elif !defined(CONFIG_CPU_CORTEX_M)
	/* For null pointer reference */
	*addr = 0;
#else
	ARG_UNUSED(addr);
	/* Dereferencing null-pointer in TrustZone-enabled
	 * builds may crash the system, so use, instead an
	 * undefined instruction to trigger a CPU fault.
	 */
	__asm__ volatile("udf #0" : : : );
#endif
}

void func_2(uint32_t *addr)
{
    int c2 = 0;
    while (1) {
        c2++;
        if (c2 >= 20) {
            printk("counter1 = %d\n", counter1);
	        func_3(addr);
        }
    } 
}

void func_1(uint32_t *addr)
{
    int c1 = 0;
    while (1) {
        c1++;
        if (c1 >= 10) {
            printk("counter1 = %d\n", counter1);
	        func_2(addr);
        }
    }    
}

void thread1_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        counter1++;
        printk("Thread 1: counter1 = %d\n", counter1);
        if (counter1 >= 20) {

            printk("counter1 = %d\n", counter1);
	        func_1(0);
        }
        k_sleep(K_MSEC(500));
    }
}



void main(void)
{
    k_tid_t tid1 = k_thread_create(&thread1, stack1, STACK_SIZE, thread1_entry, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);

    k_thread_join(tid1, K_FOREVER);
}
