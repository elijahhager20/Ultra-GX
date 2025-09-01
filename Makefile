.SUFFIXES:
.SECONDARY:

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

TARGET	  :=  boot
BUILD	   :=  build
SOURCES	 :=  source
DATA		:=
TEXTURES	:=  textures
INCLUDES	:=

CFLAGS	  =   -g -O2 -Wall $(MACHDEP) $(INCLUDE)
CXXFLAGS	=   $(CFLAGS)
LDFLAGS	 =   -g $(MACHDEP) -Wl,-Map,$(notdir $@).map
LIBS := -lfat -lwiiuse -lbte -lasnd -logc -lm
LIBDIRS	 :=

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT   :=  $(CURDIR)/$(TARGET)
export VPATH	:=  $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(TEXTURES),$(CURDIR)/$(dir))
export DEPSDIR  :=  $(CURDIR)/$(BUILD)

CFILES	  :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES	  :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES	  :=  $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=  $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
SCFFILES	:=  $(foreach dir,$(TEXTURES),$(notdir $(wildcard $(dir)/*.scf)))
TPLFILES	:=  $(SCFFILES:.scf=.tpl)

ifeq ($(strip $(CPPFILES)),)
	export LD := $(CC)
else
	export LD := $(CXX)
endif

export OFILES := $(addsuffix .o,$(BINFILES)) \
				 $(addsuffix .o,$(TPLFILES)) \
				 $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
				 $(sFILES:.s=.o) $(SFILES:.S=.o)

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
				  $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
				  -I$(CURDIR)/$(BUILD) \
				  -I$(LIBOGC_INC)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
				   -L$(LIBOGC_LIB)

export OUTPUT := $(CURDIR)/bin/$(TARGET)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol

run:
	wiiload $(OUTPUT).dol

else

$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

%.bin.o : %.bin
	@echo $(notdir $<)
	$(bin2o)

%.tpl.o : %.tpl
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPSDIR)/*.d

endif