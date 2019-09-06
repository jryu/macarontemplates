#include <cmath>

#include <cairo.h>
#include <cairo-pdf.h>

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

	// 1.5 inch circle
	cairo_arc(cr, x, y, 75, 0, 2*M_PI);
	cairo_stroke(cr);

	double dashes[] = {5, 5};
	cairo_set_dash(cr, dashes, sizeof(dashes) / sizeof(dashes[0]), 0);
	cairo_set_line_width(cr, 1.5);

	// 1.25 inch circle
	cairo_arc(cr, x, y, 62.5, 0, 2*M_PI);
	cairo_stroke(cr);

	// 1.75 inch circle
	cairo_arc(cr, x, y, 87.5, 0, 2*M_PI);
	cairo_stroke(cr);

	cairo_set_dash(cr, nullptr, 0, 0);
}

int main()
{
	cairo_surface_t *surface = cairo_pdf_surface_create("example.pdf",
			200, 200);
	cairo_t *cr = cairo_create(surface);

	draw_pattern(cr, 100, 100);

	cairo_destroy(cr);
	cairo_surface_destroy(surface);
	return 0;
}
