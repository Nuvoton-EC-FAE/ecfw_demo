#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

#define STACK_SIZE 1024
#define THREAD_PRIORITY 5

LOG_MODULE_REGISTER(demo_threadinfo, CONFIG_EC_LOG_LEVEL);

static struct k_thread thread1;
static struct k_thread thread2;
static K_THREAD_STACK_DEFINE(stack1, STACK_SIZE);
static K_THREAD_STACK_DEFINE(stack2, STACK_SIZE);
static int counter1 = 0;
static int counter2 = 0;
static int counter3 = 0;

// 模擬大量運算函數
void heavy_computation(int count)
{
    volatile double result = 0;
    for (int i = 1; i < count; i++) {
        result += i*i;
    }
    printk("Heavy computation result: %f\n", result);
}

void thread1_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        counter1++;
        printk("Thread 1: counter1 = %d\n", counter1);
        if (counter1 > 5) {
            printk("Thread 1: Starting heavy computation\n");
            heavy_computation(50);
            counter1 = 0;
        }
        k_sleep(K_SECONDS(1));
    }
}

void thread2_entry(void *p1, void *p2, void *p3)
{
    while (1) {
        counter2++;
        printk("Thread 2: counter2 = %d\n", counter2);
        if (counter2 >= 3) {
            printk("Thread 2: Starting heavy computation\n");
            heavy_computation(100);
            counter2 = 0;
        }
        k_sleep(K_SECONDS(2));
    }
}

// 獲取線程狀態字符串
const char *get_thread_state_string(k_tid_t thread_id)
{
    uint32_t thread_state = thread_id->base.thread_state;
    
    if (thread_state & _THREAD_PRESTART) return "prestart";
    if (thread_state & _THREAD_SUSPENDED) return "suspended";
    if (thread_state & _THREAD_PENDING) return "pending";
    if (thread_state & _THREAD_QUEUED) return "queued";
    if (thread_state & _THREAD_DEAD) return "dead";
    if (thread_state == 0) return "running"; // 假設 0 表示正在運行
    
    return "unknown";
}

// 獲取線程選項字符串
char *get_thread_options_string(k_tid_t thread_id, char *options_str)
{
    uint32_t thread_options = thread_id->base.user_options;
    options_str[0] = '\0';

    if (thread_options & K_ESSENTIAL) {
        strcat(options_str, "E");
    }
    if (thread_options & K_FP_REGS) {
        strcat(options_str, "F");
    }
    if (thread_options & K_SSE_REGS) {
        strcat(options_str, "S");
    }
    if (thread_options & K_USER) {
        strcat(options_str, "U");
    }
    if (options_str[0] == '\0') {
        strcpy(options_str, "N/A");
    }

    return options_str;
}

// 線程信息打印函數
void print_thread_info(const struct k_thread *thread, void *user_data)
{
    k_tid_t thread_id = (k_tid_t)thread;
    const char *thread_name = k_thread_name_get(thread_id);
    int prio = k_thread_priority_get(thread_id);
    const char *state = get_thread_state_string(thread_id);
    size_t stack_size = 0;
    size_t stack_used = 0;
    char options_str[10];

    if (thread->stack_info.start) {
        stack_size = thread->stack_info.size;
        #if defined(CONFIG_THREAD_STACK_INFO)
        uint32_t stack_space = 0;
        k_thread_stack_space_get(thread_id, &stack_space);
        stack_used = stack_size - stack_space;
        #else
        stack_used = 0; // 如果未啟用 CONFIG_THREAD_STACK_INFO，無法獲取準確的stack使用情況
        #endif
    }

    double stack_usage_percent = (stack_size > 0) ? ((float)stack_used / stack_size) * 100 : 0;

    printk("%-15s| %p | %4d | %-10s | %10zu | %10zu | %6.2f | %s\n",
           thread_name ? thread_name : "N/A",
           thread_id,
           prio,
           state,
           stack_size,
           stack_used,
           stack_usage_percent,
           get_thread_options_string(thread_id, options_str));
}

void dump_all_threads(void)
{
    printk("Thread Information:\n");
    printk("%-15s| %-10s | %4s | %-10s | %10s | %10s | %6s | %s\n",
           "thread_name", "id", "prio", "state", "stack size", "stack used", "%", "options");
    printk("------------------------------------------------------------------------------------\n");
    k_thread_foreach(print_thread_info, NULL);
    printk("------------------------------------------------------------------------------------\n");
}

void main(void)
{
    k_tid_t tid1 = k_thread_create(&thread1, stack1, STACK_SIZE, thread1_entry, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);
    k_tid_t tid2 = k_thread_create(&thread2, stack2, STACK_SIZE, thread2_entry, NULL, NULL, NULL, THREAD_PRIORITY, 0, K_NO_WAIT);

    k_thread_name_set(tid1, "Thread 1");
    k_thread_name_set(tid2, "Thread 2");

    while (1) {
        counter3++;
        printk("Main Thread: counter3 = %d\n", counter3);

        dump_all_threads();
        k_sleep(K_SECONDS(5));
    }
}