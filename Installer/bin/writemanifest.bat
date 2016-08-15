@echo off
cmd.exe
exit
mt.exe –manifest launcher.exe.manifest -outputresource:launcher.exe;1
pause