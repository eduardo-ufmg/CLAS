include ../../common_mk/flags.mk

# Target executable
TARGET := test

.SUFFIXES:

# Source files
SRCS := $(wildcard *.cpp)
SRCS += "classifier.pb.cc"

make_debug:
	echo $(SRCS)

# Include directories
INCS := -I../Common -I$${HOME}/protobuf/src -I$${HOME}/abseil-cpp

include ../../common_mk/files.mk

include ../../common_mk/rules.mk

include ../../common_mk/targets.mk
