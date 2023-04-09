@echo off
for %%i in (UI/ Types/ Graphics/ Entities/ ChunkLoader/) do (
    if not exist "./Build/%%i" (mkdir "./Build/%%i")
)
if not exist "./Release/" (mkdir "./Release/")