This is a very simple ESP-NOW experiment to test transmission of three integers across the ESP-NOW network. 

I recommend uploading the print_mac_addr program to both ESP microcontrollers first to get the mac addresses of both, unless you want to set them to custom values. 

Regardless, once you have the MAC addresses of the receiver and the transmitter, modify the peer MAC address of the other microcontroller, so that the transmitter code has the receiver MAC address, and vice versa. 

From there, the transmitter should broadcast "21 42 100" once a second to the receiver.
