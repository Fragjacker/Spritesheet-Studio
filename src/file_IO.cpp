#include "includes/file_IO.h"
#include "includes/Fl_OpenCV.h"
#include "includes/opencv_image.h"

// GLOBALS
Fl_Color colorbg = fl_rgb_color(60, 60, 60);
Fl_Color colorbut = fl_rgb_color(90, 90, 90);
Fl_Color colortext = fl_rgb_color(180, 180, 180);
Fl_Hor_Value_Slider* sliderrow = NULL;
Fl_Hor_Value_Slider* slidercol = NULL;
Fl_Hor_Value_Slider* slidersplit_h = NULL;
Fl_Hor_Value_Slider* slidersplit_w = NULL;
Fl_Output* G_filename = NULL;
Fl_Input* f_input = NULL;
Fl_Button* butsave = NULL;
Fl_OpenCV* stitchResultView = NULL;
Fl_Progress* progressbar = NULL;
Imagelist* img = NULL;
list<Mat>* m = NULL;
Mat* m_temp = NULL;
const char* G_appname = "Spritesheet-Studio";
const char* G_filterlist = "Portable network graphics\t*.png\n"
"DirectDraw Surface Image\t*.dds";
//const char* G_filterlist = "Portable network graphics\t*.png\n"
//"DirectDraw Surface Image\t*.dds\n"
//"Windows bitmap\t*.bmp\n"
//"Portable image formats\t*.{pbm,pgm,ppm}\n"
//"Sunraster\t*.{sr,ras}\n"
//"JPEG\t*.{jpeg,jpg,jpe}\n"
//"JPEG 2000\t*.jp2\n"
//"TIFF\t*.{tiff,tif}";

/// <summary>
/// Loads the files, selected by the user and stores them into a list for later processing.
/// </summary>
/// <param name="chooser"></param>
void PreLoadImages(Fl_Native_File_Chooser& chooser) {
	delete img;
	img = new Imagelist();
	int num_images = chooser.count();
	int loadresult = 0, num_bad_imgs = 0;
	string bad_img_names = "";
	bool errImg = false;
	progressbar->maximum(chooser.count());
	progressbar->label("Loading Files...");
	for (int t = 0; t < chooser.count(); t++) {
		setProgressbarValue(t+1);
		loadresult = loadimage(*img, (string)chooser.filename(t), (string)getFileExt(chooser.filename(t)));
		// Show messagebox when there's an image with not matching size
		if (loadresult == 1)
		{
			bad_img_names += (string)chooser.filename(t) + "\n";
			num_bad_imgs += 1;
			errImg = true;
		}
	}
	if (errImg)
	{
		string errorstring = "At least one or more images were found with sizes, that didn't match with the first loaded image, please check the ''error_images.txt'' file which lists the offending images!\n\nThe offending images had been skipped.";
		fl_message(errorstring.c_str());

		ofstream out("error_images.txt");
		out << "Files with non-matching image sizes listed below:\n\n" << bad_img_names;
		out.close();
	}
	progressbar->label("Completed!");
	resetColsAndRows();
	//slidersplit_h->value(0);
	//slidersplit_w->value(0);
	setupUserInputs(num_images, num_bad_imgs);
	updateSpritePreview();
}

/// <summary>
/// Update the progress bar value and allocate some CPU to redraw it.
/// </summary>
/// <param name="value"></param>
void setProgressbarValue(int value) {
	progressbar->value(value);
	Fl::check();
}

void setupUserInputs(int num_images, int num_bad_imgs)
{
	int total_imgs = num_images - num_bad_imgs, num_sqrt_imgs = std::ceil(std::sqrt(total_imgs));
	sliderrow->maximum(total_imgs);
	slidercol->maximum(total_imgs);
	slidersplit_h->maximum(num_sqrt_imgs);
	slidersplit_w->maximum(num_sqrt_imgs);
	sliderrow->value(num_sqrt_imgs);
	slidercol->value(num_sqrt_imgs);
	G_filename->value(std::to_string(total_imgs).c_str());
	slidercol->activate();
	sliderrow->activate();
	slidersplit_h->activate();
	slidersplit_w->activate();
	butsave->activate();
}

