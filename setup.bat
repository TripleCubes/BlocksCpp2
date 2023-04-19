@echo off
for %%f in (Build/ Release/) do (
    for %%i in (UI/ Types/ Graphics/ Entities/ ChunkLoader/) do (
        if not exist "%%f%%i" (mkdir "%%f%%i")
    )
)