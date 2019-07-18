mkdir build_x64
copy CMake_x64\CMakeLists.txt build_x64

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe" /rebuild "Release|x64" /out build.log ..\..\USBCAExtensionUnit\src\USBCAExtensionUnit_VS2017.sln

expand ..\dll\x64\opencv_world342.cab ..\dll\x64\opencv_world342.dll
expand ..\dll\x64\opencv_world342d.cab ..\dll\x64\opencv_world342d.dll

copy ..\..\USBCAExtensionUnit\src\x64\Release\*.dll build_x64
xcopy /s /y ..\dll\x64 build_x64

mkdir build_x64\Debug
echo.[Paths]>build_x64\Debug\qt.conf
echo.Prefix=..>>build_x64\Debug\qt.conf

mkdir build_x64\Release
echo.[Paths]>build_x64\Release\qt.conf
echo.Prefix=..>>build_x64\Release\qt.conf

copy CA_ViewER.ini build_x64
cd build_x64
cmake -G "Visual Studio 15 2017 Win64" .
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe" /rebuild release /out build.log CA_ViewER.sln
mkdir ..\..\bin\x64\plugins\platforms
copy /b release\CA_ViewER.exe ..\..\bin\x64
copy /b .\CA_ViewER.ini ..\..\bin\x64
copy /b .\USBCAExtensionUnit.dll ..\..\bin\x64
copy /b .\opencv_ffmpeg342_64.dll ..\..\bin\x64
copy /b .\opencv_world342.dll ..\..\bin\x64
copy /b .\qt.conf ..\..\bin\x64
copy /b .\Qt5Core.dll ..\..\bin\x64
copy /b .\Qt5Gui.dll ..\..\bin\x64
copy /b .\Qt5Widgets.dll ..\..\bin\x64
copy /b plugins\platforms\qminimal.dll ..\..\bin\x64\plugins\platforms
copy /b plugins\platforms\qoffscreen.dll ..\..\bin\x64\plugins\platforms
copy /b plugins\platforms\qwindows.dll ..\..\bin\x64\plugins\platforms
xcopy /i /y ..\LSC ..\..\bin\LSC
pause
