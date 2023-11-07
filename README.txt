instruction to run:
    mkdir build
    cd build
    cmake ..
    make
    ./OrderOptimizer ../
    ./OrderOptimizerTests

to enable c++17 libraries, because I am using filesystem standard library and for some versions of gcc compilers 
it is not supported, if the compiler supports it, cmake is configured to run in C++17 standard
    sudo apt-get install gcc-8 g++-8 
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
    
because I didn't want to use REST or MQTT interface without some external libraries I decided to make very 
simple terminal interface, to run program you need to pass project directory as an argument, later
you have 3 commands, not case sensitive: currentPosition, nextOrder or quit.

the most important improvements to be done:
    - stop search when thread finds order
    - optimize routing algorithm - parts with the same name has the same location
    - shortest path to make it really shortest
    - interface which will accept json