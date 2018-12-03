##
## Makefile for virus scanner**
##
## Originally made by xsyann, updated by Kenny Roback
## Contact <contact@xsyann.com>
##
<<<<<<< HEAD
## Started on  Wed Mar 26 13:49:00 2014 xsyann
## Last update Fri Apr 18 22:13:46 2014 xsyann
##

TARGET	= test

obj-m	+= $(TARGET).o
=======
## Current version built by Yuan Xiao
## Contact <xiao.465@osu.edu>
>>>>>>> e6dfba798e4358753b73d761ac16e27abb078522


all:
	cd kernel; make
	cd userSpace; make

clean:
	cd kernel; make clean
	cd userSpace; make clean
