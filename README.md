# Goip_Voip_SMS_Gateway
A gateway to convert received SMS UDP DATA into SIP Message Data 

sms_keepalive
this file keeps the SMS integrated part of Goip in registration status, it also catches any received UDP Data
that has SMS received message content. Later on.


sms_send
this file is still not working, i still can not receive the Goip reply for BulkSMS, i get UDP Checksum failed.


send_sip_message
this file should send a sip message from a sip, to a sip user with message content given from the command line
there is makefile to compile it.
to build it, you need the library, buildessential, pkg-config

./send_sip_message "sip:alice@example.com" "sip:bob@example.com" "Hello, Bob!"

to install the sofia-sip static libray
sudo apt update
sudo apt upgrade
sudo apt install libsofia-sip-ua-dev install pkg-config install build-essential


to compile use this or use the makefile
gcc your_code.c -lsofia-sip-ua

