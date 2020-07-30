
EXTRA_CXXFLAGS = -Wno-sign-compare

include ./mkfile/kaldi.mk

CXXFLAGS += -I./ -I./include/
CXXFLAGS += -fopenmp

BINFILES = VadTest

OBJFILES = Config.o webrtc_vad.o vad_core.o vad_filterbank.o vad_gmm.o vad_sp.o SignalProcessing.o \

LIBNAME = SpeechRater

#ADDLIBS +=  $(XGBOOST)/lib/librabit.a

include ./mkfile/default_rules.mk
