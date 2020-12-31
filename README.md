# Hide Explorer Search Bar
Very simple executable that hides the search bar in File Explorer and in the Open/Save dialogs in Windows 10.

Download binaries from [releases](https://github.com/valinet/HideExplorerSearchBar/releases).

Simply execute the downloaded executable file and that's it. To modify common dialogs in elevated applications, run the program as administrator. The 32-bit version works on both 32-bit and 64-bit systems.

To compile, use the included Visual Studio solution or, from a command window, do:

```
cl.exe /nologo /DUNICODE /Oi /GS- main.c Kernel32.lib User32.lib /link /RELEASE /NODEFAULTLIB /ENTRY:wWinMain
```

* `/Oi` will enable the use of `wcscmp` intrinsic (compilation is done without default library)
* `/Gs-` disables security checks (same reason, unavailable when compiling without default library)

Licensed under [MIT](https://github.com/valinet/HideExplorerSearchBar/blob/master/LICENSE).