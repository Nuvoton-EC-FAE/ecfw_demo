#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);


   void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf)
   {
       LOG_ERR("Fatal error occurred! Error code: %d", reason);
       LOG_ERR("System will halt after a delay");

       k_busy_wait(100000);  

       // Option：system reboot
       // sys_reboot(SYS_REBOOT_COLD);

       // system idle
       for (;;) {
           k_cpu_idle();
       }
   }


void test_assertions(int value)
{
    __ASSERT(value > 0, "Value must be greater than 0, but got %d", value);

    if (value > 10) {
        __ASSERT_NO_MSG(value <= 10);
    }

    printk("Value is %d\n", value);
}

void main(void)
{
    printk("Assertion Example\n");
    LOG_INF("Starting Zephyr application with assertions enabled");

    test_assertions(5);   // This should pass
    // test_assertions(0);   // This should trigger an assertion failure
    test_assertions(15);  // This should also trigger an assertion failure
}
