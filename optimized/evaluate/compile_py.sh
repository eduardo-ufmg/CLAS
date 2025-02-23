#!/bin/bash

./protoc-5.29.3/bin/protoc --proto_path=../proto/ --python_out=. classifier.proto
