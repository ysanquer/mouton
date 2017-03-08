SERVER_BIN=moutond
CLIENT_BIN=mouton
PROJECT_DOC=doc

JACORB_BIN=/home/ysanquer/bin/jacorb-3.8/bin

DOXYGEN=doxygen
CPP=g++
CPPARGS_GEN=-g -ggdb -std=c++11 -Wfatal-errors
CPPARGS=-g -ggdb -Wall -Wextra -Weffc++ -std=c++11 -Wfatal-errors
INCLUDEARGS=-Igeometry -Imain-client -Imain-server -Irender -Ishape-el-parser -Ishell -Iutil -Inamed-drawing -Iannotated-drawing -IomniEvents -Ievent-channel-servants -Iglobals -Iexcept
DEFINEARGS=-D__x86__ -D__linux__ -D__OSVERSION__=3 -D_REENTRANT -DHAVE_IOSTREAM -DHAVE_STD_IOSTREAM -DHAVE_OMNIORB4 -DHAVE_IOMANIP -DHAVE_STDLIB_H
LDARGS=-lomniORB4 -lomnithread -lomniDynamic4 -lomniCodeSets4 -lCOS4 -lCOSDynamic4 -lomniEvents
IDLCPP=omniidl
IDLCPPARGS=-bcxx -Wba
IDLJ=$(JACORB_BIN)/idl

CLIENT_OBJ=obj/mouton_main.o obj/ProcShell.o obj/Vector.o obj/AbstractLine.o obj/AbstractDefinedLine.o obj/DefinedLine.o obj/DefinedLineSegment.o obj/VectorRefSegment.o obj/Polygon.o obj/Circle.o obj/Ellipse.o obj/Image.o obj/ShapeELParser.o obj/LogRenderEngine.o obj/PullSupplierImpl.o obj/named-drawingSK.o obj/named-drawingDynSK.o obj/annotated-drawingSK.o obj/annotated-drawingDynSK.o obj/PushConsumerImpl.o obj/naming.o obj/global_verbosity.o
SERVER_OBJ=obj/moutond_main.o obj/global_verbosity.o obj/naming.o obj/ProcShell.o obj/ShapeELParser.o obj/LogRenderEngine.o obj/Vector.o obj/PullConsumerImpl.o obj/PushSupplierImpl.o obj/DefinedLine.o obj/DefinedLineSegment.o obj/Polygon.o obj/Circle.o obj/Ellipse.o obj/Image.o obj/AbstractDefinedLine.o obj/AbstractLine.o obj/VectorRefSegment.o obj/named-drawingSK.o obj/named-drawingDynSK.o obj/annotated-drawingSK.o obj/annotated-drawingDynSK.o
OBJ=$(CLIENT_OBJ) $(SERVER_OBJ)

GEN=named-drawing/named-drawing.hh named-drawing/named-drawingSK.cc named-drawing/named-drawingDynSK.cc annotated-drawing/annotated-drawing.hh annotated-drawing/annotated-drawingSK.cc annotated-drawing/annotated-drawingDynSK.cc java-client/gen

SUBDIRS=doc obj named-drawing annotated-drawing java-client/gen

CLEAN=$(OBJ) $(GEN)
MRPROPER=$(CLIENT_BIN) $(SERVER_BIN) $(SUBDIRS)


all: init $(CLIENT_BIN) $(SERVER_BIN)


$(CLIENT_BIN): $(CLIENT_OBJ)
	@echo "Linking together $^ into $@"
	@$(CPP) $(LDARGS) $^ -o $@

$(SERVER_BIN): $(SERVER_OBJ)
	@echo "Linking together $^ into $@"
	@$(CPP) $(LDARGS) $^ -o $@

$(PROJECT_DOC):
	@echo "Generating Doxygen documentation"
	@$(DOXYGEN) Doxyfile

java-client/gen/fr/upem/mouton/AnnotatedDrawing%: idl/annotated-drawing.idl
	@echo "Generating $@ from $<"
	@$(IDLJ) -d java-client/gen $<

