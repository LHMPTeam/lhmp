# How to compile Lost Heaven Multiplayer?

## How to compile on Windows client/server
* Open LHMP.sln in Visual Studio 2013.
* Compile

## How to compile Windows server browser
* You need to have Qt Framework to be able to compile the browser
* Open Browser in Qt Creator and compile.

## How to compile GNU/Linux server under Ubuntu
* Download CodeBlocks IDE, use Tesla.cbp in /Tesla/ directory, 
  add there latest headers | source files and COMPILE IT.
* If you need to compile it on another machine, use "cbp2mak" application to generate makefile,
  copy directories: Tesla, shared, sdks(Squirrel,RakNet) and makefile on another PC and use make in /Tesla/
* Watch out for x86-64 machines.
* "cbp2make -in Tesla.cbp -out makefile -unix" - in Tesla directory to generate makefile.