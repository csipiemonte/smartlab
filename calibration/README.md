Calibration
==============

The calibration module is used to build a model that is able
to correct sensor's readings.

Usually the calibration is a process that involves a manual procedure
where the operator makes a series of observations against reference
values.

This reference values may be given by a reference measure instrument
or by altering the physical environment that the sensor is measuring
by a known quantity.

In any case the result of this operation is a table with two columns:
the first column contains the reference (expected) values while the second
column contains the output of the sensor.

Starting from this information, the module will compute a polynomial
approximation of a function. The domain of this function is between
the min and max value of measured values (second column).

Once the function has been found, the module will be able 
to correct a value using this function.

The module also provide a _toString()_ method that will generate
a CSV representation of the function. This CSV can be sent to
the firmware of a smart object (e.g. Arduino) to provide this
kind of correction on board.

The actual implementation uses Apache Commons Math3 (<http://commons.apache.org/proper/commons-math/>)
spline interpolation. For this kind of interpolation the CSV
has the following format.
Given N, the number of observations in input during the calibration, the
CSV will contain N lines.
The first N-1 lines contains the coefficient of the polynomials
of third degree plus the value x that denotes the start value of
the interval of values that are the domain for the given polynomial.

As an example consider the following input CSV:

    1 , 1.2
    3 , 3.05
    4 , 4.3
    5 , 5.5

For a reference value of 1 (in the example) the sensor measure was 1.2, and so on...

The spline interpolation will generate the following CSV:
    
    1.2,1.0,1.172174456055778,0.0,-0.026616033593775606
    3.05,3.0,0.8988943311316869,-0.14771898644545461,0.05488281723208411
    4.3,4.0,0.7868600707934446,0.0580915781748608,-0.016136549493016884
    5.5,,,,

The value will be corrected if it is between 1.2 and 5.5.
Between 1.2 and 3.05, the first line of coefficients must be
used:
    1.0,1.172174456055778,0.0,-0.026616033593775606
    
In this specific case, given x the value to be corrected:
    f(x) = 1.0 + 1.172174456055778 * (x - 1.2) + 0.0 * (x - 1.2)^2 - 0.026616033593775606 (x-1.2)^3


Building
--------------
Copy commons-math3-3.3.jar inside the folder lib/, that run build.xml ant script

Usage
--------------
The file _Example.java_ provide a simple code that reads the input CSV,
initializes the calibration module and than prints the CSV representation 
of the spline polynomial function.
This sample program also prints the correction of a given value.
Both input CSV file name and value to be corrected are given using the command line.
