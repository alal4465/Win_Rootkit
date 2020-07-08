# Win_Rootkit
A kernel-mode rootkit with remote control that utilizes DKOM.       
Hiding Processes, token manipulation , remote command execution...
### Process elevation (token manipulation):
![elev](https://user-images.githubusercontent.com/60041914/86833920-da6c7f80-c0a2-11ea-9f6d-9ce15bbbdc5d.gif)

### Hiding Processes:
![hide](https://user-images.githubusercontent.com/60041914/86835571-fc670180-c0a4-11ea-9f0f-35b1a1eac1ff.gif)

Tested on Windows 7 SP 1

### Features
- [x] Elevate Process privillages to NT AUTHORITY\SYSTEM by token manipulation
- [x] Hide process by unlinking from ActiveProcessLinks
- [x] Remote command execution
- [x] A remote keylogger
- [x] Dropper
