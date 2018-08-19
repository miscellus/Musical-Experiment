#ifndef GLOBAL_CONSTANTS_H
#define GLOBAL_CONSTANTS_H

// #ifndef SAMPLES_PER_SECOND
// #define SAMPLES_PER_SECOND 8000
// #endif

// #ifndef AMPLITUDE
// #define AMPLITUDE 0.3
// #endif

// #ifndef BEATS_PER_MINUTE
// #define BEATS_PER_MINUTE 100
// #endif

// #define BEATS_PER_SECOND (BEATS_PER_MINUTE/60.0)

// #define SECONDS_PER_BEAT (60.0/BEATS_PER_MINUTE)

#define PI 3.141592653589793
#define TAU 6.283185307179586

/* Ste\ sizes */
#define s57 0.03716272234383503  //2**(-57/12)
#define s56 0.03937253280921478  //2**(-56/12)
#define s55 0.04171374544281358  //2**(-55/12)
#define s54 0.04419417382415922  //2**(-54/12)
#define s53 0.04682209615239629  //2**(-53/12)
#define s52 0.049606282874006244 //2**(-52/12)
#define s51 0.052556025953357156 //2**(-51/12)
#define s50 0.0556811698837712   //2**(-50/12)
#define s49 0.05899214454260585  //2**(-49/12)
#define s48 0.0625               //2**(-48/12)
#define s47 0.06621644339745596  //2**(-47/12)
#define s46 0.0701538780193358   //2**(-46/12)
#define s45 0.07432544468767006  //2**(-45/12)
#define s44 0.07874506561842957  //2**(-44/12)
#define s43 0.08342749088562713  //2**(-43/12)
#define s42 0.08838834764831845  //2**(-42/12)
#define s41 0.09364419230479261  //2**(-41/12)
#define s40 0.09921256574801246  //2**(-40/12)
#define s39 0.10511205190671431  //2**(-39/12)
#define s38 0.11136233976754242  //2**(-38/12)
#define s37 0.11798428908521168  //2**(-37/12)
#define s36 0.125                //2**(-36/12)
#define s35 0.1324328867949119   //2**(-35/12)
#define s34 0.1403077560386716   //2**(-34/12)
#define s33 0.14865088937534013  //2**(-33/12)
#define s32 0.15749013123685915  //2**(-32/12)
#define s31 0.16685498177125427  //2**(-31/12)
#define s30 0.1767766952966369   //2**(-30/12)
#define s29 0.18728838460958522  //2**(-29/12)
#define s28 0.19842513149602492  //2**(-28/12)
#define s27 0.21022410381342863  //2**(-27/12)
#define s26 0.22272467953508485  //2**(-26/12)
#define s25 0.23596857817042335  //2**(-25/12)
#define s24 0.25                 //2**(-24/12)
#define s23 0.2648657735898238   //2**(-23/12)
#define s22 0.28061551207734325  //2**(-22/12)
#define s21 0.29730177875068026  //2**(-21/12)
#define s20 0.3149802624737183   //2**(-20/12)
#define s19 0.3337099635425086   //2**(-19/12)
#define s18 0.3535533905932738   //2**(-18/12)
#define s17 0.3745767692191704   //2**(-17/12)
#define s16 0.3968502629920499   //2**(-16/12)
#define s15 0.42044820762685725  //2**(-15/12)
#define s14 0.44544935907016964  //2**(-14/12)
#define s13 0.47193715634084676  //2**(-13/12)
#define s12 0.5                  //2**(-12/12)
#define s11 0.5297315471796477   //2**(-11/12)
#define s10 0.5612310241546865   //2**(-10/12)
#define s9  0.5946035575013605   //2**(-9/12)
#define s8  0.6299605249474366   //2**(-8/12)
#define s7  0.6674199270850172   //2**(-7/12)
#define s6  0.7071067811865476   //2**(-6/12)
#define s5  0.7491535384383408   //2**(-5/12)
#define s4  0.7937005259840998   //2**(-4/12)
#define s3  0.8408964152537145   //2**(-3/12)
#define s2  0.8908987181403393   //2**(-2/12)
#define s1  0.9438743126816935   //2**(-1/12)
#define S1  1.0594630943592953   //2**(1/12)
#define S2  1.122462048309373    //2**(2/12)
#define S3  1.189207115002721    //2**(3/12)
#define S4  1.2599210498948732   //2**(4/12)
#define S5  1.3348398541700344   //2**(5/12)
#define S6  1.4142135623730951   //2**(6/12)
#define S7  1.4983070768766815   //2**(7/12)
#define S8  1.5874010519681994   //2**(8/12)
#define S9  1.681792830507429    //2**(9/12)
#define S10 1.7817974362806785   //2**(10/12)
#define S11 1.8877486253633868   //2**(11/12)
#define S12 2.0                  //2**(12/12)
#define S13 2.1189261887185906   //2**(13/12)
#define S14 2.244924096618746    //2**(14/12)
#define S15 2.378414230005442    //2**(15/12)
#define S16 2.5198420997897464   //2**(16/12)
#define S17 2.6696797083400687   //2**(17/12)
#define S18 2.8284271247461903   //2**(18/12)
#define S19 2.996614153753363    //2**(19/12)
#define S20 3.174802103936399    //2**(20/12)
#define S21 3.363585661014858    //2**(21/12)
#define S22 3.563594872561357    //2**(22/12)
#define S23 3.775497250726774    //2**(23/12)
#define S24 4.0                  //2**(24/12)
#define S25 4.237852377437181    //2**(25/12)
#define S26 4.489848193237491    //2**(26/12)
#define S27 4.756828460010884    //2**(27/12)
#define S28 5.039684199579493    //2**(28/12)
#define S29 5.339359416680137    //2**(29/12)
#define S30 5.656854249492381    //2**(30/12)
#define S31 5.993228307506727    //2**(31/12)
#define S32 6.3496042078727974   //2**(32/12)
#define S33 6.727171322029716    //2**(33/12)
#define S34 7.127189745122715    //2**(34/12)
#define S35 7.550994501453547    //2**(35/12)
#define S36 8.0                  //2**(36/12)
#define S37 8.475704754874362    //2**(37/12)
#define S38 8.979696386474982    //2**(38/12)
#define S39 9.513656920021768    //2**(39/12)
#define S40 10.079368399158986   //2**(40/12)
#define S41 10.678718833360273   //2**(41/12)
#define S42 11.313708498984761   //2**(42/12)
#define S43 11.986456615013454   //2**(43/12)
#define S44 12.699208415745595   //2**(44/12)
#define S45 13.454342644059432   //2**(45/12)
#define S46 14.25437949024543    //2**(46/12)
#define S47 15.101989002907095   //2**(47/12)
#define S48 16.0                 //2**(48/12)
#define S49 16.95140950974872    //2**(49/12)
#define S50 17.959392772949972   //2**(50/12)

