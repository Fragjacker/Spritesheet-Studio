#include "includes/file_IO.h"
#include "includes/opencv_image.h"

// GLOBALS
Fl_Hor_Value_Slider* sliderrow = NULL;
Fl_Hor_Value_Slider* slidercol = NULL;
Fl_Output* G_filename = NULL;
Fl_Button* butsave = NULL;
Imagelist* img = NULL;
const char* G_appname = "Spritesheet-Studio";
const char* G_filterlist = "Portable network graphics\t*.png\n"
"DirectDraw Surface Image\t*.dds\n"
"Windows bitmap\t*.bmp\n"
"Portable image formats\t*.{pbm,pgm,ppm}\n"
"Sunraster\t*.{sr,ras}\n"
"JPEG\t*.{jpeg,jpg,jpe}\n"
"JPEG 2000\t*.jp2\n"
"TIFF\t*.{tiff,tif}";

void PreLoadImages(Fl_Native_File_Chooser& chooser) {
	delete img;
	std::string res;
	img = new Imagelist();
	int numImages = chooser.count();
	for (int t = 0; t < chooser.count(); t++) {
		res = getFileExt(chooser.filename(t));
		cout << res;
		loadimage(*img, (string)chooser.filename(t), res);
	}
	sliderrow->maximum(numImages);
	slidercol->maximum(numImages);
	sliderrow->value(std::ceil(std::sqrt(numImages)));
	slidercol->value(std::ceil(std::sqrt(numImages)));
	G_filename->value(std::to_string(numImages).c_str());
	slidercol->activate();
	sliderrow->activate();
	butsave->activate();
};

void ComputeSpritesheet(Fl_Widget*, void*) {
	stitchimages(*img, sliderrow->value(), slidercol->value());
};


std::string getFileExt(const char* c) {
	string s = c;
	size_t i = s.rfind('.', s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}
	return("");
};

void PickFile_CB(Fl_Widget*, void*) {
	// Create native chooser
	Fl_Native_File_Chooser native;
	native.title("Pick multiple files");
	native.type(Fl_Native_File_Chooser::BROWSE_MULTI_FILE);
	native.filter(G_filterlist);
	native.preset_file(G_filename->value());
	// Show native chooser
	switch (native.show()) {
	case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
	case  1: fprintf(stderr, "*** CANCEL\n"); break;		// CANCEL
	default: 								// PICKED FILE
		if (native.filename()) {
			G_filename->value(native.filename());
			PreLoadImages(native);
		}
		else {
			G_filename->value("NULL");
		}
		break;
	}
}

void PickDir_CB(Fl_Widget*, void*) {
	// Create native chooser
	Fl_Native_File_Chooser native;
	native.title("Pick a Directory");
	native.directory(G_filename->value());
	native.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
	// Show native chooser
	switch (native.show()) {
	case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
	case  1: fprintf(stderr, "*** CANCEL\n"); break;		// CANCEL
	default: 								// PICKED DIR
		if (native.filename()) {
			G_filename->value(native.filename());
		}
		else {
			G_filename->value("NULL");
		}
		break;
	}
}

