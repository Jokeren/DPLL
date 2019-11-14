PROJECT := tiny_sat
CONFIGS := Makefile.config

include $(CONFIGS)

.PHONY: clean all objects install

LIB_DIR := lib/
INC_DIR := include/
BUILD_DIR := build/

LIB := $(LIB_DIR)libtiny_sat.a

ifdef DEBUG
OFLAGS += -g -DDEBUG
endif

ifdef PERFORMANCE
OFLAGS += -O3 -ffast-math -DPERFORMANCE

ifdef ARCH
OFLAGS += -march=$(ARCH)
endif

ifdef SIMD
OFLAGS += -m$(SIMD)
endif

ifdef CPU
OFLAGS += -mcpu=$(CPU)
endif

ifdef TUNE
OFLAGS += -mtune=$(TUNE)
endif
endif

ifdef JEMALLOC
LD_JEMALLOC = -L$(JEMALLOC) -ljemalloc
endif

CFLAGS := -std=c++11 $(OFLAGS)
LDFLAGS := -lstdc++ $(LD_JEMALLOC)
SRCS := $(shell find src -maxdepth 3 -name "*.cc")
OBJECTS := $(addprefix $(BUILD_DIR), $(patsubst %.cc, %.o, $(SRCS)))
OBJECTS_DIR := $(sort $(addprefix $(BUILD_DIR), $(dir $(SRCS))))
BINS := $(PROJECT)

all: dirs bins objects lib
dirs: $(OBJECTS_DIR) $(LIB_DIR) $(BIN_DIR)
bins: $(BINS)
objects: $(OBJECTS)
lib: $(LIB)

$(OBJECTS_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

$(BINS): % : %.cc $(LIB)
	$(MCXX) $(CFLAGS) -I$(INC_DIR) -I$(CXXOPTS) $^ -o $@ $(LDFLAGS) -L$(LIB_DIR)

$(LIB): $(OBJECTS)
	ar crf $@ $^ 

$(OBJECTS): $(BUILD_DIR)%.o : %.cc
	$(MCXX) $(CFLAGS) -I$(INC_DIR) -o $@ -c $<

clean:
	-rm -rf $(BUILD_DIR) $(LIB_DIR) $(BINS)

ifdef PREFIX
install:
	mkdir -p $(PREFIX)/$(BUILD_DIR)
	mkdir -p $(PREFIX)/$(INC_DIR)
	cp -rf $(LIB_DIR) $(PREFIX)
	cp -rf $(INC_DIR) $(PREFIX)
endif

#utils
print-% : ; $(info $* is $(flavor $*) variable set to [$($*)]) @true
