@echo off

echo -------
echo -------

set Wildcard=*.h *.cpp *.inl *.c

echo TODOS in Client:
findstr -s -n -i -l "TODO" Client\%Wildcard%

echo.
echo TODOS in Loader:
findstr -s -n -i -l "TODO" Loader\%Wildcard%

echo.
echo TODOS in Server:
findstr -s -n -i -l "TODO" Server\%Wildcard%

echo.
echo TODOS in MasterServer:
findstr -s -n -i -l "TODO" MasterServer\%Wildcard%

echo -------
echo -------
