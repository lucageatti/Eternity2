EASYLOCAL = ../easylocal-3
FLAGS = -std=c++11 -Wall #-O3 # -g
COMPOPTS = -I$(EASYLOCAL)/include $(FLAGS)
EASYLOCALLIB = -lEasyLocal -lboost_program_options
LINKOPTS = -L$(EASYLOCAL)/lib $(EASYLOCALLIB)  -pthread

SOURCE_FILES = Eternity2_Data.cc Eternity2_Basics.cc Eternity2_Helpers.cc  Eternity2_Main.cc
OBJECT_FILES = Eternity2_Data.o Eternity2_Basics.o Eternity2_Helpers.o Eternity2_Main.o
HEADER_FILES = Eternity2_Data.hh Eternity2_Basics.hh Eternity2_Helpers.hh  

Eternity2: $(OBJECT_FILES)
	g++ $(OBJECT_FILES) $(LINKOPTS) -o Eternity2

Eternity2_Data.o: Eternity2_Data.cc Eternity2_Data.hh
	g++ -c $(COMPOPTS) Eternity2_Data.cc

Eternity2_Basics.o: Eternity2_Basics.cc Eternity2_Basics.hh Eternity2_Data.hh
	g++ -c $(COMPOPTS) Eternity2_Basics.cc

Eternity2_Helpers.o: Eternity2_Helpers.cc Eternity2_Helpers.hh Eternity2_Basics.hh Eternity2_Data.hh
	g++ -c $(COMPOPTS) Eternity2_Helpers.cc

Eternity2_Main.o: Eternity2_Main.cc Eternity2_Helpers.hh Eternity2_Basics.hh Eternity2_Data.hh
	g++ -c $(COMPOPTS) Eternity2_Main.cc

clean:
	rm -f $(OBJECT_FILES) Eternity2

