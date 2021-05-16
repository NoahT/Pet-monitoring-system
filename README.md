# ECE 196 Hardware Project
## Authors: ....


# server testing on browser
On a terminal run:
    $ ifconfig

Get your wifi IP address should be next to wlp4s0

    inet addr:xxx.xxx.xxx.xxx 

On other terminal on the Server folder run:

    $ python server.py

In your favorite browser search bar type

    <ip_addres>:port_number/Hi.html

    NOTE: the server is running on PORT=12000

    Ex:
    128.238.251.26:12000/Hi.html

    Also you can use loopback address

    Ex:
    127.0.0.1:12000/Hi.html
    