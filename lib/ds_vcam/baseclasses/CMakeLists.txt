cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "baseclasses")
project(${PROJECT})
message("    -- build ${PROJECT}")

include(GenerateExportHeader)

set(BASECLASSES_HEADERS
        amextra.h
        amfilter.h
        cache.h
        checkbmi.h
        combase.h
        cprop.h
        ctlutil.h
        ddmm.h
        dllsetup.h
        dxmperf.h
        fourcc.h
        measure.h
        msgthrd.h
        mtype.h
        outputq.h
        perflog.h
        perfstruct.h
        pstream.h
        pullpin.h
        refclock.h
        reftime.h
        renbase.h
        schedule.h
        seekpt.h
        source.h
        stdafx.h
        streams.h
        strmctl.h
        sysclock.h
        transfrm.h
        transip.h
        videoctl.h
        vtrans.h
        winctrl.h
        winutil.h
        wxdebug.h
        wxlist.h
        wxutil.h
        )
set(BASECLASSES_SOURCES
        amfilter.cpp
        amvideo.cpp
        arithutil.cpp
        combase.cpp
        cprop.cpp
        ctlutil.cpp
        ddmm.cpp
        dllentry.cpp
        dllsetup.cpp
        mtype.cpp
        outputq.cpp
        perflog.cpp
        pstream.cpp
        pullpin.cpp
        refclock.cpp
        renbase.cpp
        schedule.cpp
        seekpt.cpp
        source.cpp
        stdafx.cpp
        strmctl.cpp
        sysclock.cpp
        transfrm.cpp
        transip.cpp
        videoctl.cpp
        vtrans.cpp
        winctrl.cpp
        winutil.cpp
        wxdebug.cpp
        wxlist.cpp
        wxutil.cpp
        )

add_library(${PROJECT} STATIC ${BASECLASSES_HEADERS})
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

#if (MSVC)
#    set_target_properties(${PROJECT} PROPERTIES COMPILE_OPTIONS "/Zc:wchar_t-")
#endif()
