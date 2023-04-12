OBJS = globals 					$\
		Graphics/frameBuffer 	$\
		Graphics/shader 		$\
		Graphics/mesh 			$\
		Graphics/texture 		$\
		Graphics/graphics 		$\
		Graphics/text			$\
		UI/ui 					$\
		UI/button 				$\
		ChunkLoader/chunk 		$\
		ChunkLoader/chunkLoader $\
		Entities/entity 		$\
		Entities/player 		$\
		Entities/camera 		$\
		Entities/raycast 		$\
		Types/vectormath 		$\
		Types/color 			$\
		Types/rect				$\
		time 					$\
		math 					$\
		threadControls 			$\
		
INCLUDES = -I"./Libs/glfw-3.3.8.bin.WIN64/include" $\
		-I"./Libs/glad/include" $\
		-I"./Libs/glm" $\
		-I"./Libs/FastNoiseLite" $\
		-I"./Libs/stb_image" $\
		-I"./Libs/FreeType/include" $\

LIBS = -L"./Libs/glfw-3.3.8.bin.WIN64/lib-mingw-w64" $\
		-L"./Libs/FreeType/Binaries" $\

FLAGS = -lglfw3 -lopengl32 -lgdi32 -l:freetype.a

OBJPATHS = $(foreach obj,$(OBJS),./Build/$(obj).o)

define NEWLINE


endef

rebuild:
	$(foreach obj,$(OBJS),g++ -c ./Codes/$(obj).cpp -o ./Build/$(obj).o $(INCLUDES) $(LIBS) $(FLAGS)$(NEWLINE))
	g++ -c ./Codes/main.cpp -o ./Build/main.o $(INCLUDES) $(LIBS) $(FLAGS)
	gcc -c ./Libs/glad/src/glad.c -o ./Build/glad.o $(INCLUDES) $(LIBS) $(FLAGS) 
	g++ -c ./Codes/stb_image.cpp -o ./Build/stb_image.o -I"./Libs/stb_image"
	g++ -o ./Build/blocks.exe $(OBJPATHS) ./Build/stb_image.o ./Build/glad.o ./Build/main.o $(INCLUDES) $(LIBS) $(FLAGS) -static-libgcc -static-libstdc++

buildRelease:
	g++ -o ./Release/blocks.exe $(OBJPATHS) ./Build/stb_image.o ./Build/glad.o ./Build/main.o $(INCLUDES) $(LIBS) $(FLAGS) -mwindows -static-libgcc -static-libstdc++

./Build/blocks.exe: $(OBJPATHS) ./Build/main.o ./Build/glad.o ./Build/stb_image.o
	g++ -o ./Build/blocks.exe $(OBJPATHS) ./Build/stb_image.o ./Build/glad.o ./Build/main.o $(INCLUDES) $(LIBS) $(FLAGS) -static-libgcc -static-libstdc++

./Build/main.o: ./Codes/main.cpp
	g++ -c ./Codes/main.cpp -o $@ $(INCLUDES) $(LIBS) $(FLAGS)

./Build/glad.o:
	gcc -c ./Libs/glad/src/glad.c -o $@ $(INCLUDES) $(LIBS) $(FLAGS) 

./Build/stb_image.o: ./Codes/stb_image.cpp
	g++ -c ./Codes/stb_image.cpp -o $@ -I"./Libs/stb_image"

$(OBJPATHS): ./Build/%.o: ./Codes/%.cpp ./Codes/%.h
	g++ -c $< -o $@ $(INCLUDES) $(LIBS) $(FLAGS)