/// <summary>
/// Event function to call the preview generator for the final image.
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void ComputeSpritesheet(Fl_Widget*, void*) {
	updateSpritePreview();
}
/// <summary>
/// Retrieves the Spritesheet preview image and shows it in the window as a preview.
/// </summary>
void updateSpritePreview() {
	m = stitchimages(*img, (int)sliderrow->value(), (int)slidercol->value(), (int)slidersplit_h->value(), (int)slidersplit_w->value());
	delete m_temp;
	m_temp = new Mat(*m->begin());
	resize(*m_temp, *m_temp, Size(stitchResultView->w(), stitchResultView->h()), 0, 0, INTER_CUBIC);
	stitchResultView->SetImage(m_temp);
}

/// <summary>
/// Callback function to happen when pressing the save spritesheet button.
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
void saveSpriteSheet(Fl_Widget*, void*) {
	//Setup writing the file
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	int index = 0;
	progressbar->maximum(m->size());
	progressbar->label("Writing Files...");
	setProgressbarValue(index);
	for (list<Mat>::iterator i = m->begin(); i != m->end(); i++)
	{
		index++;
		string outstr = f_input->value() + to_string(index) + ".png";
		imwrite(outstr, *i, compression_params);
		setProgressbarValue(index);
	}
	progressbar->label("Completed!");
	delete m, m_temp;
}

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
	//native.preset_file(G_filename->value());
	// Show native chooser
	switch (native.show()) {
	case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
	case  1: fprintf(stderr, "*** CANCEL\n"); break;		// CANCEL
	default: 								// PICKED FILE
		if (native.filename()) {
			//G_filename->value(native.filename());
			PreLoadImages(native);
		}
		break;
	}
}

