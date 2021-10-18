include Makefile.inc

build: $(BIN)/${HL_TARGET}/tonemapping.a

export LD_LIBRARY_PATH=/mnt/d/Software/Halide-12/distrib/lib

$(GENERATOR_BIN)/tonemapping.generator: tonemapping_generator.cpp utils/util.cpp $(GENERATOR_DEPS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -g $(filter %.cpp,$^) -o $@ $(LIBHALIDE_LDFLAGS) -ljpeg -lpng -rdynamic -larmadillo

$(BIN)/%/tonemapping.a: $(GENERATOR_BIN)/tonemapping.generator
	@mkdir -p $(@D)
	$^ -g tonemapping -o $(@D) -f tonemapping target=$*

$(BIN)/%/main: main.cpp $(BIN)/%/tonemapping.a utils/util.cpp
	@mkdir -p $(@D)
	g++ -I /mnt/d/Software/Halide-12/distrib/include -I /mnt/d/Software/Halide-12/distrib/tools -I ./ \
	-L /mnt/d/Software/Halide-12/distrib/lib $^ -o $@ -rdynamic -lHalide -lpthread -ldl -ljpeg -lpng -larmadillo -std=c++11

$(BIN)/%/output.png: $(BIN)/%/main 
	@mkdir -p $(@D)
	$< 

test: $(BIN)/${HL_TARGET}/output.png

clean:
	rm -rf $(BIN)
