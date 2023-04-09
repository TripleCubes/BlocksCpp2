@echo off
cd Release
for /R %%i in (*) do (
    if NOT %%~nxi == .gitignore (
        del %%~i
        echo deleted %%~i 
    )
)
cd ..