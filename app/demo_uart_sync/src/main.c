#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#define UART_NODE DT_NODELABEL(uart2)
static const struct device *uart_dev;

#define MSG_SIZE 32
static uint8_t rx_buf[MSG_SIZE];
static uint8_t rx_next_buf[MSG_SIZE];
static uint8_t tx_buf[] = "Hello from UART1\r\n";

static void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type) 
    {
        case UART_TX_DONE:
            printk("UART TX Complete\n");
            break;

        case UART_RX_RDY:
            printk("Received data: (len:%d)\n", evt->data.rx.len);
            for (int i = 0; i < evt->data.rx.len; i++) {
                printk("%c", evt->data.rx.buf[evt->data.rx.offset + i]);
            }
            printk("\n");
            break;

        case UART_RX_BUF_REQUEST:
            uart_rx_buf_rsp(dev, rx_next_buf, sizeof(rx_next_buf));
            break;

        case UART_RX_BUF_RELEASED:
            printk("Buffer released\n");
            break;

        case UART_RX_DISABLED:
            printk("RX disabled, re-enabling RX\n");
            uart_rx_enable(dev, rx_buf, sizeof(rx_buf), SYS_FOREVER_US);
            break;

        case UART_RX_STOPPED:
            printk("RX stopped, re-enabling RX\n");
            uart_rx_enable(dev, rx_buf, sizeof(rx_buf), SYS_FOREVER_US);
            break;
    }
}

void main(void)
{
    int ret;

    uart_dev = DEVICE_DT_GET(UART_NODE);
    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready\n");
        return;
    }

    ret = uart_callback_set(uart_dev, uart_callback, NULL);
    if (ret) {
        printk("Failed to set callback: %d\n", ret);
        return;
    }

    ret = uart_rx_enable(uart_dev, rx_buf, sizeof(rx_buf), SYS_FOREVER_US);
    if (ret) {
        printk("Failed to enable RX: %d\n", ret);
        return;
    }

    ret = uart_tx(uart_dev, tx_buf, strlen(tx_buf), SYS_FOREVER_MS);
    if (ret) {
        printk("Failed to send data: %d\n", ret);
        return;
    }

    while (1) {
        k_sleep(K_MSEC(5000));

        ret = uart_tx(uart_dev, tx_buf, strlen(tx_buf), SYS_FOREVER_MS);
        if (ret) {
            printk("Failed to send periodic data: %d\n", ret);
        }
    }
}
