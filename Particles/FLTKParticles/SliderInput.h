

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>

class SliderInput : public Fl_Group {
    Fl_Value_Slider    *slider;

	double		*attatchedValue_;

    // CALLBACK HANDLERS
    //    These 'attach' the input and slider's values together.
    //
    void Slider_CB2() {
        static int recurse = 0;
        if ( recurse ) { 
            return;
        } else {
            recurse = 1;
            char s[80];
            sprintf(s, "%d", (int)(slider->value() + .5));
			*attatchedValue_ = slider->value();
            // fprintf(stderr, "SPRINTF(%d) -> '%s'\n", (int)(slider->value()+.5), s);
            //input->value(s);          // pass slider's value to input
            recurse = 0;
        }
    }

    static void Slider_CB(Fl_Widget *w, void *data) {
        ((SliderInput*)data)->Slider_CB2();
    }

public:
    // CTOR
    SliderInput(int x, int y, int w, int h, double *connectedValue, const char *l=0) : Fl_Group(x,y,w,h) {

        slider = new Fl_Value_Slider(x, y, w, h);
		slider->step(0.01);
        slider->type(1);
        slider->callback(Slider_CB, (void*)this);

		slider->label(l);

		attatchedValue_ = connectedValue;

        bounds(1, *attatchedValue_);     // some usable default
        value(*attatchedValue_);          // some usable default
        end();             // close the group
    }

    // MINIMAL ACCESSORS --  Add your own as needed
    int  value() const    { return((int)(slider->value() + 0.5)); }
    void value(int val)   { slider->value(val); Slider_CB2(); }
    void minumum(int val) { slider->minimum(val); }
    int  minumum() const  { return((int)slider->minimum()); }
    void maximum(int val) { slider->maximum(val); }
    int  maximum() const  { return((int)slider->maximum()); }
    void bounds(int low, int high) { slider->bounds(low, high); }
};