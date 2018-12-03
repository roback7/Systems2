<<<<<<< HEAD
echo "┌──────────────────────────────┐"
echo "│  Load Kernel Module syscall  │"
echo "└──────────────────────────────┘"
echo ""
echo "Enter the process name you wish to scan:"
read proc
#echo "# insmod syscall.ko file"
sudo insmod ./scan.ko userProc="$proc"
echo ""
echo "┌──────────────────────────────┐"
echo "│           lsmod              │"
echo "└──────────────────────────────┘"
echo "$ lsmod | grep 'syscall'"
lsmod | grep "scan"
echo ""
echo "┌──────────────────────────────┐"
echo "│           Test               │"
echo "└──────────────────────────────┘"
echo "$ ./test $1"
./test/test $1
echo ""
echo "┌──────────────────────────────┐"
echo "│ Unload Kernel Module syscall │"
echo "└──────────────────────────────┘"
echo "# rmmod syscall"
=======

#echo "Enter the filename you wish to scan:"
#read file
#sudo make
echo "Loading virus scanner..."
sudo insmod ./scan.ko
#sudo gcc user.c -o user
./user $1
>>>>>>> parent of e6dfba7... Final version
sudo rmmod scan
echo ""
echo "┌──────────────────────────────┐"
echo "│           dmesg              │"
echo "└──────────────────────────────┘"
echo "$ dmesg"
dmesg | tail -n 15
