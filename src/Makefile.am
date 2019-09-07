%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^

dist_noinst_DATA = config.proto
BUILT_SOURCES = config.pb.cc

noinst_PROGRAMS = macarontemplates
macarontemplates_SOURCES = main.cpp config.pb.cc config.pb.h
macarontemplates_CPPFLAGS = $(CAIRO_CFLAGS)
macarontemplates_LDADD = $(CAIRO_LIBS) $(PROTOBUF_LIBS) 
