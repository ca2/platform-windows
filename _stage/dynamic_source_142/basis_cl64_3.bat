chcp 65001
call "%VS_VARS%" %VS_VARS_PLAT2%



cl /Yu"framework.h" /MP /GS /Qpar- /W3 /Gy- /Zc:wchar_t /I"%CA2_ROOT%include" /I"%CA2_ROOT%app/include" /I"." /I"%CA2_ROOT%app-core/netnode_dynamic_source_script/framework" /I"%CA2_ROOT%" /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/base/include" /I"%CA2_ROOT%app/base" /I"%CA2_ROOT%app" /I"%CA2_ROOT%app/aura" /I"%CA2_ROOT%app/base" /I"%CA2_ROOT%app/axis" /I"%CA2_ROOT%app/aura" /I"%CA2_ROOT%app/axis" /I"%CA2_ROOT%platform/thirdparty" /I"%CA2_ROOT%platform/seed/metaseed/../../" /I"%CA2_ROOT%app-core/netnode_dynamic_source_script/../.." /I"%CA2_ROOT%app-core/netnode_dynamic_source_script/../../" /I"%CA2_ROOT%app-core/netnode_dynamic_source_script/../" /I"%CA2_ROOT%app-core/netnode_dynamic_source_script/" /I"%CA2_ROOT%app-core/netnode_dynamic_source_script/" /I"%NETNODE_ROOT%net/netseed" /I"%NETNODE_ROOT%net/netseed/include" /I"%CA2_ROOT%time/dynamic_source/library/include" /Zi /Gm- /Od /Ob0 /sdl /Fd"%CA2_ROOT%time/intermediate/x64/basis/netnode_dynamic_source_script/vc140.pdb" /Zc:inline /fp:fast /D "_DYNAMIC_SOURCE_SCRIPT_LIBRARY" /D "_WINDLL" /D "_MBCS" /fp:except- /errorReport:prompt /GF- /WX- /Zc:forScope /RTC1 /GR /Gd /Oy- /MDd /openmp- /Fa"%CA2_ROOT%time/intermediate/x64/basis/netnode_dynamic_source_script/" /EHa /nologo /Fo"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/" /Fp"%CA2_ROOT%time/intermediate/x64/basis/netnode_dynamic_source_script/netnode_dynamic_source_script.pch" /Zm384 /bigobj /c "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%.cpp" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-compile-log.txt" 


link /OUT:"%TARGET_PATH%.dll" /MANIFEST:NO /NXCOMPAT /PDB:"%TARGET_PATH%.pdb" /DYNAMICBASE "aura.lib" "auracharguess.lib"  "aurasqlite.lib"  "axis.lib" "axisbzip2.lib" "axiscrypto.lib" "axisfreeimage.lib" "axisidn.lib" "libmysql.lib" "axisopenssl.lib" "axiszlib.lib" "base.lib" "aura.lib" "data.lib" "html.lib" "html_lite.lib" "math.lib" "netnode.lib" "programming.lib" "time.lib" "userdatetime.lib" "app_core_alarm.lib" "aqua.lib" /DEF:"%CA2_ROOT%app-core/netnode_dynamic_source_script/netnode_dynamic_source_script.def" /DEBUG /FIXED:NO /IMPLIB:"%CA2_ROOT%time/library/x64/basis/netnode_dynamic_source_script.lib" /DLL /MACHINE:X64 /INCREMENTAL /SUBSYSTEM:WINDOWS /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /ManifestFile:"%CA2_ROOT%time/intermediate/x64/basis/netnode_dynamic_source_script/netnode_dynamic_source_script.dll.intermediate.manifest" /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"%CA2_ROOT%time/library/x64/basis"  /LIBPATH:"%CA2_ROOT%time/x64/basis" /LIBPATH:"%CA2_ROOT%platform/thirdparty/library/x64/basis" /TLBID:1   "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/framework.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/netnode_dynamic_source_script.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/netnode_dynamic_source_script/%ITEM_NAME%/%ITEM_TITLE%.obj" > "%CA2_ROOT%time/dynamic_source/%ITEM_NAME%-link-log.txt"



