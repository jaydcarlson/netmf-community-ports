@echo off
SET PROJECT_DIR=%CD%
rem copy %CURRENT_DIR%\*.* %CURRENT_DIR%\backup\
set 
rem copy /y "..\Managed\Stubs\*.h" "."
copy /y "..\Managed\Stubs\*.h" %PROJECT_DIR%\
copy /y "..\Managed\Stubs\*.proj" %PROJECT_DIR%\
copy /y "..\Managed\Stubs\*.featureproj" %PROJECT_DIR%\
copy /y "..\Managed\Stubs\HAL.*" %PROJECT_DIR%\
copy /y "..\Managed\Stubs\*_mshl.*" %PROJECT_DIR%\
pause