void setupGUI(int argc, char** argv) {
	//// For a nicer looking browser under linux, call Fl_File_Icon::load_system_icons();
	//// (If you do this, you'll need to link with fltk_images)
	//// NOTE: If you do not load the system icons, the file chooser will still work, but
	////       no icons will be shown. However, this means you do not need to link in the
	////       fltk_images library, potentially reducing the size of your executable.
	//// Loading the system icons is not required by the OSX or Windows native file choosers.
#if !defined(WIN32) && !defined(__APPLE__)
	Fl_File_Icon::load_system_icons();
#endif

	int argn = 1;
#ifdef __APPLE__
	// OS X may add the process number as the first argument - ignore
	if (argc > argn && strncmp(argv[1], "-psn_", 5) == 0)
		argn++;
#endif
	Fl_Color colorbg = fl_rgb_color(60, 60, 60);
	Fl_Color colorbut = fl_rgb_color(90, 90, 90);
	Fl_Color colortext = fl_rgb_color(180, 180, 180);

	Fl_Window* win = new Fl_Window(640, 400, G_appname);
	win->size_range(win->w(), win->h(), 0, 0);
	win->color(colorbg);
	win->begin();
	{
		int x = 10, y = 10;

		G_filename = new Fl_Output(x + 70, y, 50, 25, "Num Files:");
		G_filename->box(FL_FLAT_BOX);
		G_filename->color(colorbut);
		G_filename->value("");
		G_filename->tooltip("Default filename");
		G_filename->textcolor(colortext);
		G_filename->labelcolor(colortext);

		// FileIO buttons
		Fl_Button* but = new Fl_Button(win->w() - x - 80, y, 80, 25, "Pick File");
		but->callback(PickFile_CB);
		but->box(FL_FLAT_BOX);
		but->color(colorbut);
		but->labelcolor(colortext);

		Fl_Button* butdir = new Fl_Button(but->x() - x - but->w(), y, 80, 25, "Pick Dir");
		butdir->callback(PickDir_CB);
		butdir->box(FL_FLAT_BOX);
		butdir->color(colorbut);
		butdir->labelcolor(colortext);


		butsave = new Fl_Button(win->w() - x - 140, win->h() - 25 - 10, 140, 25, "Save Spritesheet..");
		butsave->callback(ComputeSpritesheet);
		butsave->box(FL_FLAT_BOX);
		butsave->color(colorbut);
		butsave->labelcolor(colortext);
		butsave->deactivate();

		// Column and Row sliders
		y += G_filename->h() + 20;
		slidercol = new Fl_Hor_Value_Slider(x, y, (win->w() / 2) - x, 20, "Number of Columns");
		slidercol->step(1);
		slidercol->maximum(10);
		slidercol->minimum(1);
		slidercol->value(1);
		slidercol->deactivate();
		slidercol->color(colorbut);
		slidercol->textcolor(colortext);
		slidercol->labelcolor(colortext);

		sliderrow = new Fl_Hor_Value_Slider(x + slidercol->w() + 20, y, (win->w() / 2) - x - 20, 20, "Number of Rows");
		sliderrow->step(1);
		sliderrow->maximum(10);
		sliderrow->minimum(1);
		sliderrow->value(1);
		sliderrow->deactivate();
		sliderrow->color(colorbut);
		sliderrow->textcolor(colortext);
		sliderrow->labelcolor(colortext);


		y += slidercol->h() + 50;
		Fl_Help_View* view = new Fl_Help_View(x, y, win->w() * 0.8, 200);
		view->box(FL_FLAT_BOX);
		view->color(win->color());
#define TAB "&lt;Tab&gt;"
		view->textfont(FL_HELVETICA);
		view->textsize(10);
		view->textcolor(colortext);
		view->value("The Filter can be one or more filter patterns, one per line.\n"
			"Patterns can be:<ul>\n"
			"  <li>A single wildcard (e.g. <tt>\"*.txt\"</tt>)</li>\n"
			"  <li>Multiple wildcards (e.g. <tt>\"*.{cxx,h,H}\"</tt>)</li>\n"
			"  <li>A descriptive name followed by a " TAB " and a wildcard (e.g. <tt>\"Text Files" TAB "*.txt\"</tt>)</li>\n"
			"</ul>\n"
			"In the above \"Filter\" field, you can use <b><font color=#55f face=Courier>Ctrl-I</font></b> to enter " TAB " characters as needed.<br>\n"
			"Example:<pre>\n"
			"\n"
			"    Text<font color=#55f>&lt;Ctrl-I&gt;</font>*.txt\n"
			"    C Files<font color=#55f>&lt;Ctrl-I&gt;</font>*.{cxx,h,c,cpp}\n"
			"    Tars<font color=#55f>&lt;Ctrl-I&gt;</font>*.{tar,tar.gz}\n"
			"    Apps<font color=#55f>&lt;Ctrl-I&gt;</font>*.app\n");

		//win->resizable(G_filename);
	}
	win->end();
	win->show(argc, argv);
}