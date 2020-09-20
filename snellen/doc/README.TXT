readme_dev.txt

 Created on: May 6, 2009
     Author: andrew

 Updated Jan 24, 2014 AH

/** --------------------
    Snellen Requirements
    -------------------- **/

I use debian 5.0 (lenny). First I install a minimal system using netinstall. Then I install frame buffer,
development tools and some extra stuff:

The frame buffer stuff:

apt-get install libdirectfb-bin libdirectfb-dev libdirectfb-1.0-0 libdirectfb-extra fbset

Editing and compiling tools:

apt-get install build-essential vim emacs21-nox gdb mc less

and a few extra utilities

apt-get install mingetty lirc debian-archive-keyring

Also, the snellen package uses libconfig:

wget http://www.hyperrealm.com/libconfig/libconfig-1.3.2.tar.gz
tar -xzvf libconfig-1.3.2.tar.gz
cd libconfig-1.3.2
./configure --prefix=/usr
make
make install


/**  ------------------
     Start Frame Buffer
     ------------------  **/

The fb driver must be installed. If you want, you can start linux in frame buffer mode by modifying
the boot parameters of the linux kernel. I prefer to start in normal console mode and insert the frame
buffer module later. I use a script in the /etc/init.d directory with a symlink in /etc/rc2.d. Different video
cards use different drivers, so the contents of the script depend on your video card and monitor. 
Here are some examples for machines I have set up:

A Gateway Profile 3 all-in-one PC used this command:

modprobe i810fb hsync1=30 hsync2=70 vsync1=62 vsync2=90 xres=1024 yres=768 bpp=16

An ATI Rage 3d Pro card with a ViewSonic VE510s LCD monitor used this command:

modprobe atyfb mode=1024x768-24

Snellen plays well with the vesafb driver. vesafb is typically loaded at boot using a kernel option; thereafter
it cannot be changed. To load the driver add the "vga=0x3nn" kernel option to the boot parameters, where 0xnnn
is one of the numbers in the table below (from linux kernel docs vesafb.txt):

    | 640x480  800x600  1024x768 1280x1024
----+-------------------------------------
256 |  0x301    0x303    0x305    0x307   
32k |  0x310    0x313    0x316    0x319   
64k |  0x311    0x314    0x317    0x31A   
16M |  0x312    0x315    0x318    0x31B   



/** ----------------
    Building Snellen
    ---------------- **/

Install the Unpack the source tar ball. Change directory to snellen/src.

Of course, it is very important that the letters be drawn the appropriate height on the screen. For version 0.98 
you must edit two lines in the file snellen-0.98/src/snellen.h. Lines 30 and 31 describe the screen height, 
in millimeters, and the distance to the patient, also in millimeters. DirectFB retrieves the pixel height of your
screen from VESA DDC data, then snellen calculates the letter height. For reference, the big E at the top of the
chart should be 80mm tall with a distance of 6000 mm.

Use the make command to build the binary. Launch the snellen binary from the src directory.

/**  ------------------
     Configuration File
     ------------------ **/

Snellen looks for the following configuration files, in this order:

/etc/snellen.rc
~/.snellen.rc
./snellen.rc

Snellen uses the first configuration file it finds and ignores the rest.

Snellen 1.0 only has two configurable parameters: ScreenHeightInMM and DistanceToPatientInMM. Using these
two values (and the size of the screen in pixels) snellen calculates the size of the glyphs. Here is an
example of how the definitions would appear in the config file:

ScreenHeightInMM = 231;
DistanceToPatientInMM = 6000;

Snellen automatically writes the config file when it shuts down. DO NOT EDIT THE CONFIG FILE WHILE SNELLEN
IS RUNNING! Snellen will overwrite your changes.

/**  --------------------------
     Autologin and Powering off
     -------------------------- **/

I have one machine set up to go straight to snellen when it starts. Snellen launchs in frame
buffer mode on virtual terminal 5. VT's 1 to 4 can be used for debugging.

To set up autologin and autolaunch snellen, first install mingetty (apt-get install mingetty) then
edit /etc/inittab. Replace 

1:23:respawn:/sbin/getty 38400 tty1 

with

1:23:respawin:/sbin/mingetty --autologin root tty1

Also, add this to the end of root's ~/.profile file:

if [ ! $z ] ; then
	cd /root/snellen/src
	./snellen
	exit
fi

The same machine is set up to power down gracefully when the tech pushes the power button. That's handled by
the apmd package: apt-get install apmd

/** ---------
    Streamzap
    --------- **/

I have a streamzap remote working with DirectFB and snellen. It is a little bit picky. My lircd.conf file is 
included in the snellen/docs directory as lircd.conf.streamzap

/** --------------------------------------
    Colour Console and Syntax Highlighting
    -------------------------------------- **/

I do my development on the snellen PC, so it is nice to have colors and syntax highlighting enabled.

First, add the following lines to .bashrc:


if [ "$TERM" = "linux" ]; then
    echo -en "\e]P0222222" #black
    echo -en "\e]P8222222" #darkgrey
    echo -en "\e]P1803232" #darkred
    echo -en "\e]P9982b2b" #red
    echo -en "\e]P25b762f" #darkgreen
    echo -en "\e]PA89b83f" #green
    echo -en "\e]P3aa9943" #brown
    echo -en "\e]PBefef60" #yellow
    echo -en "\e]P4324c80" #darkblue
    echo -en "\e]PC2b4f98" #blue
    echo -en "\e]P5706c9a" #darkmagenta
    echo -en "\e]PD826ab1" #magenta
    echo -en "\e]P692b19e" #darkcyan
    echo -en "\e]PEa1cdcd" #cyan
    echo -en "\e]P7ffffff" #lightgrey
    echo -en "\e]PFdedede" #white
    clear #for background artifacting
fi


Sadly, syntax highlighting is turned off by default in many distros.
Turn vim's syntax highlighting back on in vim using ":syntax on" or 
by uncommenting the "syntax on" line in /etc/vim/vimrc

To turn on syntax highlighting in emacs, use one of these commands:

M-x font-lock-mode
M-x global-font-lock-mode

or add these lines to you .emacs file:

(custom-set-variables
'(global-font-lock-mode t nil (font-lock)))


/*  ----------------
    Fonts
    ---------------- */

Some free fonts:

vera
liberation
dejavu
ttf-linex
ttf-engadget
ttf-inconsolata
ttf-ocr-a

to find free truetype fonts use this command:

apt-cache search ttf | grep free

/* -----------------
   Key Mapping
   ----------------- */

A
B
C
D
E
F
G
H	Help
I
J
K
L
M
N
O
P
Q
R
S	Activate Screensaver
T
U
V
W
X
Y
Z
ESC

