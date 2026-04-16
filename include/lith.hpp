


#include <cstdint>
enum class LithTokens: uint8_t{
        //types
        bit,            
        bit8,           
        bit16,
        bit32,
        bit64,
        bit128,
        bit256,
        nothing,

        //modules
        bring,
        send,

        //operators
        plus, //+
        minus, //-
        star, //*
        slash, //    /
        non,   // !
        assign, //=
        dot,    //cast     num.char
        arrow,  // -> / pipe operator
        unArrow, // <-    not though yet, TODO: give a reason to unArrow

        //comparator operators
        equal, //==
        nonEqual, //!=
        lessyr, // <=
        lesser, //<,
        greaty, //>=
        greater, //>

        //bitwise operators
        both, // and
        atLeast, // or
        xOr,
        nOr,
        nAnd,
        xNor,

        //composed operators
        addy, //+=
        lessy, //-=
        stary, // *=
        slashy, //   /=
        andy, // &=
        ory, //|=
        xory, //^=
        nory, //!|=
        nandy, //!&=
        xnory, //!^=

        // incrementor / decrementor
        incrementor,
        decrementor




};


