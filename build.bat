@echo off
REM Build the project
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release || (echo Build failed && exit /b 1)

REM Copy release or debug files to the main build directory
if exist .\build\Release (
    copy .\build\Release\* .\build
) else if exist .\build\Debug (
    copy .\build\Debug\* .\build
) else (
    echo Copying failed
    exit /b 1
)

REM Copy resources to the build directory
xcopy /E /I /Y .\ressources .\build\ressources\ || (echo Copying resources failed && exit /b 1)

REM Create a tar archive
tar -C %CD% -cf package.tar build || (echo Tar failed && exit /b 1)
