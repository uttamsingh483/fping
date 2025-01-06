# IP Range Ping Tool 

This is a simple C program that allows you to ping a range of IP addresses and find all pingable address(fping).

**Note**- this is the substitute of fping for windows.
## Usage

Compile the program using a C compiler:

```
gcc -o ip_range_ping ip_range_ping.c
```
## Example
Suppose you want to ping IP addresses 192.168.1.0 to 192.168.1.5, you can run:

```
./ip_range_ping 192.168.1.0-5
```
This will ping the IP addresses from 192.168.1.0 to 192.168.1.5.


## update mac lookuptable
```
curl https://standards-oui.ieee.org/ > mac_lookup_table.txt 
```