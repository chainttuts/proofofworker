# This file contains a make script for the ProofOfWorker application
#
# Author: Josh McIntyre
#

# This block defines makefile variables
LIB_FILES=src/lib/proofofworker.js src/lib/sha256.js
SERVER_FILES=src/server/*.c
SERVER_INCLUDES=src/server
SERVER_SERVICE=src/server/proofofworker.service
CC=gcc
FLAGS=-I$(SERVER_INCLUDES) -lssl -lcrypto
MIDDLEWARE_FILES=src/middleware/*.php

BUILD_DIR=bin/proofofworker
SERVER_DIR=bin/proofofworker_server
SERVER_BIN=proofofworker
MIDDLEWARE_DIR=bin/proofofworker_middleware

# This rule builds the lib
build: $(LIB_FILES)
	mkdir -p $(BUILD_DIR)
	cp $(LIB_FILES) $(BUILD_DIR)

# This rule builds the server
server: $(SERVER_FILES)
	mkdir -p $(SERVER_DIR)
	$(CC) -o $(SERVER_DIR)/$(SERVER_BIN) $(SERVER_FILES) $(FLAGS)
	cp $(SERVER_SERVICE) $(SERVER_DIR)

# This rule builds verification middleware
middleware: $(MIDDLEWARE_FILES)
	mkdir -p $(MIDDLEWARE_DIR)
	cp $(MIDDLEWARE_FILES) $(MIDDLEWARE_DIR)

# This rule cleans the build directories
clean: $(BUILD_DIR) $(SERVER_DIR) $(MIDDLEWARE_DIR)
	rm $(BUILD_DIR)/* $(SERVER_DIR)/* $(MIDDLEWARE_DIR)/*
	rmdir $(BUILD_DIR) $(SERVER_DIR) $(MIDDLEWARE_DIR)
