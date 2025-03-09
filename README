## Dependencies
- cmake
- build-essential
- protobuf-compiler
- python3
- python3-sklearn
- python3-protobuf

to install all,
```bash
sudo apt install cmake build-essential protobuf-compiler python3 python3-sklearn python3-protobuf
```

## Use
- compile the protobuf with `proto/compile_cpp.sh` and `evaluate/compile_py.sh`
```bash
cd proto
./compile_cpp.sh
cd ../evaluate
./compile_py.sh
```

- build the executables with `cmake` from the project root:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
- generate data with `evaluate/generate.py`
```bash
cd evaluate
python3 generate.py --dim <number of features> --type <dataset type, [blob, circle, moons, xor]> --idtype <type of the labels, [int, str]> --noise <noise level of the dataset> --vertcount <total number of vertices>
```
- evaluate the execution of all algorithms with `evaluate/evaluate.py`
```bash
cd evaluate
python3 evaluate.py --dataset <path to dataset from generate.py> --tolerance <tolerance for the filter>
```
for dimensions 2 and 3, dataset and labeled data are plotted. for higher dimensions, only the statistics table is plotted.
