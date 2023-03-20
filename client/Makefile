CCPP:=gcc

CXXFLAGS = $(shell pkg-config --cflags libcbor)
LDFLAGS = $(shell pkg-config --libs libcbor)

all : client_sin_wave
client_sin_wave: client_sin_wave.o
	$(CCPP) $(CXXFLAGS) -o $@ $< $(LDFLAGS) -lm
%.o : %.cpp
	$(CCPP) $(CXXFLAGS) -o $@ -c $<
clean:
	rm -f *.o *~ client_sin_wave
