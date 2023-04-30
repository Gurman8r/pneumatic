-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --

-- zip
cpp_project_common("Modules", "zip", "StaticLib")

targetdir("%{_BUILD_BIN}")

includedirs{
	"%{_THIRDPARTY}minizip/",
	"%{_THIRDPARTY}zlib/",
}

files{
	"%{_MODULES}zip/zip.premake5.lua",
	"%{_THIRDPARTY}minizip/**.h", "%{_THIRDPARTY}minizip/**.c",
	"%{_THIRDPARTY}zlib/**.h", "%{_THIRDPARTY}zlib/**.c",
}

-- * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * --