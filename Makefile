# This file contains a make script for the ProofOfWorker application
#
# Author: Josh McIntyre
#

# This block defines makefile variables
LIB_FILES=src/lib/proofOfWorker.js src/lib/sha256.js

BUILD_DIR=bin/proofOfWorker

# This rule builds the lib
build: $(LIB_FILES)
	mkdir -p $(BUILD_DIR)
	cp $(LIB_FILES) $(BUILD_DIR)

# This rule cleans the build directory
clean: $(BUILD_DIR)
	rm $(BUILD_DIR)/*
	rmdir $(BUILD_DIR) 
