/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0

 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <version.h>
#include <app_version.h>
#include <stdio.h>


static int cmd_info_board(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, CONFIG_BOARD);

	return 0;
}

static int cmd_info_version(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Zephyr version %s", KERNEL_VERSION_STRING);

	return 0;
}

static int cmd_info_app_version(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "App version %s %s", APP_VERSION_STRING,STRINGIFY(APP_BUILD_VERSION));

	return 0;
}

static int cmd_shell_help(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "show help: %d", argc);
    if(argc == 1){
        shell_help(sh);
        return SHELL_CMD_HELP_PRINTED;
    }

	for(size_t i=0; i< argc; i++){
		shell_print(sh, "check arg %d: %s", i, argv[i]);
	}

	return 0;
}

#ifdef CORE_DEBUG_COREDUMP
	void trigger_core_dump(void)
	{
		printk("Triggering core dump...\n");
		k_panic();
	}
#endif // CORE_DEBUG_COREDUMP

void main(void)
{
	printk("Nuvoton - Zephyr RTOS: %s\n", CONFIG_BOARD);
	printk("Zephyr version %s", KERNEL_VERSION_STRING);

	#ifdef CONFIG_DEBUG_COREDUMP
		k_sleep(K_SECONDS(5));
		trigger_core_dump();
	#endif // CONFIG_DEBUG_COREDUMP

}

SHELL_STATIC_SUBCMD_SET_CREATE(subinfo,
    SHELL_CMD(board, NULL, "Show board command.", cmd_info_board),
    SHELL_CMD(version, NULL, "Show Zephyr version command.", cmd_info_version),
    SHELL_CMD(app_version, NULL, "Show app version command.", cmd_info_app_version),
    SHELL_SUBCMD_SET_END 
);


//Register shell_demo
SHELL_CMD_REGISTER(shell_demo, &subinfo, "Demo commands", cmd_shell_help);