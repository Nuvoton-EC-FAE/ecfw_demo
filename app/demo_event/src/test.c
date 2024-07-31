#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024
#define TEST_RECEIVER_PRIORITY 7

#define EVENT_A 0x01
#define EVENT_B 0x02
#define EVENT_C 0x04

extern struct k_event test_event;

void test_receiver_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        uint32_t received = k_event_wait(&test_event, EVENT_A | EVENT_B | EVENT_C, true, K_FOREVER);
        
        if (received & EVENT_A) {
            printk("Test Receiver: Received EVENT_A\n");
        }
        if (received & EVENT_B) {
            printk("Test Receiver: Received EVENT_B\n");
        }
        if (received & EVENT_C) {
            printk("Test Receiver: Received EVENT_C\n");
        }
    }
}

void test_receiver1_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        uint32_t received = k_event_wait_all(&test_event, EVENT_A | EVENT_B | EVENT_C, false, K_FOREVER);
        printk("Test receiver1_thread\n");

        if (received == (EVENT_A | EVENT_B | EVENT_C)) {
            printk("Receiver1: Received all events (EVENT_A, EVENT_B, EVENT_C)\n");
            k_event_clear(&test_event, EVENT_A | EVENT_B | EVENT_C);
        }
    }
}

K_THREAD_DEFINE(test_receiver_id, STACK_SIZE, test_receiver_thread, NULL, NULL, NULL, TEST_RECEIVER_PRIORITY, 0, 0);
K_THREAD_DEFINE(test_receiver_id1, STACK_SIZE, test_receiver1_thread, NULL, NULL, NULL, TEST_RECEIVER_PRIORITY, 0, 0);