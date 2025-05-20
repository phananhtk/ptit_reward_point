# Makefile cho WalletSystem
OPENSSL_PREFIX = /mingw64
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -I$(OPENSSL_PREFIX)/include
LIBS = -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -lcurl
TARGET = wallet_system
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
    $(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LIBS)

clean:
    rm -f $(TARGET)
