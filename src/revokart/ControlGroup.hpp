#pragma once

#include "rk_types.hpp"

class UIControl;
class Page;

class ControlGroup
{
public:
    // After construction, create() must be called once to put the group
    // in a valid state. Calling create() twice is a memory leak.

    inline UIControl *operator[](int index) { return at(index); }

    void create(UIControl *parent, int size);
    void create(Page *page, int size);

    void insert(int index, UIControl *control, int pass);
    UIControl *at(int index);
    struct Functor
    {
        virtual void operator()(UIControl *control) = 0;
    };

    // Call a function on every element
    void debug(); // effective no-op
    void init();  // initialize every control

    void calc();              // call each frame
    void draw(int draw_pass); // call each frame

    void solve();                     // solve positioning
    void transform(Functor &functor); // a la std::transform

    UIControl *getParent() { return mParent; }
    Page *getPage() { return mPage; }

    UIControl **mData;
    UIControl **mDataSorted; // By z_index
    UIControl *mParent;
    Page *mPage;
    s32 mSize;
};
