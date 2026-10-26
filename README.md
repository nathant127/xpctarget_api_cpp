# xpctarget_api_cpp

This repository is part of project to allow controlling the Armeo Power exoskeleton through ROS2 linux, however this library works on its own. 
The documentation for the Armeo Power project can be found here [armeo_docs](https://github.com/nathant127/armeo_docs).

xpctarget is a system developed by Mathworks to design and deploy realtime models onto a "Target PC", which can then be communicated with via the xpc target api.

This repository is a C++ linux based replacement for "xpcapi.dll", a windows based C library that allows a user to communicate with the Target PC.
This is needed as Mathworks never built a version of their api for linux.

As this is a replacement, the function signatures and functionality are the same so that it can work as a drop in replacement.

The API is not complete, many functions are left unimplemented and some may not be tested, however the goal was only to implement and test the functions necessary for the Armeo Power project and that was sucessful.

## Design

- The xpc target protocol was decoded using wireshark on xpcapi.dll. The implementation of each api function was guessed based on their name, parameters and the packets they sent out.
The wireshark plugin that was created to decrypt the xpc protocol can be found at [xpc plugin](wireshark/xpc.lua).

- Please note that this project is designed to almost be a drop in replacement for xpcapi.dll, however the internal implementation is likely completely different as I designed the internal implementation based only on what I could find out about xpcapi.dll by treating it like a black box.

- The error handling system is completely different from xpcapi.dll, with xPCGetLastError() being unimplemented, and instead using c++ exceptions as the error system. This was done to save time and if needed in the future could be replaced by a better system.

- All functions are blocking and will wait for a message to be sent out and a response recieved before moving on, as such only one message can be sent at a time, this is intentional as it is the same behaviour as xpcapi.dll. This should not be changed as it is unknown if the Target PC can handle a connection sending multiple messages at a time. This does have it's advantages, as it means that there is only ever one "message" object that is shared between every api call and as such memory is only ever allocated when you make a message bigger than the last one.

- IMPORTANT: DO NOT call multiple xpcapi functions at the same time using threads or other such methods, the API is NOT thread safe. This could be changed in the future but would complicate the API and require a large amount of refactoring.

- The port parameter is only there for future expansion and to match xpcapi.dll, the purpose it to allow multiple taget PC's to be connected to from the same process, currently the port parameter is just ignored and as such only one connection can be active at a time. If similarity to xpcapi.dll is not desired, this could be replaced with an object oriented approach, dropping the port parameter.

- Many functions take a raw pointer as a parameter. This was done to copy xpcapi.dll, but ideally this could be changed to use std::vectors or references or something else as it makes the implementation and usage awkward.

## Dependencies
- cmake
- linux

## Build
```bash
git clone https://github.com/nathant127/xpctarget_api_cpp/
cd xpctarget_api_cpp

mkdir build
cd build
cmake ..
make
```
