# Conway-
Let's try to create something before I die of sadness

### Prerequisites

To run this code, you’ll need to install the `cJSON` library. On Linux, you can install it with:

```bash
sudo apt-get install libcjson-dev
```

### JSON File Structure

Your JSON file should look like this:

```json
{
    "cells": [
        { "x": 5, "y": 5 },
        { "x": 5, "y": 6 },
        { "x": 5, "y": 7 },
        { "x": 6, "y": 6 }
    ]
}
```
Each line is a cell's coordinate. Do not try putting cells outside the grid, I haven't tried to fix an issue that does not occur in a normal context of utilisation.

### Running the Program

To run the program:

```bash
gcc -o game_of_life game_of_life.c -lcjson
./game_of_life configuration.json
```

The program will display each generation and can be paused or stopped as needed with "p" to pause and "q" to resume.
