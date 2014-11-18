CEP - Validation
==============

Overview
--------------
This package provides some extensions to Siddhi engine to implement validation checks.

Building
--------------
The building is based on Maven.

Usage
--------------
Export the jar file and follow WSO2 instructions to install it inside the CEP. Once installed it can be used inside Siddhi queries.

**yucca:range_validity(value, min, max)** 

This function returns the validity string based on a range check. Validity strings returned by this function are:

* *"valid"*
* *"erroneous"*

The following example adds the validity string to output0 by checking that the upper limit of value is 100.

`from input0`

`select meta_source, time, value, yucca:range_validity(value, -1/0, 100) as validity`

`insert into output0`

**yucca:stddev(value)**

This is an aggregation function that computes standard deviation, and is provided as basic tool to write validation queries over statistics. Example follows:

`from input0#windows.length(50)`

`select yucca:stddev(value) as sdev`

`insert into output0`


