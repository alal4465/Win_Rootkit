# Win_Rootkit
A kernel-mode rootkit with remote control that utilizes some C++ Runtime.         
Uses DKOM and IRP Hooks.             
Hiding Processes, token manipulation , hiding tcp network connections by port...

### Process elevation (token manipulation):
![elev](https://user-images.githubusercontent.com/60041914/86833920-da6c7f80-c0a2-11ea-9f6d-9ce15bbbdc5d.gif)

### Hiding Processes:
![hide](https://user-images.githubusercontent.com/60041914/86835571-fc670180-c0a4-11ea-9f0f-35b1a1eac1ff.gif)

### Hiding TCP network connections:
![ezgif-6-8cefc9a805ab](https://user-images.githubusercontent.com/60041914/87465040-c1247f80-c61c-11ea-8869-80fbb301cdf2.gif)

Tested on Windows 7 SP 1

### Features
- [x] Elevate Process privillages to NT AUTHORITY\SYSTEM by token manipulation
- [x] Hide process by unlinking from ActiveProcessLinks
- [x] Remote command execution
- [x] A remote keylogger
- [x] Dropper
- [x] TCP connection hiding by port (IRP hooking)
