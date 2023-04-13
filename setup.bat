@echo off
for %%i in (UI/ Types/ Graphics/ Entities/ ChunkLoader/ Menus/) do (
    if not exist "./Build/%%i" (mkdir "./Build/%%i")
)
if not exist "./Release/" (mkdir "./Release/")