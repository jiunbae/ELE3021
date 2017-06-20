#include <linux/kernel.h>
#include <linux/module.h>

int hello_module_init(void) {
	printk(KERN_EMERG "hello module~! im in kernel \n");
	return 0;
}

void hello_module_cleanup(void) {
	printk("<0>Bye end of module~!\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);

MODULE_LICENSE("GPL");

