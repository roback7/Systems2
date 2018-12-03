##
## Makefile for virus scanner**
##
## Originally made by xsyann, updated by Kenny Roback
## Contact <contact@xsyann.com>
##
## Current version built by Yuan Xiao
## Contact <xiao.465@osu.edu>


all:
	cd kernel; make
	cd userSpace; make

clean:
	cd kernel; make clean
	cd userSpace; make clean
