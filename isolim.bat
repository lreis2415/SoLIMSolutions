@echo off
set mypath=%cd%
setlocal
  set PROJ_LIB=%mypath%\proj_lib
  start "" ".\bin\isolim.exe"
endlocal

