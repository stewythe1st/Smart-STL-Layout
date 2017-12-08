# Evolutionary STL Layouts:

An Application of the 2D Bin Packing Problem

### Makes Use of

The following third party libraries and specifications were used.

* C++11
* Arash Pratow's [C++ Bitmap library](http://partow.net/programming/bitmap/)

### Running

#### Linux

Use the the included ```compile.sh``` compile the program. You will need to supply the configuration file as an argument. If no specific configuration file is supplied, it will assume ```default.cfg```.

```bash
./stl-layout.out default.cfg
```

#### Windows
Open the .sln file in Visual Studio. Build and run withing th IDE. To specifiy a configuration file, modify the project properties to add a runtime arguments.

```
 default.cfg
```

### Output

The program will output the following files. The filename of each will match the configuration name specified earlier in the cfg file.

* Log CSV```(log/example.csv)``` - Logs all per-generation data to a csv for analysis.
* Log CSV```(log/example_best.csv)``` - Logs data about each run's final best population.
* Standard Out - Mirrors the logfile.
* Solution Image```(img/example_run_0.bmp)``` - Prints a bitmap image of each generation's best solution.

### Author

Stuart Miller - sm67c@mst.edu - [Github](https://github.com/stewythe1st)
