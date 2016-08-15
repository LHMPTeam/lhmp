# Structure

Project is divided into several parts which represent multiplayer's components.

## Core components

* **Server** is multiplatform dedicated server application

* **Client** is Win32 dynamically loaded library (DLL), serving as a multiplayer client
## Additional software

* **Loader** is Win32 console injecting client's dll into Mafia's process

* **lhmp_browser** is the officially supported browser, written in Qt framework

* **lhmp_installer** provides user-friendly installer for installing client at end-user's computer

* **masterserver** handles the list of all running public LHMP servers

* **_Old** contains old files that might be useful for someone.

## 3rd party libraries 

All 3rd party libraries used in project, can be found in **sdks** directory.

* RakNet (Jenkinss Software)
* Squirrel3

* DirectX 8.1 SDK

* MD5 (Colin Plumb)