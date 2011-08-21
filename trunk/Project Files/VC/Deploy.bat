RMDIR /S /Q ..\..\Boris\Build\Win32\Boris


MKDIR ..\..\Boris\Build\Win32\Boris
MKDIR ..\..\Boris\Build\Win32\Boris\Scripts
MKDIR ..\..\Boris\Build\Win32\Boris\Data

::Copy Executable
COPY Release\Boris.exe ..\..\Boris\Build\Win32\Boris

::Compile Script Files
FOR /f %%A IN ('DIR /b ..\..\Boris\Scripts *.lua') DO CALL ..\..\Boris\Tools\Win32\luac\luac5.1.exe -o ..\..\Boris\Build\Win32\Boris\Scripts\%%A  ..\..\Boris\Scripts\%%A 

::Compile Release Script files
FOR /f %%A IN ('DIR /b "..\..\Boris\Release Scripts" *.lua') DO CALL ..\..\Boris\Tools\Win32\luac\luac5.1.exe -o ..\..\Boris\Build\Win32\Boris\Scripts\%%A  "..\..\Boris\Release Scripts\%%A"

::Copy resources
COPY ..\..\Boris\Data\ARIAL.TTF ..\..\Boris\Build\Win32\Boris\Data\
COPY ..\..\Boris\Data\top.dat ..\..\Boris\Build\Win32\Boris\Data\

PATH ..\..\Boris\Bin\Win32\

::Copy and package media
CALL Release\ResourcePacker.exe ../../Boris/Data/Global  ../../Boris/Build/Win32/Boris/Data/Global.dat
CALL Release\ResourcePacker.exe ../../Boris/Data/Forms/Game  ../../Boris/Build/Win32/Boris/Data/Game.dat
CALL Release\ResourcePacker.exe ../../Boris/Data/Forms/GameOver  ../../Boris/Build/Win32/Boris/Data/GameOver.dat
CALL Release\ResourcePacker.exe ../../Boris/Data/Forms/MainMenu  ../../Boris/Build/Win32/Boris/Data/MainMenu.dat
CALL Release\ResourcePacker.exe ../../Boris/Data/Forms/Score  ../../Boris/Build/Win32/Boris/Data/Score.dat

::Copy dll files
FOR /f %%A IN ('DIR /b ..\..\Boris\Bin\Win32 *.dll') DO COPY ..\..\Boris\Bin\Win32\%%A ..\..\Boris\Build\Win32\Boris\

PAUSE