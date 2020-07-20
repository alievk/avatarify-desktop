cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT "baseclasses")
project(${PROJECT})
message("    -- build ${PROJECT}")

set(HEADERS
        amextra.h
        amfilter.h
        cache.h
        combase.h
        cprop.h
        ctlutil.h
        dllsetup.h
        fourcc.h
        measure.h
        msgthrd.h
        mtype.h
        outputq.h
        pstream.h
        refclock.h
        reftime.h
        renbase.h
        source.h
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

add_library(${PROJECT} STATIC ${HEADERS})
set_target_properties(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)

#if (MSVC)
#    set_target_properties(${PROJECT} PROPERTIES COMPILE_OPTIONS "/Zc:wchar_t-")
#endif()
