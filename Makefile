BIN             := normalize reconstruct_ser reconstruct_parallel reconstruct_par

INCLUDES        :=
LIBS            := -lgomp

CFLAGS          := -O3 -m64
LDFLAGS         := -fopenmp

CXX             := g++
MPIXX           := mpic++

C_SOURCCES		:=
HEADERS         :=

C_OBJS          :=


normalize: normalize.cpp pgmio.hpp
	$(CXX) $(CFLAGS) -o $@ normalize.cpp pgmio.cpp

reconstruct_ser: reconstruct_ser.cpp pgmio.hpp
	$(CXX) $(CFLAGS) -o $@ reconstruct_ser.cpp pgmio.cpp

reconstruct_par: reconstruct_par.cpp pgmio.hpp
	$(MPIXX) $(CFLAGS) -o $@ reconstruct_par.cpp pgmio.cpp


clean:
	rm $(BIN) *.o core*
