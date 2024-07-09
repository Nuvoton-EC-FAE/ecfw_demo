/* Length of buffer of printable size */
#define LOG_BUF_SZ		64

/* Length of buffer of printable size plus null character */
#define LOG_BUF_SZ_RAW		(LOG_BUF_SZ + 1)

/* Log Buffer */
static char log_buf[LOG_BUF_SZ_RAW];