.PHONY = all clean build test picture
CC = g++ -std=c++11 -pthread -O3

# SRCS := $(shell find . -name "*.cpp")
# BINS := $(SRCS:%.cpp=%.o)
MATHSRC := $(wildcard src/math/*.cpp)
MATHBIN := $(MATHSRC:%.cpp=%.o)

clean:
	rm -rvf $(shell find . -name "*.o") *.exe *.out

all: build

math: ${MATHBIN}

ray.o: src/math/ray.cpp vec3.o
	${CC} -c src/math/ray.cpp

vec3.o: src/math/vec3.cpp
	${CC} -c src/math/vec3.cpp

# sphere.o: src/math/sphere.cpp vec3.o
# 	${CC} -c src/math/sphere.cpp

hittablelist.o: src/math/hittablelist.cpp
	${CC} -c src/math/hittablelist.cpp

picture: clean sphere.o hittablelist.o ray.o vec3.o
	${CC} -o picture.out src/picture.cpp $(shell find . -name "*.o")

render: clean hittablelist.o ray.o vec3.o
	${CC} -o render.out $(shell find . -name "*.o") src/render.cpp 
	./render.out
# render: clean
# 	${CC} -o render.out src/render.cpp src/math/ray.cpp src/math/vec3.cpp src/math/hittablelist.cpp $(shell find . -name "*.o")
# 	./render.out

atmosphere: clean hittablelist.o ray.o vec3.o
	${CC} -o atmosphere.out $(shell find . -name "*.o") src/atmosphere.cpp 
	./atmosphere.out

random: clean hittablelist.o ray.o vec3.o
	${CC} -o random.out src/random.cpp $(shell find . -name "*.o")
	./random.out


test: clean vec3.o ray.o
	${CC} -o test.out src/test.cpp $(shell find . -name "*.o") 

fxaa: vec3.o
	${CC} -o FXAA.out src/FXAA.cpp vec3.o
	./FXAA.out

# %.o: %.cpp
# 	${CC} -c $<

# picture: clean src/picture.cpp ${MATHBIN} 
# 	${CC} -o picture.out src/picture.cpp $(shell find . -name "*.o")

# test: clean src/test.cpp ${MATHBIN} 
# 	${CC} -o test.out src/test.cpp $(shell find . -name "*.o")