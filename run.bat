@echo off
set progName=ShootingRange
g++ main.cpp stl.cpp -lfreeglut -lglu32 -lopengl32 -lwinmm -o %progName%
if %errorlevel% neq 0 (
  exit /b
)
%progName%