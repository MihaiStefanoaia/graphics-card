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

#define TYPE_PCI_CUSTOM_DEVICE "basicdev"

#define BDEV_DEVICE_ID 	0xbeef
typedef struct DevState DevState;
DECLARE_INSTANCE_CHECKER(DevState, BDEV, TYPE_PCI_CUSTOM_DEVICE)

struct DevState {
    PCIDevice pdev;
};

static void pci_dev_register_types(void);
static void dev_instance_init(Object *obj);
static void dev_class_init(ObjectClass *class, const void *data);
static void pci_dev_realize(PCIDevice *pdev, Error **errp);
static void pci_dev_uninit(PCIDevice *pdev);

type_init(pci_dev_register_types)

static void pci_dev_register_types(void)
{
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    };
    static const TypeInfo dev_info = {
        .name          = TYPE_PCI_CUSTOM_DEVICE,
        .parent        = TYPE_PCI_DEVICE,
        .instance_size = sizeof(DevState),
        .instance_init = dev_instance_init,
        .class_init    = dev_class_init,
        .interfaces = interfaces,
    };

    type_register_static(&dev_info);
}

static void dev_instance_init(Object *obj)
{
	printf("BDEV instance init\n");
}

static void dev_class_init(ObjectClass *class, const void *data)
{
	printf("Class init\n");
    //DeviceClass *dc = DEVICE_CLASS(class);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

    k->realize = pci_dev_realize;
    k->exit = pci_dev_uninit;
    k->vendor_id = 0xdead;
    k->device_id = BDEV_DEVICE_ID;
    k->class_id = PCI_CLASS_OTHERS;
}

static void pci_dev_realize(PCIDevice *pdev, Error **errp)
{
	printf("BDEV Realize\n");
    //DevState *dev = BDEV(pdev);
    //uint8_t *pci_conf = pdev->config;
}

static void pci_dev_uninit(PCIDevice *pdev)
{
	printf("BDEV un-init\n");
}