java-client/gen/fr/upem/mouton/NamedDrawing%: idl/named-drawing.idl
	@echo "Generating $@ from $<"
	@$(IDLJ) -d java-client/gen $<

obj/mouton_main.o: main-client/mouton_main.cpp shell/ProcShell.h named-drawing/named-drawing.hh annotated-drawing/annotated-drawing.hh globals/global_verbosity.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/moutond_main.o: main-server/moutond_main.cpp
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/ProcShell.o: shell/ProcShell.cpp shell/ProcShell.h shell/Shell.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/Vector.o: geometry/Vector.cpp geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/AbstractLine.o: geometry/AbstractLine.cpp geometry/AbstractLine.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/AbstractDefinedLine.o: geometry/AbstractDefinedLine.cpp geometry/AbstractDefinedLine.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/DefinedLine.o: geometry/DefinedLine.cpp geometry/DefinedLine.h geometry/AbstractDefinedLine.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/DefinedLineSegment.o: geometry/DefinedLineSegment.cpp geometry/DefinedLineSegment.h geometry/AbstractDefinedLine.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/VectorRefSegment.o: geometry/VectorRefSegment.cpp geometry/VectorRefSegment.h geometry/AbstractLine.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/Polygon.o: geometry/Polygon.cpp geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/Circle.o: geometry/Circle.cpp geometry/Circle.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/Ellipse.o: geometry/Ellipse.cpp geometry/Ellipse.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/Image.o: geometry/Image.cpp geometry/Image.h geometry/Shape.h geometry/Vector.h geometry/Transformable.h geometry/Metric.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/ShapeELParser.o: shape-el-parser/ShapeELParser.cpp shape-el-parser/ShapeELParser.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/LogRenderEngine.o: render/LogRenderEngine.cpp render/LogRenderEngine.h render/RenderEngine.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/PullSupplierImpl.o: event-channel-servants/PullSupplierImpl.cpp event-channel-servants/PullSupplierImpl.h named-drawing/named-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/named-drawingDynSK.o: named-drawing/named-drawingDynSK.cc named-drawing/named-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS_GEN) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/named-drawingSK.o: named-drawing/named-drawingSK.cc named-drawing/named-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS_GEN) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

named-drawing/%: idl/named-drawing.idl
	@echo "Generating $@ from $<"
	@$(IDLCPP) $(IDLCPPARGS) -Cnamed-drawing $<

obj/annotated-drawingDynSK.o: annotated-drawing/annotated-drawingDynSK.cc annotated-drawing/annotated-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS_GEN) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/annotated-drawingSK.o: annotated-drawing/annotated-drawingSK.cc annotated-drawing/annotated-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS_GEN) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

annotated-drawing/%: idl/annotated-drawing.idl
	@echo "Generating $@ from $<"
	@$(IDLCPP) $(IDLCPPARGS) -Cannotated-drawing $<

obj/PushConsumerImpl.o: event-channel-servants/PushConsumerImpl.cpp event-channel-servants/PushConsumerImpl.h annotated-drawing/annotated-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/PullConsumerImpl.o: event-channel-servants/PullConsumerImpl.cpp event-channel-servants/PullConsumerImpl.h named-drawing/named-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/PushSupplierImpl.o: event-channel-servants/PushSupplierImpl.cpp event-channel-servants/PushSupplierImpl.h annotated-drawing/annotated-drawing.hh
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/naming.o: omniEvents/naming.cc omniEvents/naming.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@

obj/global_verbosity.o: globals/global_verbosity.cpp globals/global_verbosity.h
	@echo "Building $< into $@"
	@$(CPP) $(CPPARGS) $(INCLUDEARGS) $(DEFINEARGS) -c $< -o $@



.PHONY: init doc clean mrproper

init:
	@mkdir -p $(SUBDIRS)

doc: $(PROJECT_DOC)


clean:
	@rm -vrf $(CLEAN)

mrproper: clean
	@rm -vrf $(MRPROPER)

