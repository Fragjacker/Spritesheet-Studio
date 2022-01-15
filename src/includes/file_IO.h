#include <string>		/* strstr() */
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Help_View.H>
#include <Fl/Fl_Hor_Value_Slider.H>

void PickFile_CB(Fl_Widget*, void*);
void PickDir_CB(Fl_Widget*, void*);
void ComputeSpritesheet(Fl_Widget*, void*);
std::string getFileExt(const char* c);
void setupGUI(int argc, char** argv);
void PreLoadImages(Fl_Native_File_Chooser& chooser);