#include <minos/types.h>
#include <minos/errno.h>
#include <minos/string.h>
#include <virt/mmio.h>

static LIST_HEAD(mmio_handler_list);

int do_mmio_emulation(gp_regs *regs, int write,
		unsigned long address, unsigned long *value)
{
	struct mmio_handler *handler;

	list_for_each_entry(handler, &mmio_handler_list, list) {
		if (handler->ops->check(regs, address)) {
			if (write)
				return handler->ops->write(regs, address, value);
			else
				return handler->ops->read(regs, address, value);
		}
	}

	return -ENOENT;
}

int register_mmio_emulation_handler(char *name, struct mmio_ops *ops)
{
	struct mmio_handler *handler;

	if ((!ops) || (!ops->read) || (!ops->write) || (!ops->check))
		return -EINVAL;

	handler = (struct mmio_handler *)
		malloc(sizeof(struct mmio_handler));
	if (!handler)
		return -ENOMEM;

	memset((char *)handler, 0, sizeof(struct mmio_handler));
	init_list(&handler->list);
	handler->ops = ops;
	strncpy(handler->name, name,
		MIN(strlen(name), MMIO_HANDLER_NAME_SIZE - 1));

	list_add_tail(&mmio_handler_list, &handler->list);

	return 0;
}