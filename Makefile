# Makefile không cần dùng brew, tự chọn OpenSSL path thủ công

# Kiểm tra đường dẫn OpenSSL (ưu tiên Homebrew M1/M2 trước)
ifeq ($(wildcard /opt/homebrew/opt/openssl@3/include/openssl/sha.h),)
    OPENSSL_PREFIX = /usr/local/opt/openssl@3
else
    OPENSSL_PREFIX = /opt/homebrew/opt/openssl@3
endif

# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Include và Lib path
INCLUDES = -I$(OPENSSL_PREFIX)/include
LIBS = -L$(OPENSSL_PREFIX)/lib -lssl -lcrypto -lcurl

# Tên chương trình
TARGET = wallet_system
SRC = main.cpp

# Build rule
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SRC) $(LIBS)

# Clean
clean:
	rm -f $(TARGET)
