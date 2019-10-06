
CXXFLAGS=-O2 -std=c++11 -Wall -Wextra -pedantic -I..

all: rsa-keygen.exe rsa-encode.exe rsa-decode.exe

rsa-keygen.exe: rsa_keygen.cpp rsa.h rsa_key.h
	$(LINK.cpp) rsa_keygen.cpp $(LDFLAGS) -o $@

rsa-encode.exe: rsa_encode.cpp rsa.h rsa_code.h rsa_key.h
	$(LINK.cpp) rsa_encode.cpp $(LDFLAGS) -o $@

rsa-decode.exe: rsa_decode.cpp rsa.h rsa_code.h rsa_key.h
	$(LINK.cpp) rsa_decode.cpp $(LDFLAGS) -o $@

.PHONY: clean rsa-keygen rsa-encode rsa-decode

clean:
	del rsa-keygen.exe rsa-encode.exe rsa-decode.exe

rsa-keygen: rsa-keygen.exe
rsa-encode: rsa-encode.exe
rsa-decode: rsa-decode.exe
