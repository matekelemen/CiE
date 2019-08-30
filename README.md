# Splinecomputation

The goal of this example project is to compute and visualize (b-)spline curves 
and basis functions. This involves rather expensive numerical computations as 
well as a graphical representation of the results. The former would suggest an 
implementation in a low-level language such as C or C++. While such languages 
can deliver superior performance, implementing a visualization can be quite 
tedious. It involves either platform dependent code or using third party 
libraries. Therefore, it would be preferable to use an interpreted language such 
as python, which provides modules for simple plotting or even graphical user 
interfaces. 

Such scenarios are commonly resolved by implementing a core library in C++ and
introducing python bindings for selected components. That is, these components
(functions, classes, etc.) are exported (wrapped) to a library object that can 
be imported in python. There are libraries specifically for this task; we will 
be using pybind11 (https://github.com/pybind/pybind11). This way, the 
performance of C++ can be combined with the versatility of python.

While we could just set up this project in our favorite IDE, this has some 
significant drawbacks. We might not be able to use our project on a different 
operating system, the IDE might not be supported in the future, there might 
be version conflicts and so on. We go a different path by using CMake 
(https://cmake.org/) which is both, a scripting language and a software tool.
It allows to script the complete build process and generate project files from
these scripts. The user can choose for which tool the project files shall be
generated. There are many target build environments available such as MS Visual
Studio and Unix makefiles. This choice can now be made by the user rather
than the developer.

And as every project should, we test our C++ core to have some confidence in 
the correctness of the implementation. To this end, we use the catch2 
(https://github.com/catchorg/Catch2) testing framework which exists in a 
single-header version.

Last, but not least, as you can see we use git as version control system. A 
pretty neat feature of git is to have submodules, which allows to embedd 
other git repositories without them being part of the repository. We added
pybind11 as submodule, so you will not see their code here, but if you clone
this repository with --recursive, git will automatically clone pybind11 too.  

## Setup and build

In order to compile and run this project you need the following tools:
* CMake (at least version 3.4.3): https://cmake.org/download/
* A build automation tool, e.g. MS Visual Studio on Windows, Unix makefiles, 
  Eclipse IDE, ...
* A C++ compiler (+ linker), e.g. MSVC (coming with MS Visual Studio), GCC, 
  clang, ...
* A python installation providing numpy and matplotlib  

This is the general procedure how to set up the splinecomputation project:
* Setup the tools listed above
* Clone the project by typing 
  <code>git clone --recursive https://gitlab.lrz.de/cie/splinecomputation.git source</code>
  in a terminal. This will clone into a local folder named source. As we
  specified <code>--recursive</code> it will clone pybind11 as well.
* Setup a build project using CMake. Specify the 
  <code>CMAKE_INSTALL_PREFIX</code> where you want the binaries and scripts. 
  Make sure that <code>PYTHON_EXECUTABLE</code> under advanced is set correctly.
* Compile your build project and install. This depends on the build system you 
  chose. E.g. you open Visual Studio and build the <code>INSTALL</code> target.

Now you can go into your install folder, run the testrunner and execute the 
python scripts, e.g. 
<pre><code>python plotBSplineCurve.py</pre></code> 

and
<pre><code>python plotBSplineBasis.py</pre></code>

This will give you the two plots:

![](doc/png/plots.png)

The third script 

<pre><code>python  plotBSplineCurveDeBoor.py</pre></code> 

gives  you exactily the same curves as plotBSplineCurve.py, but using the 
algorithm of DeBoor, i.e. without computing the basis functions. 

Note, that when we are talking about executing programs in the terminal (such as 
git), we assume those programs are in your operating systems PATH environment 
variable. The OS will look up programs in this path, such that we can skip the
path to the executable and just type its name. If that is not the case the 
absolute path to the executable needs to be added.

### On Windows

On Windows we tested the following environments:

**Microsoft Visual Studio**

To set up splinecomputation as Visual Studio project follow the setup steps above
and you will create a Visual Studio project. Once you open that you will find 
one project for each CMake target. You can build and install by right-clicking
on the <code>INSTALL</code> target and <code>build</code>.

**MinGW64**

MinGW64 is a clone of gcc for Windows and comes with its implementation of the
make program known from Unix systems. It turned out to be a bit tricky and you 
might enter some complications. These links might help:

* Compilation error similar to "hypot is not defined" and linker error saying 
  "undefined reference to __imp_Py_InitModule4":
  https://github.com/pybind/pybind11/issues/1342
* Your linker complains about "plugin needed to handle lto object":
  https://stackoverflow.com/questions/32221221/mingw-x64-windows-plugin-needed-to-handle-lto-object
* Just a side note: It turns out the -Wextra flag leads to compilation errors 
  with pybind11

See *On Ubuntu* below on how to use make.

### On Ubuntu

On Ubuntu we have tried Unix makedfiles and the Eclipse IDEs using also makefiles.
Generally you follow the steps above. Once you have set up your build project open
a terminal, go to the build folder, build the project typing <code>make -j4</code>, 
where <code>-j4</code> specifies that we want to compile with 4 cores. Now install 
by typing <code>make install</code>.

