#include <kfs/io.h>
#include <kfs/ps2_controller.h>


extern uint8_t		wait_ps2_to_write(void)
{
	uint8_t		status;

	//wait for bit 1 to be clear
	do {
		status = inportb(0x64);
	} while ((status & 0x02) == 1);
	return (status);
}

extern uint8_t		wait_ps2_to_read(void)
{
	uint8_t		status;

	//wait for bit 0 to be set
	do {
		status = inportb(0x64);
	} while ((status & 0x01) == 0);
	return (status);
}

extern uint8_t		send_command(uint8_t port, uint8_t cmd,
								 uint8_t data, uint8_t send_data,
								 uint8_t await_response)
{
	uint8_t		response = 0x0;

	wait_ps2_to_write();
	//send command code to controller
	outportb(port, cmd);
	if (send_data) {
		wait_ps2_to_write();
		//send data byte to data port
		outportb(0x60, data);
	}
	if (await_response) {
		wait_ps2_to_read();
		//read response from data port
		response = inportb(0x60);
	}
	return (response);
}
