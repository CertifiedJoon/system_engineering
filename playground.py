"""
System
first process on windows boot. pid=4. kernel mode. starts smss.exe 
Abnormal behavior:
1. a parent process
2. multiple instances
3. image path not windows/system32
4. pid not 4

System > smss.exe (session management subsystem). responsible for
booting multiple process and terminates. csrss.exe, wininit.exe, winlogon.exe, explorer.exe
1. parent process not System
2. User not SYSTEM
3. multiple instances running
4. image path not Windows/system32

csrss.exe (client server runtime subsystem). responsible for 

Abnormal behavior:
1. an actual parent process smss.exe starts this and terminates
2. multiple instances
3. image path not windows/system32
4. misspellings

wininit.exe (windows initializtion process). reponsible for booting services and sessions
Abnormal behavior
1. an actual parent process smss.exe starts this and terminates
2. multiple instances
3. image path not windows/system32
4. misspellings

wininit.exe > services.exe responsible for creating and managing windows services.
Abnormal behavior
1. parent process not wininit.exe
2. multiple instances
3. image path not windows/system32
4. misspellings

wininit.exe > services.exe > svchosts.exe responsible for creating session for each services.
often targeted because there are always multiple instances running
Abnormal behavior
1. parentnot services.exe
2. -k not in binary path/command line
3. image path not windows/system32
4. misspellings

wininit.exe > lsass.exe (local security authority subsystem) responsible for storing credentials,
manage password change etc. often targeted by credential dumping, pass the hash attack
1. multiple instances running
2. wrong parent process
3. image path not windows/system32

VBS by credential guard.

winlogon.exe reponsible for user session, screen saver, user profile

explorer.exe responsible for user's file system
look out for outgoing tcp connections


"""
