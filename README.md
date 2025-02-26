## ABOUT *EVALUATE*

`apt install python3-protobuf` package raises `cannot import name 'runtime_version' from 'google.protobuf'`. So, a virtual environment is created to install it through `pip3 install protobuf`.
However, by the time of writing this, the latest `protobuf` python stable is `5.29.3`, while `protoc` is `6.31.0`. Since we need compatible major versions, a downgraded `protoc` is locally instaled in `evaluate`. So, to run evaluate:
- compile the proto file: `@evaluate $ ./compile_py.sh`
- activate the virtual environment: `@evaluate $ source <venv>/bin/activate`
- from there, run the scripts
