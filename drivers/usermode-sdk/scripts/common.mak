#  
#  Unified Building System for Linux
#  By CSK (csk@live.com)
#  

.SUFFIXES: .o .cpp .s .cc

.PHONY: make_subs clean install additional_clean 
# collect objs to build
OBJ+= $(patsubst %.c, $(OBJ_ROOT)/%.o,$(filter-out %*.c,$(CSRC)) )
OBJ+= $(patsubst %.cpp, $(OBJ_ROOT)/%.o,$(filter-out %*.cpp,$(CXXSRC)))
OBJ+= $(patsubst %.cc, $(OBJ_ROOT)/%.o,$(filter-out %*.cc,$(CXXCCSRC)))



distclean: additional_distclean clean

clean: additional_clean
	rm -f $(OBJ)
	rm -f $(DEP_FILE)


$(OBJ_ROOT)/%.o: %.cpp
	mkdir -p `dirname $@`
	$(CXX) -c $(CXXFLAGS) -fPIC -o $@ $<

$(OBJ_ROOT)/%.o: %.c
	mkdir -p `dirname $@`
	$(CC) -c $(CFLAGS) -fPIC -o $@ $<

$(OBJ_ROOT)/%.o: %.cc
	mkdir -p `dirname $@`
	$(CXX) -c $(CXXFLAGS) -fPIC -o $@ $<


#Dependency builing
$(OBJ_ROOT)/%.d: %.c
	mkdir -p `dirname $@`
	$(CC) -M $(CFLAGS) $< | sed "s;$(notdir $*).o:;$(OBJ_ROOT)/$*.o $(OBJ_ROOT)/$*.d:;" > $@

$(OBJ_ROOT)/%.d: %.cpp
	mkdir -p `dirname $@`
	$(CXX) -M $(CXXFLAGS) $< | sed "s;$(notdir $*).o:;$(OBJ_ROOT)/$*.o $(OBJ_ROOT)/$*.d:;" > $@

$(OBJ_ROOT)/%.d: %.cc
	mkdir -p `dirname $@`
	$(CXX) -M $(CXXFLAGS) $< | sed "s;$(notdir $*).o:;$(OBJ_ROOT)/$*.o $(OBJ_ROOT)/$*.d:;" > $@

$(EXEC_DEST): $(OBJ) $(DEP_AR)
	mkdir -p `dirname $@`
	$(CC) -o $@ $^ -L$(OUTPUT_ROOT) $(LDFLAGS)


$(STATIC_DEST): $(OBJ)
	mkdir -p `dirname $@`
	@for i in $(OBJ); do echo "$@<=$$i"; $(AR) rcs $@ $$i; done

$(DYNAMIC_DEST): $(OBJ) $(DEP_AR)
	mkdir -p `dirname $@`
	$(CC)  -fPIC -shared -o $@ $^ -L$(OUTPUT_ROOT) $(LDFLAGS)

ifneq ($(MAKECMDGOALS), clean)
sinclude $(DEP_FILE)
endif
