#!/bin/bash
# You are NOT allowed to change the files' names!

domainNames="domainNames.txt"
domainNames2="domainNames2.txt"
IPAddressesSame="IPAddressesSame.txt"
IPAddressesDifferent="IPAddressesDifferent.txt"
IPAddressesSameTmp="IPAddressesSameTmp.txt"
IPAddressesDifferentTmp="IPAddressesDifferentTmp.txt"
adblockRules="adblockRules"



function adBlock() {
    if [ "$EUID" -ne 0 ];then
        printf "Please run as root.\n"
        exit 1
    fi

       if [ "$1" = "-domains"  ]; then
       #Find same and different domain names and fill out 2 temporary files.
        sort domainNames.txt domainNames2.txt | uniq -d > IPAddressesSameTmp.txt
        sort domainNames.txt domainNames2.txt | uniq -u > IPAddressesDifferentTmp.txt

          #take the ip adresses from each file and save them into IPAddressesSame and IPAddressesDifferent.

          while IFS= read -r ip
        do
           dig +short $ip | grep '^[.0-9]*$' >>$IPAddressesSame
        done < $IPAddressesSameTmp

          while IFS= read -r ip
        do
           dig +short $ip | grep '^[.0-9]*$' >>$IPAddressesDifferent
        done < $IPAddressesDifferentTmp

         #delete temporary files
        rm IPAddressesDifferentTmp.txt
        rm IPAddressesSameTmp.txt
    
    elif [ "$1" = "-ipssame"  ]; then

        if [ -s $IPAddressesSame ]
        then 
            while IFS= read -r ip
            do
             sudo iptables -A INPUT -s $ip -j DROP
            done < $IPAddressesSame
            
        else 
            echo "Empty file. Cannot configure rules."
        fi
        true
     elif [ "$1" = "-ipsdiff"  ]; then

         if [ -s $IPAddressesDifferent ]
        then 
            while IFS= read -r ip
            do
                iptables -A INPUT -s $ip -j REJECT
            done < $IPAddressesDifferent 
            
        else 
            echo "Empty file. Cannot configure rules."
        fi
        true
        
    elif [ "$1" = "-save"  ]; then

        iptables-save >$adblockRules
        true
        
    elif [ "$1" = "-load"  ]; then

        iptables-restore <$adblockRules
        true
        
    elif [ "$1" = "-reset"  ]; then

        
        iptables -P INPUT ACCEPT
        iptables -P OUTPUT ACCEPT
        iptables -P FORWARD ACCEPT


         iptables -F INPUT
         iptables -F OUTPUT
         iptables -F FORWARD

        
    elif [ "$1" = "-list"  ]; then

        iptables -L
        true
        
    elif [ "$1" = "-help"  ]; then
        printf "This script is responsible for creating a simple adblock mechanism. It rejects connections from specific domain names or IP addresses using iptables.\n\n"
        printf "Usage: $0  [OPTION]\n\n"
        printf "Options:\n\n"
        printf "  -domains\t  Configure adblock rules based on the domain names of '$domainNames' file.\n"
        printf "  -ips\t\t  Configure adblock rules based on the IP addresses of '$IPAddresses' file.\n"
        printf "  -save\t\t  Save rules to '$adblockRules' file.\n"
        printf "  -load\t\t  Load rules from '$adblockRules' file.\n"
        printf "  -list\t\t  List current rules.\n"
        printf "  -reset\t  Reset rules to default settings (i.e. accept all).\n"
        printf "  -help\t\t  Display this help and exit.\n"
        exit 0
    else
        printf "Wrong argument. Exiting...\n"
        exit 1
    fi
}

adBlock $1
exit 0