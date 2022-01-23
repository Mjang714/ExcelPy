# ExcelPy
a project that uses xlwings and python with pybind11. This is a basic starter project hopefully I can build out some unique stuff.

Here are some important things to note about this project

1. When compiling the python code I used python 3.10. Make sure you use the same version or recompile the .pyd again
	a. Python binding leverages pybind11 for the compilation, thus if you choose to recompile the .pyd you will need pybind11 installed locally
	b. if you need to recompile follow the instruction laid out here: https://docs.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022

2. Currently working adding Excel interface to the function though not sure how long that will take. 


