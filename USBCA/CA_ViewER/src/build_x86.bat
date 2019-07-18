mkdir build_x86
copy CMake_x86\CMakeLists.txt build_x86

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe" /rebuild "Release|Win32" /out build.log ..\..\USBCAExtensionUnit\src\USBCAExtensionUnit_VS2017.sln

expand ..\dll\x86\opencv_world342.cab ..\dll\x86\opencv_world342.dll
expand ..\dll\x86\opencv_world342d.cab ..\dll\x86\opencv_world342d.dll

copy ..\..\USBCAExtensionUnit\src\Release\*.dll build_x86
xcopy /s /y ..\dll\x86 build_x86

mkdir build_x86\Debug
echo.[Paths]>build_x86\Debug\qt.conf
echo.Prefix=..>>build_x86\Debug\qt.conf

mkdir build_x86\Release
echo.[Paths]>build_x86\Release\qt.conf
echo.Prefix=..>>build_x86\Release\qt.conf

copy CA_ViewER.ini build_x86
cd build_x86
cmake -G "Visual Studio 15 2017" .
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe" /rebuild release /out build.log CA_ViewER.sln
mkdir ..\..\bin\x86\plugins\platforms
copy /b release\CA_ViewER.exe ..\..\bin\x86
copy /b .\CA_ViewER.ini ..\..\bin\x86
copy /b .\USBCAExtensionUnit.dll ..\..\bin\x86
copy /b .\opencv_ffmpeg342.dll ..\..\bin\x86
copy /b .\opencv_world342.dll ..\..\bin\x86
copy /b .\qt.conf ..\..\bin\x86
copy /b .\Qt5Core.dll ..\..\bin\x86
copy /b .\Qt5Gui.dll ..\..\bin\x86
copy /b .\Qt5Widgets.dll ..\..\bin\x86
copy /b plugins\platforms\qminimal.dll ..\..\bin\x86\plugins\platforms
copy /b plugins\platforms\qoffscreen.dll ..\..\bin\x86\plugins\platforms
copy /b plugins\platforms\qwindows.dll ..\..\bin\x86\plugins\platforms
xcopy /i /y ..\LSC ..\..\bin\LSC
pause
