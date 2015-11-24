# piot-athome-arduino
This is a resource for the PiOT at Home Workshop Materials for connecting Arduinos (Cactus Micros) to a NodeJS server acting as a hub.

Here you will find code materials for the tutorial as well as the tutorial itself in the [wiki](https://github.com/InitialState/piot-athome-arduino/wiki).

We recently wrote up [how you can turn your Raspberry Pi (or any other similar board) into a Smart Home Hub](https://github.com/InitialState/node-hub/wiki) for all of your little internet-connected devices that can't handle TLS encryption.

Why not just use the Pi to read from all those sensors? Well, a Pi is a base price of $35, a WiFi dongle will cost you another $10, and honestly being the size of a business card is still kinda big to find a place for in your house. We also happen to be in the middle of an incredible technological revolution that's popping out cheap WiFi boards faster than they can arrive in your mailbox.

These little boards are usually less than 25% the size of a Pi and cost 1/3 of the price. One such board is the Cactus Micro.

But there's a price for being cheap - the Cactus Micro (and many boards like it) is one of those internet-connected boards incapable of handling basic TLS encryption when sending data over the web. This is definitely a problem if you want to securely stream data about your home. This is where the Raspberry Pi comes in handy ... [read more](https://github.com/InitialState/piot-athome-arduino/wiki)