/*
Note Frequency in hertz (Wavelength in centimeters)
*/

#define CONCERT_PITCH 440

#define c0 (s57 * CONCERT_PITCH)
#define C0 (s56 * CONCERT_PITCH)
#define d0 (s55 * CONCERT_PITCH)
#define D0 (s54 * CONCERT_PITCH)
#define e0 (s53 * CONCERT_PITCH)
#define f0 (s52 * CONCERT_PITCH)
#define F0 (s51 * CONCERT_PITCH)
#define g0 (s50 * CONCERT_PITCH)
#define G0 (s49 * CONCERT_PITCH)
#define a0 (s48 * CONCERT_PITCH)
#define A0 (s47 * CONCERT_PITCH)
#define b0 (s46 * CONCERT_PITCH)
#define c1 (s45 * CONCERT_PITCH)
#define C1 (s44 * CONCERT_PITCH)
#define d1 (s43 * CONCERT_PITCH)
#define D1 (s42 * CONCERT_PITCH)
#define e1 (s41 * CONCERT_PITCH)
#define f1 (s40 * CONCERT_PITCH)
#define F1 (s39 * CONCERT_PITCH)
#define g1 (s38 * CONCERT_PITCH)
#define G1 (s37 * CONCERT_PITCH)
#define a1 (s36 * CONCERT_PITCH)
#define A1 (s35 * CONCERT_PITCH)
#define b1 (s34 * CONCERT_PITCH)
#define c2 (s33 * CONCERT_PITCH)
#define C2 (s32 * CONCERT_PITCH)
#define d2 (s31 * CONCERT_PITCH)
#define D2 (s30 * CONCERT_PITCH)
#define e2 (s29 * CONCERT_PITCH)
#define f2 (s28 * CONCERT_PITCH)
#define F2 (s27 * CONCERT_PITCH)
#define g2 (s26 * CONCERT_PITCH)
#define G2 (s25 * CONCERT_PITCH)
#define a2 (s24 * CONCERT_PITCH)
#define A2 (s23 * CONCERT_PITCH)
#define b2 (s22 * CONCERT_PITCH)
#define c3 (s21 * CONCERT_PITCH)
#define C3 (s20 * CONCERT_PITCH)
#define d3 (s19 * CONCERT_PITCH)
#define D3 (s18 * CONCERT_PITCH)
#define e3 (s17 * CONCERT_PITCH)
#define f3 (s16 * CONCERT_PITCH)
#define F3 (s15 * CONCERT_PITCH)
#define g3 (s14 * CONCERT_PITCH)
#define G3 (s13 * CONCERT_PITCH)
#define a3 (s12 * CONCERT_PITCH)
#define A3 (s11 * CONCERT_PITCH)
#define b3 (s10 * CONCERT_PITCH)
#define c4 (s9 * CONCERT_PITCH)
#define C4 (s8 * CONCERT_PITCH)
#define d4 (s7 * CONCERT_PITCH)
#define D4 (s6 * CONCERT_PITCH)
#define e4 (s5 * CONCERT_PITCH)
#define f4 (s4 * CONCERT_PITCH)
#define F4 (s3 * CONCERT_PITCH)
#define g4 (s2 * CONCERT_PITCH)
#define G4 (s1 * CONCERT_PITCH)
#define a4 CONCERT_PITCH
#define A4 (S1 * CONCERT_PITCH)
#define b4 (S2 * CONCERT_PITCH)
#define c5 (S3 * CONCERT_PITCH)
#define C5 (S4 * CONCERT_PITCH)
#define d5 (S5 * CONCERT_PITCH)
#define D5 (S6 * CONCERT_PITCH)
#define e5 (S7 * CONCERT_PITCH)
#define f5 (S8 * CONCERT_PITCH)
#define F5 (S9 * CONCERT_PITCH)
#define g5 (S10 * CONCERT_PITCH)
#define G5 (S11 * CONCERT_PITCH)
#define a5 (S12 * CONCERT_PITCH)
#define A5 (S13 * CONCERT_PITCH)
#define b5 (S14 * CONCERT_PITCH)
#define c6 (S15 * CONCERT_PITCH)
#define C6 (S16 * CONCERT_PITCH)
#define d6 (S17 * CONCERT_PITCH)
#define D6 (S18 * CONCERT_PITCH)
#define e6 (S19 * CONCERT_PITCH)
#define f6 (S20 * CONCERT_PITCH)
#define F6 (S21 * CONCERT_PITCH)
#define g6 (S22 * CONCERT_PITCH)
#define G6 (S23 * CONCERT_PITCH)
#define a6 (S24 * CONCERT_PITCH)
#define A6 (S25 * CONCERT_PITCH)
#define b6 (S26 * CONCERT_PITCH)
#define c7 (S27 * CONCERT_PITCH)
#define C7 (S28 * CONCERT_PITCH)
#define d7 (S29 * CONCERT_PITCH)
#define D7 (S30 * CONCERT_PITCH)
#define e7 (S31 * CONCERT_PITCH)
#define f7 (S32 * CONCERT_PITCH)
#define F7 (S33 * CONCERT_PITCH)
#define g7 (S34 * CONCERT_PITCH)
#define G7 (S35 * CONCERT_PITCH)
#define a7 (S36 * CONCERT_PITCH)
#define A7 (S37 * CONCERT_PITCH)
#define b7 (S38 * CONCERT_PITCH)
#define c8 (S39 * CONCERT_PITCH)
#define C8 (S40 * CONCERT_PITCH)
#define d8 (S41 * CONCERT_PITCH)
#define D8 (S42 * CONCERT_PITCH)
#define e8 (S43 * CONCERT_PITCH)
#define f8 (S44 * CONCERT_PITCH)
#define F8 (S45 * CONCERT_PITCH)
#define g8 (S46 * CONCERT_PITCH)
#define G8 (S47 * CONCERT_PITCH)
#define a8 (S48 * CONCERT_PITCH)
#define A8 (S49 * CONCERT_PITCH)
#define b8 (S50 * CONCERT_PITCH)

