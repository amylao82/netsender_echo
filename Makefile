
SUBDIRS := netsender

INC_DIR += -Inetsender/include

CFLAGS += -g

LD_LIBS := -pthread -lrt
LD_LIBS += -Lnetsender -lnetsender

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))  
EXES := $(patsubst %.cpp,%.o,$(SRCS))  

TARGETS := $(patsubst %.cpp,%,$(SRCS))
  
all : submodule $(TARGETS)

submodule: $(SUBDIRS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done

% : %.cpp
	$(CXX) $(CFLAGS) $< -o $(subst .o, ,$@) $(LD_LIBS) $(INC_DIR)


clean:
	rm $(TARGETS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean || exit 1; \
	done
