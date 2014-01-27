#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7dc216d9, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x2fbfdab3, __VMLINUX_SYMBOL_STR(usb_deregister) },
	{ 0xbb521d73, __VMLINUX_SYMBOL_STR(usb_register_driver) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0xf6c9030a, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x1cba218d, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x924236ee, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xa59dfc19, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x346c7121, __VMLINUX_SYMBOL_STR(dev_set_drvdata) },
	{ 0x2a83b971, __VMLINUX_SYMBOL_STR(usb_get_dev) },
	{ 0xa6327d27, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xdf08d9a5, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x2221a11b, __VMLINUX_SYMBOL_STR(usb_put_dev) },
	{ 0x70d0f033, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x7565e694, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x9dea5d62, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x44d787a, __VMLINUX_SYMBOL_STR(dev_get_drvdata) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x65d4ac63, __VMLINUX_SYMBOL_STR(usb_interrupt_msg) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v1CBEp0003d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "A16A598FDAC2DF6F5E1E39F");
