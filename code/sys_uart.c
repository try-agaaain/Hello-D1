/*
 * sys-uart.c
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */


typedef unsigned int u32_t;
typedef unsigned long long addr_t;

static void write32(addr_t addr, u32_t value)
{
	*((volatile u32_t *)(addr)) = value;
}

static u32_t read32(addr_t addr)
{
	return( *((volatile u32_t *)(addr)) );
}

void uart_init(void)
{
	addr_t addr;
	u32_t val;

	addr = 0x02500000;	// 第0块UART的起始地址

	// 在默认情况下，0x04处为UART_IER寄存器，将其设置为0以关闭中断
	write32(addr + 0x04, 0x0);
	
	// 将0x0c的UART_LCR第8位设置位1，其他位保持不变，0x00被选为UART_DLL寄存器，0x04被选为UART_DLH寄存器
	val = read32(addr + 0x0c);
	val |= (1 << 7);
	write32(addr + 0x0c, val);
	// 设置0x00的UART_DLL为0x0d，即13，对应波特率为115200
	write32(addr + 0x00, 0x0d);
	// 设置0x04的UART_DLM为0x00，即将波特率的高8位设置位0
	write32(addr + 0x04, 0x00);

	// 将0x0c的UART_LCR第8位设置位0，0x00被选为UART_RBR/UART_THR，0x04处被选为UART_IER
	val = read32(addr + 0x0c);
	val &= ~(1 << 7);
	write32(addr + 0x0c, val);
	
	// 保持0x0c的UART_LCR高2:31位不变，将低2位设置为11，表示数据长度为8bit
	val = read32(addr + 0x0c);
	val &= ~0x02;
	val |= 0x3;
	write32(addr + 0x0c, val);
}

void sys_uart_putc(char c)
{
	addr_t addr = 0x02500000;

	while((read32(addr + 0x7c) & (0x1 << 1)) == 0);	// 当UART_USR[1]=0时阻塞读写
	write32(addr + 0x00, c);
}


void uart_puts(char *s)
{
	while (*s) {
		sys_uart_putc(*s++);
	}
}