#include <assert.h>
double NoteStringToFrequency(char *NoteString) {
    char Key = NoteString[0];
    char Octave = NoteString[1];

    if (Octave == '0') {
        if (Key == 'c') return c0;
        if (Key == 'C') return C0;
        if (Key == 'd') return d0;
        if (Key == 'D') return D0;
        if (Key == 'e') return e0;
        if (Key == 'f') return f0;
        if (Key == 'F') return F0;
        if (Key == 'g') return g0;
        if (Key == 'G') return G0;
        if (Key == 'a') return a0;
        if (Key == 'A') return A0;
        if (Key == 'b') return b0;
    }
    if (Octave == '1') {        
        if (Key == 'c') return c1;
        if (Key == 'C') return C1;
        if (Key == 'd') return d1;
        if (Key == 'D') return D1;
        if (Key == 'e') return e1;
        if (Key == 'f') return f1;
        if (Key == 'F') return F1;
        if (Key == 'g') return g1;
        if (Key == 'G') return G1;
        if (Key == 'a') return a1;
        if (Key == 'A') return A1;
        if (Key == 'b') return b1;
    }
    if (Octave == '2') {
        if (Key == 'c') return c2;
        if (Key == 'C') return C2;
        if (Key == 'd') return d2;
        if (Key == 'D') return D2;
        if (Key == 'e') return e2;
        if (Key == 'f') return f2;
        if (Key == 'F') return F2;
        if (Key == 'g') return g2;
        if (Key == 'G') return G2;
        if (Key == 'a') return a2;
        if (Key == 'A') return A2;
        if (Key == 'b') return b2;
    }
    if (Octave == '3') {
        if (Key == 'c') return c3;
        if (Key == 'C') return C3;
        if (Key == 'd') return d3;
        if (Key == 'D') return D3;
        if (Key == 'e') return e3;
        if (Key == 'f') return f3;
        if (Key == 'F') return F3;
        if (Key == 'g') return g3;
        if (Key == 'G') return G3;
        if (Key == 'a') return a3;
        if (Key == 'A') return A3;
        if (Key == 'b') return b3;
    }
    if (Octave == '4') {
        if (Key == 'c') return c4;
        if (Key == 'C') return C4;
        if (Key == 'd') return d4;
        if (Key == 'D') return D4;
        if (Key == 'e') return e4;
        if (Key == 'f') return f4;
        if (Key == 'F') return F4;
        if (Key == 'g') return g4;
        if (Key == 'G') return G4;
        if (Key == 'a') return a4;
        if (Key == 'A') return A4;
        if (Key == 'b') return b4;
    }
    if (Octave == '5') {
        if (Key == 'c') return c5;
        if (Key == 'C') return C5;
        if (Key == 'd') return d5;
        if (Key == 'D') return D5;
        if (Key == 'e') return e5;
        if (Key == 'f') return f5;
        if (Key == 'F') return F5;
        if (Key == 'g') return g5;
        if (Key == 'G') return G5;
        if (Key == 'a') return a5;
        if (Key == 'A') return A5;
        if (Key == 'b') return b5;
    }
    if (Octave == '6') {
        if (Key == 'c') return c6;
        if (Key == 'C') return C6;
        if (Key == 'd') return d6;
        if (Key == 'D') return D6;
        if (Key == 'e') return e6;
        if (Key == 'f') return f6;
        if (Key == 'F') return F6;
        if (Key == 'g') return g6;
        if (Key == 'G') return G6;
        if (Key == 'a') return a6;
        if (Key == 'A') return A6;
        if (Key == 'b') return b6;
    }
    if (Octave == '7') {
        if (Key == 'c') return c7;
        if (Key == 'C') return C7;
        if (Key == 'd') return d7;
        if (Key == 'D') return D7;
        if (Key == 'e') return e7;
        if (Key == 'f') return f7;
        if (Key == 'F') return F7;
        if (Key == 'g') return g7;
        if (Key == 'G') return G7;
        if (Key == 'a') return a7;
        if (Key == 'A') return A7;
        if (Key == 'b') return b7;
    }
    if (Octave == '8') {
        if (Key == 'c') return c8;
        if (Key == 'C') return C8;
        if (Key == 'd') return d8;
        if (Key == 'D') return D8;
        if (Key == 'e') return e8;
        if (Key == 'f') return f8;
        if (Key == 'F') return F8;
        if (Key == 'g') return g8;
        if (Key == 'G') return G8;
        if (Key == 'a') return a8;
        if (Key == 'A') return A8;
        if (Key == 'b') return b8;
    }

    return 0;
}

#endif