################################################################################################################################################
###        In this Makefile made by taka, you can use as                                                               	                     ###
###        $ make                     # You can get the executable file which written in TARGET. And the executable file will run.           ###
###        $ make target=hoge         # You can get the executable file which written in hoge. And the executable file will run.             ###
###        $ make argv=hoge           # You can get the executable file which written in TARGET. And the executable file will run with argv. ###
###        $ make notrun=1            # You can get the executable file which written in TARGET. The executable file will not run.		     ###
###        $ make clean               # The executable file which written in TARGET will removed.                                            ###
###        $ make clean target=hoge   # The executable file which written in hoge will removed.                                              ###
################################################################################################################################################

ifdef target
	TARGET=$(target)
else
	TARGET=camera
	#distrib

endif

ifeq ($(TARGET),camera)
	SOURCE_MAIN = camera.cpp
#	SOURCE_SUB = serial.cpp
endif

ifeq ($(TARGET),distrib)
	SOURCE_MAIN = distributioncamera.cpp
	SOURCE_SUB = camera.cpp
endif

ifdef argv
	COMMAND = echo run;./$(SOURCE_MAIN:%.cpp=%.out) argv
else
	COMMAND = echo run;./$(SOURCE_MAIN:%.cpp=%.out)
endif
ifdef notrun
		COMMAND = echo You got $(SOURCE_MAIN:%.cpp=%.out).
endif

PROGRAM = $(SOURCE_MAIN:%.cpp=%.out)
SUBOBJ = $(SOURCE_SUB:%.cpp=%.o)

CFLAGS = `pkg-config --cflags --libs opencv`
LDFLAGS = `pkg-config --cflags --libs opencv` -L /usr/local/lib

all: $(PROGRAM)

%.out: %.o $(SUBOBJ)
	g++ -o $@ $^ $(LDFLAGS) -w
	#$(COMMAND)
%.o : %.cpp
	g++ -o $@ $< -c $(CXXFLAGS) -w
clean:
	rm -f *.o $(PROGRAM)
