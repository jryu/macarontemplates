#include <cmath>

#include <cairo.h>
#include <cairo-pdf.h>
#include <fcntl.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <stdio.h>

#include "config.pb.h"

config::MacaronTemplatesConfig conf;

double radius() {
	return (conf.diameter() + conf.buffer()) / 2;
}

void draw_pattern(cairo_t *cr, double x, double y)
{
	// Marker on center
	cairo_set_line_width(cr, 2);
	const double marker_size = 10;
	cairo_move_to(cr, x - marker_size, y);
	cairo_line_to(cr, x + marker_size, y);
	cairo_move_to(cr, x, y - marker_size);
	cairo_line_to(cr, x, y + marker_size);
	cairo_stroke(cr);

	// circle
	cairo_arc(cr, x, y, conf.diameter() / 2, 0, 2*M_PI);
	cairo_stroke(cr);

	double dashes[] = {5, 5};
	cairo_set_dash(cr, dashes, sizeof(dashes) / sizeof(dashes[0]), 0);
	cairo_set_line_width(cr, 1.5);

	// Smaller buffer
	cairo_arc(cr, x, y, (conf.diameter() - conf.buffer()) / 2, 0, 2*M_PI);
	cairo_stroke(cr);

	// Larger buffer 
	cairo_arc(cr, x, y, (conf.diameter() + conf.buffer()) / 2, 0, 2*M_PI);
	cairo_stroke(cr);

	cairo_set_dash(cr, nullptr, 0, 0);
}

void draw_row(cairo_t *cr, double y, bool indent)
{
	double x = radius() + conf.margin();
	if (indent) {
		x += (conf.diameter() + conf.buffer() + conf.padding()) / 2;
	}
	while (x + radius() + conf.margin() < conf.canvas_width()) {
		draw_pattern(cr, x, y);
		x += conf.diameter() + conf.buffer() + conf.padding();
	}
}

bool parse_config()
{
	// Verify that the version of the library that we linked
	// against is compatible with the version of the headers we
	// compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	int fd = open("config.txt", O_RDONLY);
	if (fd < 0) {
		return false;
	}
	google::protobuf::io::FileInputStream fileInput(fd);
	fileInput.SetCloseOnDelete(true);

	if (!google::protobuf::TextFormat::Parse(&fileInput, &conf)) {
		// protobuf prints error message
		return false;
	}
	return true;
}

int main()
{
	if (!parse_config()) {
		return EXIT_FAILURE;
	}

	cairo_surface_t *surface = cairo_pdf_surface_create("example.pdf",
			conf.canvas_width(), conf.canvas_height());
	cairo_t *cr = cairo_create(surface);

	bool indent = false;
	for (int y = radius() + conf.margin();
			y + radius() + conf.margin() < conf.canvas_height();
			y += (conf.diameter() + conf.buffer() + conf.padding()) *
			sin(M_PI / 3)) {
		draw_row(cr, y, indent);
		indent = !indent;
	}

	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	return EXIT_SUCCESS;
}
