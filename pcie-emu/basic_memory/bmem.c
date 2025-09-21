#include "qemu/osdep.h"
#include "qemu/units.h"
#include "hw/pci/pci.h"
#include "hw/hw.h"
#include "hw/pci/msi.h"
#include "qemu/timer.h"
#include "qom/object.h"
#include "qemu/main-loop.h" /* iothread mutex */
#include "qemu/module.h"
#include "qapi/visitor.h"
#include <sys/types.h>

#define TYPE_PCI_CUSTOM_DEVICE "basicmem"

#define BMEM_DEVICE_ID 	0xbead
typedef struct MemState MemState;
DECLARE_INSTANCE_CHECKER(MemState, BMEM, TYPE_PCI_CUSTOM_DEVICE)

struct MemState {
    PCIDevice pdev;
    MemoryRegion mem;
    uint8_t data[0x10000];
};

static void pci_bmem_register_types(void);
static void bmem_instance_init(Object *obj);
static void bmem_class_init(ObjectClass *class, const void *data);
static void pci_bmem_realize(PCIDevice *pdev, Error **errp);
static void pci_bmem_uninit(PCIDevice *pdev);

static uint64_t bmem_mem_read(void *opaque, hwaddr addr, unsigned size);
static void bmem_mem_write(void *opaque, hwaddr addr, uint64_t val, unsigned size);

type_init(pci_bmem_register_types)

static void pci_bmem_register_types(void)
{
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    };
    static const TypeInfo bmem_info = {
        .name          = TYPE_PCI_CUSTOM_DEVICE,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(MemState),
        .instance_init = bmem_instance_init,
        .class_init    = bmem_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&bmem_info);
}

static void bmem_instance_init(Object *obj)
{
	printf("BMEM instance init\n");
}

static void bmem_class_init(ObjectClass *class, const void *data)
{
	printf("Class init\n");
    //DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize = pci_bmem_realize;
    k->exit = pci_bmem_uninit;
    k->vendor_id = 0xdead;
    k->device_id = BMEM_DEVICE_ID;
    k->class_id = PCI_CLASS_OTHERS;
}

static const MemoryRegionOps bmem_mem_ops = {
  .read = bmem_mem_read,
  .write = bmem_mem_write,
};


static void pci_bmem_realize(PCIDevice *pdev, Error **errp)
{
	printf("BMEM Realize\n");
    MemState *dev = BMEM(pdev);
    //uint8_t *pci_conf = pdev->config;
    memory_region_init_io(&dev->mem, OBJECT(dev),&bmem_mem_ops,dev,"bmem-dev",1*MiB);
    pci_register_bar(pdev,0,PCI_BASE_ADDRESS_SPACE_MEMORY,&dev->mem);
}

static void pci_bmem_uninit(PCIDevice *pdev)
{
	printf("BMEM un-init\n");
}

static uint64_t bmem_mem_read(void *opaque, hwaddr addr, unsigned size) {
  MemState *mem_dev = opaque;
  uint64_t ret;
  uint8_t* buf = (uint8_t*)(&ret);
  for(hwaddr ptr = 0; ptr < size; ptr++){
    buf[ptr] = mem_dev->data[addr + ptr];
  }
  printf("Tried to read 0x%x bytes at 0x%lx = 0x%lx\n", size, addr, ret);
  return ret;
}

static void bmem_mem_write(void *opaque, hwaddr addr, uint64_t val, unsigned size) {
  MemState *mem_dev = opaque;
  uint8_t* buf = (uint8_t*)(&val);
  for(hwaddr ptr = 0; ptr < size; ptr++){
    mem_dev->data[addr + ptr] = buf[ptr];
  }
  printf("Tried to write 0x%lx (0x%x bytes) at 0x%lx\n", val, size, addr);
}