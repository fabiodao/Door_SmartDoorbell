#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd5eaac33, "module_layout" },
	{ 0x3bf54df1, "cdev_del" },
	{ 0xe1b161, "cdev_init" },
	{ 0xe0387587, "gpio_to_desc" },
	{ 0x754e893f, "device_destroy" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x5f754e5a, "memset" },
	{ 0xc5850110, "printk" },
	{ 0x1d37eeed, "ioremap" },
	{ 0xe5c08941, "device_create" },
	{ 0xf18fa750, "pid_task" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xf30323a5, "cdev_add" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x72b27029, "init_pid_ns" },
	{ 0x5d48d0bd, "gpiod_to_irq" },
	{ 0xf4f8bfd7, "send_sig_info" },
	{ 0xedc03953, "iounmap" },
	{ 0xd5f79510, "class_destroy" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xf5991479, "gpiod_get_raw_value" },
	{ 0x9d47373c, "__class_create" },
	{ 0x9a8f962b, "find_pid_ns" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc1514a3b, "free_irq" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "341C2D5D528CD1806A18D08");
