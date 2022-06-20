#include <linux/kernel.h>       // piszemy część kernela
#include <linux/module.h>       // tutaj jest to moduł
#include <linux/proc_fs.h>      // który ma obsługiwać PROCFS
#include <linux/uaccess.h>
#include <asm/uaccess.h>        // i posługuje się: copy_from_user() i copy_to_user()
#include <asm/io.h>             // a całość komunikować się będzie z peryferiami:  writel() i readl()

#define BUF_SIZE		100

#define SYKT_GPIO_ADDR_SPACE	0x00100000
#define MEM_SIZE		16
#define FINISHER_FAIL		(0x3333)

#define SYKT_GPIO_OFFSET_1	(0x210)		// oś 1
#define SYKT_GPIO_OFFSET_2	(0x214)		// oś 2
#define SYKT_GPIO_OFFSET_3	(0x218)		// sygnał INT

void __iomem *finisher;
void __iomem *axis_1;
void __iomem *axis_2;
void __iomem *counter;


static ssize_t read_func(struct file *file, char __user *ubuf, size_t count, loff_t *offset, void __iomem *ptr)
{
	char buf[BUF_SIZE];
	int len = 0;

	if(*offset > 0 || count < BUF_SIZE)
        	return 0;

	len = snprintf(buf, BUF_SIZE, "%x\n", readl(ptr));

	if(copy_to_user(ubuf, buf, len))     // kopiowanie z przestrzeni jądra do użytkownika
		return -EFAULT;

	*offset = len;
	return len;
}


static ssize_t write_func(struct file *file, const char __user *buf, size_t len, loff_t *offset, void __iomem *ptr)
{
	char arr[BUF_SIZE];
	unsigned long val;

	// printk(KERN_INFO "MY Write PROC\n");
	if(len > BUF_SIZE)
		return 0;

	if(copy_from_user(arr, buf, len))
		return -EFAULT;


	if (sscanf(arr, "%lx", &val) != 1)
		return -EFAULT;

	// printk(KERN_INFO "Zostalo zapisane: %lx\n", val);

	writel(val, ptr);

	return len;
}

static ssize_t read_axis_1(struct file *file, char __user *buf, size_t len, loff_t *off){ return read_func(file, buf, len, off, axis_1); }
static ssize_t read_axis_2(struct file *file, char __user *buf, size_t len, loff_t *off){ return read_func(file, buf, len, off, axis_2); }
static ssize_t read_counter(struct file *file, char __user *buf, size_t len, loff_t *off){ return read_func(file, buf, len, off, counter); }

static ssize_t write_axis_1(struct file *file, const char __user *buf, size_t len, loff_t *off){ return write_func(file, buf, len, off, axis_1); }
static ssize_t write_axis_2(struct file *file, const char __user *buf, size_t len, loff_t *off){ return write_func(file, buf, len, off, axis_2); }
static ssize_t write_counter(struct file *file, const char __user *buf, size_t len, loff_t *off){ return write_func(file, buf, len, off, counter); }


static struct file_operations axis_1_ops =
{
    .owner = THIS_MODULE,
    .read = read_axis_1,
    .write = write_axis_1,
};

static struct file_operations axis_2_ops =
{
    .owner = THIS_MODULE,
    .read = read_axis_2,
    .write = write_axis_2,
};

static struct file_operations counter_ops =
{
    .owner = THIS_MODULE,
    .read = read_counter,
    .write = write_counter,
};

struct proc_dir_entry *parent;
static struct proc_dir_entry *ax_1, *ax_2, *cnt;

static int procfs_init(void)
{
	printk(KERN_ALERT "Starting the module...\n");
	parent = proc_mkdir("sykom", NULL); // nasz katalog w /proc

	// pliki w /proc/sykom do poszczególnych elementów
	ax_1 = proc_create("rj9del1", 0660, parent, &axis_1_ops);
	if(ax_1 == NULL)
	{
		printk(KERN_ALERT "Couldn't create necessary PROCFS files, exiting...\n");
		remove_proc_entry("sykom", NULL);
		return -ENOENT;
	}

	ax_2 = proc_create("rj9del2", 0660, parent, &axis_2_ops);
	if(ax_2 == NULL)
	{
		printk(KERN_ALERT "Couldn't create necessary PROCFS files, exiting...\n");
		proc_remove(ax_1);
		remove_proc_entry("sykom", NULL);
		return -ENOENT;
	}

	cnt = proc_create("rj9del3", 0660, parent, &counter_ops);
	if(cnt == NULL)
	{
		printk(KERN_ALERT "Couldn't create necessary PROCFS files, exiting...\n");
		proc_remove(ax_1);
		proc_remove(ax_2);
		remove_proc_entry("sykom", NULL);
		return -ENOENT;
	}
	// przypisanie pointerów
	finisher = ioremap(SYKT_GPIO_ADDR_SPACE, MEM_SIZE);
	axis_1 = ioremap(SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_OFFSET_1, MEM_SIZE);
	axis_2 = ioremap(SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_OFFSET_2, MEM_SIZE);
	counter = ioremap(SYKT_GPIO_ADDR_SPACE+SYKT_GPIO_OFFSET_3, MEM_SIZE);

	// weryfikacja czy procesy zostały pomyślnie utworzone
	printk(KERN_ALERT "Module successfully started!\n");
	return 0;
}

static void procfs_cleanup(void)
{
	printk(KERN_WARNING "Stopping the module...\n");

	proc_remove(ax_1);
	proc_remove(ax_2);
	proc_remove(cnt);
	remove_proc_entry("sykom", NULL);

	writel(FINISHER_FAIL | ((127)<<16), finisher); // zakończenie pracy qemu

	iounmap(axis_1);
	iounmap(axis_2);
	iounmap(counter);
	iounmap(finisher);

	printk(KERN_WARNING "Module successfully stopped!\n");
}

module_init(procfs_init);
module_exit(procfs_cleanup);

MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");

