######### SpeeDL #########

A simple file sharing server and client powered by cURL. SpeeDL
runs natively on GNU/Linux distros and can also be cross compiled
on windows machines. The default SpeeDL port is 2750

Compiling:
	1- Download.
	2- run: "make" command
	3- start server.

Starting a server:
	Compile and run:
		speedl-server [port] [server directory]
		EX: speedl-server 2750 files/

Downloading from a server: 
	Compile and run:
		speedl <address> [port] <file|command>

		Ex: speedl www.example.com archive.zip
			Or if on a non-standard port
			speedl www.example.com 2751 archive.zip
		The download will be available in the working directory.
		
		Getting server's file list:
			speedl www.example.com list
	

######### Changelog #########

Initial Release:
	- Working correctly
	- Download files directly from server
	
Version 1.1a:
	- Download meter
	- File list retrieval (from server to client)
	- Server side tweaks (cannot download server.log)

(C) Chris Dorman, 2014 - some rights reserved (GPLv3) - cddo@riseup.net
