@echo off
cd Release
for /R %%i in (*) do (
    del %%~i
    echo deleted %%~i 
)
cd ..