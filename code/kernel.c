extern void uart_init(void);
extern void uart_puts(char *s);

void start_kernel(void)
{
	uart_init();
	
	uart_puts("###########################################\n\r");
	uart_puts("####        Hello, D1 & RISC-V!        ####\n\r");
	uart_puts("###########################################\n\r");
	uart_puts("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");

	while (1) {}; // stop here!
}

