# Playbook Creator

A free, open-source editor for American (Flag) Football Playbooks

## How to build

### Linux (Ubuntu)

```
apt-get install git clang cmake pkg-config qt5-default libbotan-2-dev libboost-serialization-dev python3
cmake .
make
```

### OS X

```
brew install cmake boost botan qt
cmake -DCMAKE_PREFIX_PATH=$(brew --prefix qt5)
make
```
