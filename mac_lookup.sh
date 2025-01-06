#!/bin/bash

# Path to the MAC lookup table
MAC_LOOKUP_FILE="mac_lookup_table.txt"

# Check if the MAC lookup file exists
if [[ ! -f "$MAC_LOOKUP_FILE" ]]; then
    echo "Error: MAC lookup table '$MAC_LOOKUP_FILE' not found!"
    exit 1
fi

# Fetch ARP table
arp_output=$(arp -a)

# Print the header of the table
echo "$arp_output" | head -n 2

# Process each line of the ARP table
echo "$arp_output" | tail -n +3 | while read -r line; do
    # Extract IP, MAC, and Type from the line
    ip=$(echo "$line" | awk '{print $1}')
    mac=$(echo "$line" | awk '{print $2}')
    type=$(echo "$line" | awk '{print $3}')

    # Normalize MAC address for lookup
    normalized_mac=$(echo "$mac" | tr 'a-z' 'A-Z')
    oui=$(echo "$normalized_mac" | cut -d'-' -f1-3)

    # Look up vendor name in the lookup table
    vendor=$(grep -i "$oui" "$MAC_LOOKUP_FILE" | awk -F'\t' '{print $3}')

    # If no vendor found, set as "Unknown"
    if [[ -z "$vendor" ]]; then
        vendor="Unknown"
    fi

    # Print the line with vendor information
    printf "  %-20s %-20s %-10s %-30s\n" "$ip" "$mac" "$type" "$vendor"
done
