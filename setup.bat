@echo off
for %%f in (Build/ Release/) do (
    for %%i in (UI/ Types/ Graphics/ Entities/ ChunkLoader/ GraphicEffects/ GameTextures/) do (
        if not exist "%%f%%i" (mkdir "%%f%%i")
    )
)