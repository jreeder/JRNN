# JRNN
## "John Reeder's Neural Network Library"

This is a Neural Networks library in C++. 

It makes extensive use of boost shared pointers and some containers, so [Boost](http://www.boost.org) version 1.43 or greater is needed to compile against. 

The experimental programs make use of [TCLAP](tclap.sourceforge.net) for command line parsing, and xmlconfig/tinyxml for parameter file reading. 

xmlconfig is an extension of [tinyxml](http://www.grinninglizard.com/tinyxml/) that provides a very limited XPath like interface into xml files. It has built in value cacheing, and maintains awareness of the xml file state, so look ups are quick and values are up to date with the original xml file. 

Network serialization is implemented in a way that allows for several different output formats. Currently the only output format is JSON which is implemented using the [JSON-Spirit](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented) Library. 

Currently there are trainers for Standard Backprop, RProp, and Cascade Correlation. 

This software is of primary use to my disseratation research. So things are pretty specific to my needs. Once I've finished with my dissertation I'll clean this up and make it more presentable. 

## Dependencies & Use

*	[Boost](http://www.boost.org) Version >= 1.43
*	[TCLAP](tclap.sourceforge.net) Version >= 1.2.1
*	[JSON-Spirit](http://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented) Version >= 4.05

Currently only the VS2010 build environment is kept up to date. At some point I will be adding cmake support. 

***
Copyright &copy; 2012 John Reeder

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.