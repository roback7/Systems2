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
sudo rmmod scan
echo ""
echo "┌──────────────────────────────┐"
echo "│           dmesg              │"
echo "└──────────────────────────────┘"
echo "$ dmesg"
dmesg | tail -n 15
=======
echo "Loading virus scanner..."
sudo insmod ./kernel/scan.ko
sudo ./userSpace/user $1
sudo rmmod scan
dmesg | tail -n 1
>>>>>>> e6dfba798e4358753b73d761ac16e27abb078522
