git@github.com:amin7/rcbrowser.git

sudo apt-get install libi2c-dev

[used]
git clone https://github.com/cesanta/mongoose.git
git clone https://github.com/Tencent/rapidjson.git


https://github.com/jeromeetienne/virtualjoystick.js.git

http://rapidjson.org/md_doc_dom.html

http://dmitrybaranovskiy.github.io/raphael/
http://rangeslider.js.org/

[video]
http://www.linux-projects.org/uv4l/installation/

sudo apt-get install uv4l-webrtc

https://www.instructables.com/id/Raspberry-Pi-Video-Streaming/

[pin]
http://pi4j.com/pins/model-zerow-rev1.html
http://wiringpi.com
git clone https://github.com/Reinbert/pca9685.git
sudo make install

[html]
https://github.com/seiyria/bootstrap-slider

[start up]
https://www.stuffaboutcode.com/2012/06/raspberry-pi-run-program-at-start-up.html

[cli]
https://github.com/CLIUtils/CLI11.git

[gnu]
http://gnutoolchains.com/raspberry/

[]
https://github.com/LLK/scratchx/wiki
[start from usb]
https://www.raspberrypi.org/documentation/hardware/raspberrypi/bootmodes/msd.md
add program_usb_boot_mode=1 to  /boot/config.txt
after reboot check if 
$vcgencmd otp_dump | grep 17:
17:3020000a

[misk]
sudo systemctl  rcbrowser
sudo /etc/init.d/rcbrowser stop
sudo /etc/init.d/rcbrowser start
sudo mount -a
gpio i2cdetect
./obj/rcbrowser -f ~/git_net/rcbrowser/frontend/
make clean;make -j4
cd git_net/rcbrowser/


https://marketplace.eclipse.org/content/direct-remote-c-debugging

--------------------
#ADC ads1115
A0 -5v
A3 -[10k]-vbat-[10k]-GND
