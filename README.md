# Goip_Voip_SMS_Gateway
A gateway to convert received SMS UDP DATA into SIP Message Data 

sms_keepalive
this file keeps the SMS integrated part of Goip in registration status, it also catches any received UDP Data
that has SMS received message content. Later on.


sms_send
this file is still not working, i still can not receive the Goip reply for BulkSMS, i get UDP Checksum failed.


to install the sofia-sip static libray
sudo apt install libsofia-sip-ua-dev

to compile 
gcc your_code.c -lsofia-sip-ua

