#!/bin/bash

protoc --proto_path=../proto/ --python_out=. classifier.proto
