How to use this plugin.

1. Download World_Wind_1.4.0_Source.zip from the Nasa website or use World_Wind_1.4.0_Source.7z
2. Unzip
3. Put Multitouch.cs in \World_Wind_1.4.0_Source\WorldWind\
4. Open WorldWind.sln
5. Add the file to the project "WorldWind" in the solution explorer
6. Make sure that the wrapper library is installed (run RegisterTouchlibCOMwrapper(tlb).bat once)
7. Add a reference to the TouchlibCOMwrapper.tlb (which is located in the TouchlibCOMwrapper directory)
8. Build the application.
9. Copy over all touchlib dll files, and also copy TouchlibCOMwrapper.dll to the bin dir of NWW.
10. Run NWW
11. Load the plugin from the plugin window

Todo:
1. Fix loading/unloading the plugin.
Currently you can only load the plugin once. Unloading and Loading again won't work. You will need to restart the application.


