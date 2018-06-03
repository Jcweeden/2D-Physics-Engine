#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Game.cpp TextureManager.cpp InputHandler.cpp GameObject.cpp ShapeBody.cpp Polygon.cpp Circle.cpp ForceGenerator.cpp Contacts.cpp ShapeLinks.cpp Random.cpp Platform.cpp World.cpp BlobDemo.cpp AnchorSpringDemo.cpp ElasticMeshDemo.cpp

#CC specifies which compiler we're using
CC = g++ -std=c++11

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_gfx

#OBJ_NAME specifies the name of our executable
OBJ_NAME = Physics2D

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
