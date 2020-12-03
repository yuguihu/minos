#include <asm/asm_marco.S>
#define CONFIG_UART_BASE 0xfe215040

#define AUX_MU_LSR	0x14
#define BCM283X_MU_LSR_TX_EMPTY		(1 << 5)
#define BCM283X_MU_LSR_RX_READY		(1 << 0)


/*
 *static inline void __bcm283x_mu_putc(char c)
 *{
 *	while (!(ioread32(serial_base + AUX_MU_LSR) &
 *				BCM283X_MU_LSR_TX_EMPTY));
 *
 *	iowrite32(c, serial_base + AUX_MU_IO);
 *}
 */

/*
 * Fill in the buffer with character c (alignment handled by the hardware)
 *
 * Parameters:
 *	w0 - buf
 *
 */
	.global print

func print

	ldr	x1, =0xfe215040
1:	ldr w2, [x1, #AUX_MU_LSR]
	and w2, w2, #0x20
	cbz w2, 1b
    str w0, [x1]
	ret
	
endfunc print


