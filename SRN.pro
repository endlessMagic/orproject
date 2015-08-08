SOURCES += \
    main.cpp \
    MVA/Schwitzer_MVA.cpp \
    MVA/Linearizer_MVA.cpp \
    MVA/Exact_MVA.cpp \
    MVA/MVA.cpp \
    Station/STATION.cpp \
    Station/Phased_Station.cpp \
    Station/Markov_Phased_Station.cpp \
    Station/Rolia_MultiStation.cpp \
    Station/Simple_Phased_Station.cpp \
    Station/PS_Station.cpp \
    Station/Rolia_Phased_Station.cpp \
    Station/Delay_Station.cpp \
    Station/FCFS_Station.cpp \
    SRNSolver.cpp

HEADERS += \
    SRNSolver.h \
    MVA/MVA.h \
    MVA/Exact_MVA.h \
    MVA/Linearizer_MVA.h \
    MVA/Schwitzer_MVA.h \
    Station/Delay_Station.h \
    Station/STATION.h \
    Station/Delay_Station.h \
    Station/Markov_Phased_Station.h \
    Station/Phased_Station.h \
    Station/Rolia_MultiStation.h \
    Station/FCFS_Station.h \
    Station/Simple_Phased_Station.h \
    Station/Rolia_Phased_Station.h \
    Station/PS_Station.h


CONFIG += c++11
