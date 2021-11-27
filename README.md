# CPP-Class-Boilerplate-Generator

I am tired of writing the same code again and again for getters and setters of attributes of a class with differences only in attribute name and data type. This code generates a header file (with extension .h) and a source file (with extension .cpp) for a class with a list of attributes in the input file.


>.h file contains the basic boilerplate code of a class including attributes and prototypes of setter and getter for each attribute.

>.cpp file contains the implementation code of setter and getter for each attribute.
 



## Input Format:

```
className
DataType attribute1
DataType attribute2
DataType attribute3
```

## Output:

- className.h file
- className.cpp file
