#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#define UART_NODE DT_NODELABEL(uart2)
static const struct device *uart_dev;

#define MSG_SIZE 32
static uint8_t rx_buf[MSG_SIZE];
static uint8_t rx_next_buf[MSG_SIZE];

// 添加緩存來保存接收到的完整數據
static struct {
    uint8_t buffer[256];  // 用於存儲完整的接收數據
    uint16_t pos;         // 當前位置指針
} rx_data = {0};

static void uart_callback(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type)
    {
        case UART_TX_DONE:
            printk("UART TX Complete\n");
            break;

        case UART_RX_RDY:
            // 確保不會溢出
            if ((rx_data.pos + evt->data.rx.len) < sizeof(rx_data.buffer)) {
                // 將新接收的數據追加到緩存中
                memcpy(&rx_data.buffer[rx_data.pos],
                       &evt->data.rx.buf[evt->data.rx.offset],
                       evt->data.rx.len);
                
                rx_data.pos += evt->data.rx.len;
                
                // 檢查是否接收到完整的數據包（這裡假設以\r\n為結束標記）
                if (rx_data.pos >= 2 &&
                    rx_data.buffer[rx_data.pos-2] == '\r' &&
                    rx_data.buffer[rx_data.pos-1] == '\n') {
                    
                    // 打印完整的數據包
                    printk("Received complete data (len:%d):\n", rx_data.pos);
                    for (int i = 0; i < rx_data.pos; i++) {
                        printk("%c", rx_data.buffer[i]);
                    }
                    printk("\n");
                    
                    // 重置緩存
                    rx_data.pos = 0;
                    memset(rx_data.buffer, 0, sizeof(rx_data.buffer));
                }
            } else {
                // 緩存溢出處理
                printk("Buffer overflow, resetting buffer\n");
                rx_data.pos = 0;
                memset(rx_data.buffer, 0, sizeof(rx_data.buffer));
            }
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

    // 定義五種不同大小的資料
    static uint8_t tx_buf1[] = "Hello from UART1\r\n";
    static uint8_t tx_buf2[] = "Short\r\n";
    static uint8_t tx_buf3[] = "This is a longer message from UART1\r\n";
    static uint8_t tx_buf4[] = "1234567890\r\n";
    static uint8_t tx_buf5[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n";

    // 傳送五種不同大小的資料
    ret = uart_tx(uart_dev, tx_buf1, strlen(tx_buf1), SYS_FOREVER_MS);
    if (ret) {
        printk("Failed to send data: %d\n", ret);
        return;
    }

    ret = uart_tx(uart_dev, tx_buf2, strlen(tx_buf2), SYS_FOREVER_MS);
    if (ret) {
        printk("Failed to send data: %d\n", ret);
        return;
    }

    ret = uart_tx(uart_dev, tx_buf3, strlen(tx_buf3), SYS_FOREVER_MS);
    if (ret) {
        printk("Failed to send data: %d\n", ret);
        return;
    }

    ret = uart_tx(uart_dev, tx_buf4, strlen(tx_buf4), SYS_FOREVER_MS);
    if (ret) {
        printk("Failed to send data: %d\n", ret);
        return;
    }

    ret = uart_tx(uart_dev, tx_buf5, strlen(tx_buf5), SYS_FOREVER_MS);
    if (ret) {
        printk("Failed to send data: %d\n", ret);
        return;
    }

    while (1) {
        k_sleep(K_MSEC(5000));

        // 週期性傳送其中一種資料
        ret = uart_tx(uart_dev, tx_buf1, strlen(tx_buf1), SYS_FOREVER_MS);
        if (ret) {
            printk("Failed to send periodic data: %d\n", ret);
        }
    }
}
