@echo off
cd Build
for /R %%i in (*) do (
    del %%~i
    echo deleted %%~i 
)
cd ..