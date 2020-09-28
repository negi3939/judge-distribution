################################################################################################################################################
###        In this Makefile made by taka, you can use as                                                               	                     ###
###        $ make                     # You can get the executable file which written in TARGET.                                             ###
###        $ make target=hoge         # You can get the executable file which written in hoge.                                               ###
###        $ make clean               # The executable file which written in TARGET will removed.                                            ###
###        $ make clean target=hoge   # The executable file which written in hoge will removed.                                              ###
################################################################################################################################################

ifdef target
	TARGET=$(target)
else
	TARGET=distrib
	#main
	#camera
endif

ifeq ($(TARGET),camera)
	SOURCE_MAIN = camera.cpp
	CXXFLAGS = -DCAMERA_IS_MAIN
endif

ifeq ($(TARGET),distrib)
	SOURCE_MAIN = distributioncamera.cpp
	SOURCE_SUB = camera.cpp
	CXXFLAGS = -DDISTRI_IS_MAIN
endif
ifeq ($(TARGET),main)
	SOURCE_MAIN = distributionMain.cpp 
	SOURCE_SUB = camera.cpp distributioncamera.cpp
endif


PROGRAM = $(SOURCE_MAIN:%.cpp=%.out)
SUBOBJ = $(SOURCE_SUB:%.cpp=%.o)

LDFLAGS = `pkg-config --cflags --libs opencv` -L /usr/local/lib

all: $(PROGRAM)

%.out: %.o $(SUBOBJ)
	g++ -o $@ $^ $(LDFLAGS) -w
%.o : %.cpp
	g++ -o $@ $< -c $(CXXFLAGS) -w
clean:
	rm -f *.o $(PROGRAM)