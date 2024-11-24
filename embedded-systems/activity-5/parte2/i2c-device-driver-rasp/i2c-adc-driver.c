#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>    //kmalloc()
#include <linux/uaccess.h> //copy_to/from_user()

#define ESP32_ADDR (0x58)
#define ESP32_NAME ("ADC_ESP32")
dev_t dev = 0;
static int ESP32_connected = 0;
static struct class *dev_class;
static struct cdev i2c_adc_cdev;

static int __init i2c_adc_driver_init(void);
static void __exit i2c_adc_driver_exit(void);

// I2C
static struct i2c_board_info i2c_board_inf = {
    I2C_BOARD_INFO(ESP32_NAME, ESP32_ADDR)};
static struct i2c_adapter *i2c_adapt = NULL;
static struct i2c_client *i2c_client = NULL;
// Funcoes de Probe e Remove do I2C
static int i2c_adc_probe(struct i2c_client *client,
                         const struct i2c_device_id *id) {
  pr_info("I2C Device : Found!\n");
  ESP32_connected = 1;
  return 0;
}
static int i2c_adc_remove(struct i2c_client *client) {
  pr_info("I2C Device : Removed!\n");
  ESP32_connected = 0;
  return 0;
}
static const struct i2c_device_id i2c_adc_id[] = {{ESP32_NAME, 0}, {}};
MODULE_DEVICE_TABLE(i2c, i2c_adc_id);
// Struct para receber o dispositivo i2c correto
static struct i2c_driver i2c_adc_driver = {
    .driver =
        {
            .name = ESP32_NAME,
            .owner = THIS_MODULE,
        },
    .probe = i2c_adc_probe,
    .remove = i2c_adc_remove,
    .id_table = i2c_adc_id,
};

// Relacionado a interacao do driver como um arquivo
static int i2c_adc_open(struct inode *inode, struct file *file);
static int i2c_adc_release(struct inode *inode, struct file *file);
static ssize_t i2c_adc_read(struct file *filp, char __user *buf, size_t len,
                            loff_t *off);
static ssize_t i2c_adc_write(struct file *filp, const char *buf, size_t len,
                             loff_t *off);

// Criacao do arquivo para pedido
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = i2c_adc_read,
    .write = i2c_adc_write,
    .open = i2c_adc_open,
    .release = i2c_adc_release,
};
// As outras operacoes são ignoradas
static int i2c_adc_open(struct inode *inode, struct file *file) return 0;
static int i2c_adc_release(struct inode *inode, struct file *file) return 0;
static ssize_t i2c_adc_write(struct file *filp, const char __user *buf,
                             size_t len, loff_t *off) return 0;

static ssize_t i2c_adc_read(struct file *filp, char __user *buf, size_t len,
                            loff_t *off) {
  int adc_val;
  // Pede dado para a ESP32 e retorna para o usuario
  if (ESP32_connected == 0) {
    pr_err("I2C Device : Not Connected!\n");
    return 0;
  }
  int count = i2c_master_recv(i2c_client, &adc_val, sizeof(int));
  if (count < sizeof(int)) {
    pr_err("I2C Read : Err!\n");
    return 0;
  }
  pr_info("I2C Read : Done!");
  // Passando o Dado adquirido para o usuário
  if (copy_to_user(buf, &adc_val, sizeof(int))) {
    pr_err("Data Read : Err!\n");
    return 0;
  }
  pr_info("Data Read : Done!\n");
  return sizeof(int);
}

// Init do Módulo
static int __init i2c_adc_init(void) {
  // Alocando o numero de versao do módulo
  if ((alloc_chrdev_region(&dev, 0, 1, "i2c_adc_Dev")) < 0) {
    pr_info("Cannot allocate major number\n");
    return -1;
  }
  pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

  // Criando os arquivos do módulo
  cdev_init(&i2c_adc_cdev, &fops);

  // Adicionando o char device ao sistema
  if ((cdev_add(&i2c_adc_cdev, dev, 1)) < 0) {
    pr_info("Cannot add the device to the system\n");
    goto r_class;
  }

  // Criando a struct class
  if (IS_ERR(dev_class = class_create(THIS_MODULE, "i2c_adc_class"))) {
    pr_info("Cannot create the struct class\n");
    goto r_class;
  }

  // Criando o device
  if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "i2c_adc_device"))) {
    pr_info("Cannot create the Device 1\n");
    goto r_device;
  }
  // Pegando o I2C
  i2c_adapt = i2c_get_adapter(1); // Para Raps nr = 1
  i2c_client = i2c_new_client_device(i2c_adapt, &i2c_board_inf);
  i2c_add_driver(&i2c_adc_driver); // Adiciona nossos dispositivos de interesse
                                   // no driver de i2c
  pr_info("Device Driver Insert...Done!!!\n");
  return 0;

r_device:
  class_destroy(dev_class);
r_class:
  unregister_chrdev_region(dev, 1);
  return -1;
}
// Fim do Módulo
static void __exit i2c_adc_exit(void) {
  device_destroy(dev_class, dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  i2c_unregister_device(i2c_client);
  i2c_del_driver(&i2c_adc_driver);
  pr_info("Device Driver Remove...Done!!!\n");
}

module_init(i2c_adc_init);
module_exit(i2c_adc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivan Pancheniak <ivanrwpf@usp.br>");
MODULE_DESCRIPTION("Driver para receber dados de um ADC por I2C");
MODULE_VERSION("1.0");