//void PickDir_CB(Fl_Widget*, void*) {
//	// Create native chooser
//	Fl_Native_File_Chooser native;
//	native.title("Pick a Directory");
//	native.directory(G_filename->value());
//	native.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
//	// Show native chooser
//	switch (native.show()) {
//	case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
//	case  1: fprintf(stderr, "*** CANCEL\n"); break;		// CANCEL
//	default: 								// PICKED DIR
//		if (native.filename()) {
//			G_filename->value(native.filename());
//		}
//		else {
//			G_filename->value("NULL");
//		}
//		break;
//	}
//}

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
	Fl_Window* win = new Fl_Window(800, 950, G_appname);
	win->size_range(win->w(), win->h(), 0, 0);
	win->color(colorbg);
	win->begin();
	{
		int x = 10, y = 10;


		// FileIO buttons
		Fl_Button* but = new Fl_Button(x, y, 90, 25, "Pick Files...");
		but->callback(PickFile_CB);
		but->box(FL_FLAT_BOX);
		but->color(colorbut);
		but->labelcolor(colortext);

		//Fl_Button* butdir = new Fl_Button(but->x() + x + but->w(), y, 80, 25, "Pick Dir");
		//butdir->callback(PickDir_CB);
		//butdir->box(FL_FLAT_BOX);
		//butdir->color(colorbut);
		//butdir->labelcolor(colortext);

		G_filename = new Fl_Output(but->x() + x + but->w() + 90, y, 50, 25, "Loaded Files:");
		G_filename->box(FL_FLAT_BOX);
		G_filename->color(colorbut);
		G_filename->value("0");
		G_filename->tooltip("Default filename");
		G_filename->textcolor(colortext);
		G_filename->labelcolor(colortext);

		progressbar = new Fl_Progress(G_filename->x() + x + G_filename->w(), y, 150, 25, "");
		progressbar->maximum(100);
		progressbar->value(0);
		progressbar->selection_color(FL_GREEN);
		progressbar->color(colorbut);

		butsave = new Fl_Button(win->w() - x - 140, y, 140, 25, "Save Spritesheet..");
		butsave->callback(ComputeSpritesheet);
		butsave->box(FL_FLAT_BOX);
		butsave->color(colorbut);
		butsave->labelcolor(colortext);
		butsave->deactivate();
		butsave->callback(saveSpriteSheet);

		f_input = new Fl_Input(win->w() - x - butsave->w() - 160, y, 140, 25);
		f_input->labelcolor(colortext);
		f_input->label("Filename:");
		f_input->box(FL_FLAT_BOX);
		f_input->color(colorbut);
		f_input->textcolor(colortext);
		f_input->value("spritesheet");

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
		slidercol->callback(ComputeSpritesheet);

		sliderrow = new Fl_Hor_Value_Slider(x + slidercol->w() + 20, y, (win->w() / 2) - x - 20, 20, "Number of Rows");
		sliderrow->step(1);
		sliderrow->maximum(10);
		sliderrow->minimum(1);
		sliderrow->value(1);
		sliderrow->deactivate();
		sliderrow->color(colorbut);
		sliderrow->textcolor(colortext);
		sliderrow->labelcolor(colortext);
		sliderrow->callback(ComputeSpritesheet);

		y += sliderrow->h() + 20;
		slidersplit_h = new Fl_Hor_Value_Slider(x, y, (win->w() / 2) - x, 20, "Split into horizontal blocks");
		slidersplit_h->step(2);
		slidersplit_h->maximum(10);
		slidersplit_h->minimum(0);
		slidersplit_h->value(0);
		slidersplit_h->deactivate();
		slidersplit_h->color(colorbut);
		slidersplit_h->textcolor(colortext);
		slidersplit_h->labelcolor(colortext);
		slidersplit_h->callback(ComputeSpritesheet);

		slidersplit_w = new Fl_Hor_Value_Slider(x + slidercol->w() + 20, y, (win->w() / 2) - x - 20, 20, "Split into vertical blocks");
		slidersplit_w->step(2);
		slidersplit_w->maximum(10);
		slidersplit_w->minimum(0);
		slidersplit_w->value(0);
		slidersplit_w->deactivate();
		slidersplit_w->color(colorbut);
		slidersplit_w->textcolor(colortext);
		slidersplit_w->labelcolor(colortext);
		slidersplit_w->callback(ComputeSpritesheet);


		//		y += slidercol->h() + 50;
		//		Fl_Help_View* view = new Fl_Help_View(x, y, win->w() * 0.8, 200);
		//		view->box(FL_FLAT_BOX);
		//		view->color(win->color());
		//#define TAB "&lt;Tab&gt;"
		//		view->textfont(FL_HELVETICA);
		//		view->textsize(10);
		//		view->textcolor(colortext);
		//		view->value("The Filter can be one or more filter patterns, one per line.\n"
		//			"Patterns can be:<ul>\n"
		//			"  <li>A single wildcard (e.g. <tt>\"*.txt\"</tt>)</li>\n"
		//			"  <li>Multiple wildcards (e.g. <tt>\"*.{cxx,h,H}\"</tt>)</li>\n"
		//			"  <li>A descriptive name followed by a " TAB " and a wildcard (e.g. <tt>\"Text Files" TAB "*.txt\"</tt>)</li>\n"
		//			"</ul>\n"
		//			"In the above \"Filter\" field, you can use <b><font color=#55f face=Courier>Ctrl-I</font></b> to enter " TAB " characters as needed.<br>\n"
		//			"Example:<pre>\n"
		//			"\n"
		//			"    Text<font color=#55f>&lt;Ctrl-I&gt;</font>*.txt\n"
		//			"    C Files<font color=#55f>&lt;Ctrl-I&gt;</font>*.{cxx,h,c,cpp}\n"
		//			"    Tars<font color=#55f>&lt;Ctrl-I&gt;</font>*.{tar,tar.gz}\n"
		//			"    Apps<font color=#55f>&lt;Ctrl-I&gt;</font>*.app\n");

		y += slidersplit_h->h() + 50;
		Fl_Group* grp = new Fl_Group(x, y, win->w() * 0.97, win->h() * 0.815);
		stitchResultView = new Fl_OpenCV(x, y, win->w() * 0.97, win->h() * 0.815);
		grp->resizable(stitchResultView);
		grp->color(fl_rgb_color(20, 20, 20));
		grp->box(FL_FLAT_BOX);
		grp->labelcolor(colortext);
		grp->label("Image preview\n(Preview stretches to fill the window, final image is unstretched)");
		grp->end();
		win->resizable(grp);
	}
	win->end();
	win->show(argc, argv);
}