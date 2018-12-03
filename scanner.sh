
#echo "Enter the filename you wish to scan:"
#read file
sudo make
#echo "Loading virus scanner..."
sudo insmod ./scan.ko
sudo gcc user.c -o user
./user $1
sudo rmmod scan
dmesg | tail -n 1
