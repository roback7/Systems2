echo "Loading virus scanner..."
sudo insmod ./kernel/scan.ko
sudo ./userSpace/user $1
sudo rmmod scan
dmesg | tail -n 1
