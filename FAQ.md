

# Introduction #
Just a FAQ

# Q&A #
  * Q: I've imported the project to Visual Studio 2008, and the project builds just fine. However when I try to start the application it gives me the following error: _Error Application failed to initialize properly 0xc0150002_
  * A: The reason for this error is that the included OpenThreads library (**.lib and**.dll) was build with Visual Studio 2005. To fix the problem, download OpenThreads from http://openthreads.sourceforge.net/ and compile it with Visual Studio 2008. Next replace the **.lib (3rdparty\OpenThreads\lib) and**.dll (\bin) file.