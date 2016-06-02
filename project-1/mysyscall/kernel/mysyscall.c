#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

asmlinkage int sys_mysyscall(const char * from_user, unsigned long len) {
  char * buffer = NULL;
  buffer = kmalloc(len+1, GFP_KERNEL);
  if (buffer == NULL) {
    printk("mysyscall: kmalloc() failed.\n");
    return -1;
  }
  if (copy_from_user(buffer, from_user, len) != 0) {
    printk("mysyscall: copy_from_user() failed.\n");
    kfree(buffer);
    return -1;
  }
  buffer[len] = '\0';
  printk("%s\n", buffer);
  kfree(buffer);
  return 0;
}

