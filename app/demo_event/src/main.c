#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define STACK_SIZE 1024
#define SENDER_PRIORITY 7
#define RECEIVER_PRIORITY 7
#define RECEIVER_PRIORITY1 6
#define RECEIVER_PRIORITY2 5

#define EVENT_A 0x01
#define EVENT_B 0x02
#define EVENT_C 0x04

K_EVENT_DEFINE(test_event);

void sender_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        k_event_set(&test_event, EVENT_A);
        printk("Sender: Set EVENT_A\n");
        k_msleep(1000);

        k_event_set(&test_event, EVENT_B);
        printk("Sender: Set EVENT_B\n");
        k_msleep(1000);

        k_event_set(&test_event, EVENT_C);
        printk("Sender: Set EVENT_C\n");
        k_msleep(1000);

        k_event_set(&test_event, EVENT_A | EVENT_B);
        printk("Sender: Set EVENT_AB\n");
        k_msleep(1000);

        k_event_set(&test_event, EVENT_A | EVENT_B | EVENT_C);
        printk("Sender: Set EVENT_ABC\n");
        k_msleep(1000);
    }
}

void receiver_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        uint32_t received = k_event_wait(&test_event, EVENT_A | EVENT_B | EVENT_C, true, K_FOREVER);

        if (received & EVENT_A) {
            printk("Main Receiver: Received EVENT_A\n");
        }
        if (received & EVENT_B) {
            printk("Main Receiver: Received EVENT_B\n");
        }
        if (received & EVENT_C) {
            printk("Main Receiver: Received EVENT_C\n");
        }
    }
}

void receiver1_thread(void *p1, void *p2, void *p3)
{
    uint8_t flagA = 0;
    uint8_t flagB = 0;
    uint8_t flagC = 0;

    while (1) {
        uint32_t received = k_event_wait(&test_event, EVENT_A | EVENT_B | EVENT_C, false, K_FOREVER);
        printk("Main receiver1_thread\n");

        if (received & EVENT_A) {
            printk("Main Receiver1: Received EVENT_A\n");
            flagA++;
            if (flagA == 2) {
                k_event_clear(&test_event, EVENT_A);
                flagA = 0;
            }
        }
        if (received & EVENT_B) {
            printk("Main Receiver1: Received EVENT_B\n");
            flagB++;
            if (flagB == 2) {
                k_event_clear(&test_event, EVENT_B);
                flagB = 0;
            }
        }
        if (received & EVENT_C) {
            printk("Main Receiver1: Received EVENT_C\n");
            flagC++;
            if (flagC == 2) {
                k_event_clear(&test_event, EVENT_C);
                flagC = 0;
            }
        }
    }
}

void receiver2_thread(void *p1, void *p2, void *p3)
{
    while (1) {
        uint32_t received = k_event_wait_all(&test_event, EVENT_A | EVENT_B | EVENT_C, true, K_FOREVER);
        printk("Main receiver2_thread\n");

        if (received == (EVENT_A | EVENT_B | EVENT_C)) {
            printk("Receiver2: Received all events (EVENT_A, EVENT_B, EVENT_C)\n");
        }
    }
}

K_THREAD_DEFINE(sender_id, STACK_SIZE, sender_thread, NULL, NULL, NULL, SENDER_PRIORITY, 0, 0);
K_THREAD_DEFINE(receiver_id, STACK_SIZE, receiver_thread, NULL, NULL, NULL, RECEIVER_PRIORITY, 0, 0);
K_THREAD_DEFINE(receiver_id1, STACK_SIZE, receiver1_thread, NULL, NULL, NULL, RECEIVER_PRIORITY1, 0, 0);
K_THREAD_DEFINE(receiver_id2, STACK_SIZE, receiver2_thread, NULL, NULL, NULL, RECEIVER_PRIORITY2, 0, 0);

void main(void)
{
    printk("Main: Event API test started\n");
}
