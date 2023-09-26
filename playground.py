"""
System: first kernel process to be run, run at boot. starts smss.exe
Abnormal behavior: pid not 4, multiple instances running, a parent process, 

System > smss.exe: Session Management Subsystem. starts processes both in kernel mode and user mode
such as wininit.exe, csrss.exe, winlogon.exe, explorer.exe
Abnormal behavior: parent not SYSTEM, user not SYSTEM, image path not windows/system32, 

csrss.exe: client server runtime subsystem, manages client server interaction.
Abnormal behavior: actual parent process (smss.exe starts this and terminates), multiple instances
running, misspellings, user not SYSTEM, wrong image path

wininit.exe: process to initialize windows

wininit.exe > services 

wininit.exe > services >  svchosts.exe: manages session for each hosts. 
always have multiple instances running, so it is often targeted. 
Abnormal behavior: parent process other than csrss, mispellings, wrong image path, command without -k

winlogon

winlogon > lsass.exe pass the hash

explorer. look out for outgoing tcp connection

alert tcp any any <> any 80 (msg:"found"; flag:"S"; sid:1000001;rev:1)

Get-WinEvent -LogName Application-FilterXPath '*/System/Provider[@Name="WLMS"] and */EvenData/Data[@Name=ProcessCreationTime]=blablah'


"""
