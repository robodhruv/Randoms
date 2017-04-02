## ModelSim Installation issues
### Ubuntu 14.xx and above

Ignore this if you have **not** encountered any issue with the installation and running of ModelSim and Quartus on your system. You are very lucky. (*Just Kidding! You have surely had this issue, only sorted.*)

Hence assuming you have been following the procedure given in [this guide](Getting Started.pdf). Most certainly, Quartus will install jsut fine, and so will ModelSim. The issue is in launching due to inappropriate linking etc.

#### Stage 1
This is the simplest error you would encounter. Navigate to the `modelsim_ase` folder and run:
```sh
cd /opt/modelsim_ase/bin/
./vsim
```

Unless you have not updated your Linux kernel in the last 3 years, you are most certain to encounter an error saying `could not find ./../linux_rh60/vsim`. To understand this (optional) you can open the file `vsim` and notice that in the `if...elseif` conditions, the default description points towards a folder for the Red Hat distro.  
(This setup will only work for you without changes IF your Linux kernel is 3.x or below. Highly unlikely. Check using `uname -r`).

Okay, all of that aside, basically you need to replace this as (`sudo` [is your friend!](http://i.imgur.com/LzCAfLY.jpg)) :
```sh
*)                vco="linux" ;;
```
Also, change the very first non-commented line to:
```sh
mode=${MTI_VCO_MODE:-"32"}
```
This should be adequate for most of you, and running `/opt/modelsim_ase/bin/vsim` will open a plain boring GUI. For that lot, thanks a lot for sticking around! Hope it helped. The others, keep following.

#### Stage 2

Here is where things get nasty. Does running the `vsim` script lead to:
```sh
** Fatal: Read failure in vlm process (0,0)
Segmentation fault (core dumped)
```
If yes, hang on. If not, I am sorry! Google is your friend!

I am guessing this issue came up for me because of the infinite existing packages on my system, including ROS. That seems to have ruined the setup for me, and hence more learning about the intricacies of compilation and linking. Totally worth it!

Okay, so here's what you would have to do. You probably need to build a new version of freetype, a font setting library and modify ModelSim to use it. You can download the source code for freetype [here](http://download.savannah.gnu.org/releases/freetype/freetype-2.4.12.tar.bz2).

```sh
sudo apt-get build-dep -a i386 libfreetype6
# The above forced me to uninstall my ROS packages, and well.. I had no choice. Temporarily, let's remove them and proceed
cd ~/Downloads/
cd freetype-2.4.12
./configure --build=i686-pc-linux-gnu "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
make -j8
```
Most certainly, the above would give you an error looking like:
```
checking whether the C++ compiler works... no
...
configure: error: C++ compiler cannot create executables
```

This is because you could be on a 64-bit system but you will also need the 32-bit versions of the libraries that it depends on. After an hour of fretting, here's what you'll have to do.

```sh
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install build-essential
# You already had build-essential and gcc, g++ etc. But you will also need to install the 32 bit versions.

sudo apt-get install gcc-multilib g++-multilib \
lib32z1 lib32stdc++6 lib32gcc1 \
expat:i386 fontconfig:i386 libfreetype6:i386 libexpat1:i386 libc6:i386 libgtk-3-0:i386 \
libcanberra0:i386 libpng12-0:i386 libice6:i386 libsm6:i386 libncurses5:i386 zlib1g:i386 \
libx11-6:i386 libxau6:i386 libxdmcp6:i386 libxext6:i386 libxft2:i386 libxrender1:i386 \
libxt6:i386 libxtst6:i386
```

This just about solves the dependency issues and compiler errors. Run the following inside `freetype...` folder.
```sh
./configure --build=i686-pc-linux-gnu "CFLAGS=-m32" "CXXFLAGS=-m32" "LDFLAGS=-m32"
make -j8
```

This should nicely compile and give you the libraries in the directory `objs/.libs/`. Move this to the `modelsim_ase` folder and define the linkings. Almost done here...

```sh
mkdir /opt/modelsim_ase/lib32

sudo cp ~/Downloads/freetype-2.4.12/objs/.libs/libfreetype.so* /opt/modelsim_ase/lib32

sudo vim /opt/modelsim_ase/bin/vsim
```
Find the line which says
```sh
dir=`dirname $arg0`
```
_(Note that this occurs in multiple locations. In simple terms, look at the one in the main sequence of execution, and not in the if statement. You can ignore the one under `CYGWIN`. Cygwin is for Windows. Windows is for losers! :p Apologies.)_

Below this add the line:
```sh
export LD_LIBRARY_PATH=${dir}/lib32
```

Save this file. This would only be possible if you have `sudo` permissions. And finally!
```sh
cd /opt/modelsim_ase/bin/
./vsim
```

Hopefully, a plain boring GUI will show up!  



**Hope this helped. Cheers!**  
_Dhruv_
