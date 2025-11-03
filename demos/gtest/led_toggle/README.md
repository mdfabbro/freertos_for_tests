# Blink

Typical blinking example.

**NOTE:** There is nothing to test here, but it can be used to make sure you
can move on to the rest of the projects.

## To Build

```bash
mkdir -p build
cd build
cmake ..
make
```

The expected binary `demo_blink` will be available in the `build` directory

## Execute

If you run it from the `build` directory, the expected output will be:

```bash
$ ./demo_blink 
Blink! Total ticks: 0
Blink! Total ticks: 1001
Blink! Total ticks: 2001
Blink! Total ticks: 3001
...
```
