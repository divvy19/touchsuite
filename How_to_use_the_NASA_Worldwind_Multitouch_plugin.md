**How to build and use the NASA Worldwind multitouch plugin**



# Requirements #
  * WinXP or higher.
  * [TortoiseSVN](http://www.tortoisesvn.net/downloads)
  * [Touchlib](http://code.google.com/p/touchlib) ([install instructions](http://www.multigesture.net/articles/touchlib-compiling-instructions/)).
  * DirectX SDK ([latest version](http://msdn.microsoft.com/en-us/directx/aa937788.aspx)).

# Instructions #

## Building the TouchlibCOMwrapper ##
  1. [Compile touchlib](http://www.multigesture.net/articles/touchlib-compiling-instructions/)
  1. Use TortoiseSVN to do a checkout of http://touchsuite.googlecode.com/svn/trunk/Csharp/
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww1%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww1.jpg)
  1. Open up TouchlibCOMwrapper.sln in the TouchlibCOMwrapper directory.
  1. Press ALT-F7 to open up the project properties.
  1. Set configuration to Release.
  1. Go to C/C++ > General and open up "Additional Include Directories".
  1. Replace "$(SolutionDir)/../touchlib/include" with the path to your touchlib include directory.
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww2%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww2.jpg)
  1. Go to Linker > General and open up "Additional Library Directories".
  1. Replace "$(SolutionDir)/../touchlib/lib" with the path to your touchlib lib directory.
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww3%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww3.jpg)
  1. Click OK.
  1. Set the project mode to release mode and press F7 (Build solution).
  1. In the directory "Csharp\TouchlibCOMwrapper\release" there should be a file TouchlibCOMwrapper.lib and TouchlibCOMwrapper.dll. We will need these files later on.
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww4%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww4.jpg)

## Register the TouchlibCOMwrapper ##
  1. I have included a compiled build of TouchlibCOMwrapper.idl, if you need to recompile it, please read the instructions in readme\_COM.txt
  1. To register the wrapper double click the following batch file: RegisterTouchlibCOMwrapper(tlb).bat.
  1. You could also run the batch file from a commandline box:
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww5%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww5.jpg)
  1. If you don't register this file, you will not be able to add a reference to this wrapper to the C# project.

## Compiling NASA World Wind and the Multitouch plug-in ##
  1. Use [7-zip](http://www.7-zip.org/) to decompress World\_Wind\_1.4.0\_Source.7z (directory: Csharp\Nasa\_World\_Wind\_plugin ).
  1. Put Multitouch.cs in \World\_Wind\_1.4.0\_Source\WorldWind\
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww6%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww6.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww7%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww7.jpg)
  1. Open WorldWind.sln
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww8%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww8.jpg)
  1. Add the file (Multitouch.cs) to the project "WorldWind" in the solution explorer
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww9%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww9.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww10%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww10.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww11%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww11.jpg)
  1. Make sure that the wrapper library is installed (if you haven't already done it, run RegisterTouchlibCOMwrapper(tlb).bat once)
  1. Add a reference (Rightclick on the left on WorldWind>References>Add Reference) to the TouchlibCOMwrapper.tlb (which is located in the TouchlibCOMwrapper directory).
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww12%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww12.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww13%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww13.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww14%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww14.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww15%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww15.jpg)
  1. ~~Set the project to Release Mode and x86~~
  1. Set the project to Debug Mode and 'Any CPU'(for now the project won't build in release mode)
  1. Build the application (Press F7).
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww16%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww16.jpg)
  1. Copy over all touchlib dll and touchlib config files, and also copy TouchlibCOMwrapper.dll to the bin dir of NWW (World\_Wind\_1.4.0\_Source\World\_Wind\_1.4.0\_Source\bin\Debug).
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww17%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww17.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww18%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww18.jpg)
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww19%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww19.jpg) -> ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww20%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww20.jpg)
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww21%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww21.jpg)
  1. Run NWW (WorldWind.exe)
  1. Load the 'multitouch' plugin from the plugin window
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww22%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww22.jpg)
  1. Select Multitouch from the list and click on Load
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww23%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww23.jpg)
  1. After clicking on Load, it should start Touchlib in the background (video dialogs should appear)
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww24%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww24.jpg)
  1. Enable fullscreen:
  1. ![![](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww25%20(WinCE).jpg)](http://touchsuite.googlecode.com/svn/site/images/worldwind/nww25.jpg)

## Using the Multitouch plug-in in NASA World Wind ##
  1. Enable the fullscreen mode
  1. Single touch == pan earth
  1. Two touches == zoom/rotate

### Adjusting sensitivity (Requires re-compiling the source) ###
  1. Panning: Change the value of lines 207 and 208 (Multitouch.cs) from 3000 to another value.
  1. Zoom: Change the value of line 64 (Multitouch.cs) (RotationFactor)
  1. Rotate: Change the value of line 63 (Multitouch.cs) (ZoomFactor)

# Known issues #
  * Currently you can only load the plug-in once. Unloading and loading the plugin again will not work. You will need to restart NASA World Wind.
  * Crash on exit