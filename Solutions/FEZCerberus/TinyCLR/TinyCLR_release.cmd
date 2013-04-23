call C:\MicroFrameworkPK_v4_3\setenv_gcc.cmd 4.6.2 c:\GCC_4_6
msbuild /t:build /p:flavor=release;memory=flash TinyCLR.proj
