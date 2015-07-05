INCLUDEPATH += \
   ../3rdparty/yaml-cpp/include

win32:INCLUDEPATH += "$(BOOST_ROOT)/include"

SOURCES += \
    ../3rdparty/yaml-cpp/src/binary.cpp \
    ../3rdparty/yaml-cpp/src/convert.cpp \
    ../3rdparty/yaml-cpp/src/directives.cpp \
    ../3rdparty/yaml-cpp/src/emit.cpp \
    ../3rdparty/yaml-cpp/src/emitfromevents.cpp \
    ../3rdparty/yaml-cpp/src/emitter.cpp \
    ../3rdparty/yaml-cpp/src/emitterstate.cpp \
    ../3rdparty/yaml-cpp/src/emitterutils.cpp \
    ../3rdparty/yaml-cpp/src/exp.cpp \
    ../3rdparty/yaml-cpp/src/memory.cpp \
    ../3rdparty/yaml-cpp/src/node.cpp \
    ../3rdparty/yaml-cpp/src/node_data.cpp \
    ../3rdparty/yaml-cpp/src/nodebuilder.cpp \
    ../3rdparty/yaml-cpp/src/nodeevents.cpp \
    ../3rdparty/yaml-cpp/src/null.cpp \
    ../3rdparty/yaml-cpp/src/ostream_wrapper.cpp \
    ../3rdparty/yaml-cpp/src/parse.cpp \
    ../3rdparty/yaml-cpp/src/parser.cpp \
    ../3rdparty/yaml-cpp/src/regex_yaml.cpp \
    ../3rdparty/yaml-cpp/src/scanner.cpp \
    ../3rdparty/yaml-cpp/src/scanscalar.cpp \
    ../3rdparty/yaml-cpp/src/scantag.cpp \
    ../3rdparty/yaml-cpp/src/scantoken.cpp \
    ../3rdparty/yaml-cpp/src/simplekey.cpp \
    ../3rdparty/yaml-cpp/src/singledocparser.cpp \
    ../3rdparty/yaml-cpp/src/stream.cpp \
    ../3rdparty/yaml-cpp/src/tag.cpp \
    ../3rdparty/yaml-cpp/src/contrib/graphbuilder.cpp \
    ../3rdparty/yaml-cpp/src/contrib/graphbuilderadapter.cpp

HEADERS += \
    ../3rdparty/yaml-cpp/include/yaml-cpp/anchor.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/binary.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/dll.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/emitfromevents.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/emitter.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/emitterdef.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/emittermanip.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/emitterstyle.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/eventhandler.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/exceptions.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/mark.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/noncopyable.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/null.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/ostream_wrapper.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/parser.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/stlemitter.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/traits.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/yaml.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/contrib/anchordict.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/contrib/graphbuilder.h \
    ../3rdparty/yaml-cpp/src/collectionstack.h \
    ../3rdparty/yaml-cpp/src/directives.h \
    ../3rdparty/yaml-cpp/src/emitterstate.h \
    ../3rdparty/yaml-cpp/src/emitterutils.h \
    ../3rdparty/yaml-cpp/src/exp.h \
    ../3rdparty/yaml-cpp/src/indentation.h \
    ../3rdparty/yaml-cpp/src/nodebuilder.h \
    ../3rdparty/yaml-cpp/src/nodeevents.h \
    ../3rdparty/yaml-cpp/src/ptr_stack.h \
    ../3rdparty/yaml-cpp/src/ptr_vector.h \
    ../3rdparty/yaml-cpp/src/regex_yaml.h \
    ../3rdparty/yaml-cpp/src/regeximpl.h \
    ../3rdparty/yaml-cpp/src/scanner.h \
    ../3rdparty/yaml-cpp/src/scanscalar.h \
    ../3rdparty/yaml-cpp/src/scantag.h \
    ../3rdparty/yaml-cpp/src/setting.h \
    ../3rdparty/yaml-cpp/src/singledocparser.h \
    ../3rdparty/yaml-cpp/src/stream.h \
    ../3rdparty/yaml-cpp/src/streamcharsource.h \
    ../3rdparty/yaml-cpp/src/stringsource.h \
    ../3rdparty/yaml-cpp/src/tag.h \
    ../3rdparty/yaml-cpp/src/token.h \
    ../3rdparty/yaml-cpp/src/contrib/graphbuilderadapter.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/convert.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/emit.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/impl.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/iterator.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/node.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/parse.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/ptr.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/type.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/bool_type.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/impl.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/iterator.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/iterator_fwd.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/memory.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/node.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/node_data.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/node_iterator.h \
    ../3rdparty/yaml-cpp/include/yaml-cpp/node/detail/node_ref.h
