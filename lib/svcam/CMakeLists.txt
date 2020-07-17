cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "SVCam")
project(${PROJECT})

include(GenerateExportHeader)

message("  -- build SVCam")

set(BASECLASSES_HEADERS
        baseclasses/amextra.h
        baseclasses/amfilter.h
        baseclasses/cache.h
        baseclasses/checkbmi.h
        baseclasses/combase.h
        baseclasses/cprop.h
        baseclasses/ctlutil.h
        baseclasses/ddmm.h
        baseclasses/dllsetup.h
        baseclasses/dxmperf.h
        baseclasses/fourcc.h
        baseclasses/measure.h
        baseclasses/msgthrd.h
        baseclasses/mtype.h
        baseclasses/outputq.h
        baseclasses/perflog.h
        baseclasses/perfstruct.h
        baseclasses/pstream.h
        baseclasses/pullpin.h
        baseclasses/refclock.h
        baseclasses/reftime.h
        baseclasses/renbase.h
        baseclasses/schedule.h
        baseclasses/seekpt.h
        baseclasses/source.h
        baseclasses/stdafx.h
        baseclasses/streams.h
        baseclasses/strmctl.h
        baseclasses/sysclock.h
        baseclasses/transfrm.h
        baseclasses/transip.h
        baseclasses/videoctl.h
        baseclasses/vtrans.h
        baseclasses/winctrl.h
        baseclasses/winutil.h
        baseclasses/wxdebug.h
        baseclasses/wxlist.h
        baseclasses/wxutil.h
        )
set(BASECLASSES_SOURCES
        baseclasses/amfilter.cpp
        baseclasses/amvideo.cpp
        baseclasses/arithutil.cpp
        baseclasses/combase.cpp
        baseclasses/cprop.cpp
        baseclasses/ctlutil.cpp
        baseclasses/ddmm.cpp
        baseclasses/dllentry.cpp
        baseclasses/dllsetup.cpp
        baseclasses/mtype.cpp
        baseclasses/outputq.cpp
        baseclasses/perflog.cpp
        baseclasses/pstream.cpp
        baseclasses/pullpin.cpp
        baseclasses/refclock.cpp
        baseclasses/renbase.cpp
        baseclasses/schedule.cpp
        baseclasses/seekpt.cpp
        baseclasses/source.cpp
        baseclasses/stdafx.cpp
        baseclasses/strmctl.cpp
        baseclasses/sysclock.cpp
        baseclasses/transfrm.cpp
        baseclasses/transip.cpp
        baseclasses/videoctl.cpp
        baseclasses/vtrans.cpp
        baseclasses/winctrl.cpp
        baseclasses/winutil.cpp
        baseclasses/wxdebug.cpp
        baseclasses/wxlist.cpp
        baseclasses/wxutil.cpp
        )

set(SOURCES
        baseclasses/dllentry.cpp
        dll.def
        dllmain.cpp
        stdafx.cpp
        stdafx.h
        SVCam.cpp
        SVCam.h
        targetver.h
        )

set(CMAKE_ENABLE_EXPORTS TRUE)
set(BUILD_SHARED_LIBS TRUE)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${PROJECT} SHARED
        ${BASECLASSES_HEADERS}
        ${SOURCES}
        )
set_source_files_properties(dll.def PROPERTIES HEADER_FILE_ONLY TRUE)

if (MSVC)
    set_target_properties(${PROJECT} PROPERTIES COMPILE_OPTIONS "/Zc:wchar_t-")
endif()
