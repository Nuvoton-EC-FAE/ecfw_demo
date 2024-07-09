/*
 * Copyright Meta Platforms, Inc. and its affiliates.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <zephyr/debug/coredump.h>
#include <zephyr/sys/printk.h>
#include <zephyr/logging/log.h>
#include <coredump_other.h>


LOG_MODULE_REGISTER(coredump, CONFIG_KERNEL_LOG_LEVEL);


static int error;
static bool is_valid;


// int hex2char(uint8_t x, char *c)
// {
// 	if (x <= 9) {
// 		*c = x + (char)'0';
// 	} else  if (x <= 15) {
// 		*c = x - 10 + (char)'a';
// 	} else {
// 		return -EINVAL;
// 	}

// 	return 0;
// }

static void coredump_empty_backend_start(void)
{
    /* Reset error, is_valid */
    error = 0;
    is_valid = false;
}

static void coredump_empty_backend_end(void)
{
    is_valid = true;
}

static void coredump_empty_backend_buffer_output(uint8_t *buf, size_t buflen)
{
    /* Output the coredump buffer through printk */
    uint8_t log_ptr = 0;
	size_t remaining = buflen;
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

}

static int coredump_empty_backend_query(enum coredump_query_id query_id, void *arg)
{
    int ret;

    switch (query_id) {
    case COREDUMP_QUERY_GET_ERROR:
        ret = error;
        break;
    case COREDUMP_QUERY_HAS_STORED_DUMP:
        ret = is_valid ? 1 : 0;
        break;
    case COREDUMP_QUERY_GET_STORED_DUMP_SIZE:
        ret = 0;
        break;
    default:
        ret = -ENOTSUP;
        break;
    }

    return ret;
}

static int coredump_empty_backend_cmd(enum coredump_cmd_id cmd_id, void *arg)
{
    int ret;

    switch (cmd_id) {
    case COREDUMP_CMD_CLEAR_ERROR:
        error = 0;
        ret = 0;
        break;
    case COREDUMP_CMD_VERIFY_STORED_DUMP:
        ret = is_valid ? 1 : 0;
        break;
    case COREDUMP_CMD_INVALIDATE_STORED_DUMP:
        is_valid = false;
        ret = 0;
        break;
    case COREDUMP_CMD_ERASE_STORED_DUMP:
        is_valid = false;
        ret = 0;
        break;
    default:
        ret = -ENOTSUP;
        break;
    }

    return ret;
}

struct coredump_backend_api coredump_backend_other = {
    .start = coredump_empty_backend_start,
    .end = coredump_empty_backend_end,
    .buffer_output = coredump_empty_backend_buffer_output,
    .query = coredump_empty_backend_query,
    .cmd = coredump_empty_backend_cmd,
};
