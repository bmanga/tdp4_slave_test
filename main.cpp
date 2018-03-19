#include "mbed.h"

I2CSlave slave (p28, p27);
DigitalOut led1 (LED1);

Serial pc (USBTX, USBRX);

struct sensors
{
	uint16_t ultra;
	uint16_t l1, l2, l3;
} send;


char bytes[1];



int main()
{
	send.ultra = 11;
	send.l1 = 22;
	send.l3 = 33;
	send.l2 = 44;

	slave.address(0xE0);
	slave.frequency(100000);

	pc.printf("Hello world\n"); // Just to make sure the code actually works.

	while (true) {
		int i = slave.receive();
		switch (i) {
			case 0: break;
			case I2CSlave::ReadAddressed:
			{
				int res = slave.write((const char *)(&send), 8);
				++send.ultra;
				led1 = 1;
				wait(0.001);
				led1 = 0;
				pc.printf("I was read addressed. Response %s successul.\n", res == 0 ? "" : "not");
				break;
			}
			case I2CSlave::WriteAddressed:
			{
				bool error = slave.read((char *)bytes, 1);
				if (error) {
					pc.printf("error");
				} else {
					pc.printf("received %d\n", bytes[0]);
				}
				break;
			}
			default: {
				pc.printf("there is something wrong: %d\n", i);
			}
		}
	}
}