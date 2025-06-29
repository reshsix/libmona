/*
 *  This file is part of libmona
 *
 *  Libmona is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  Libmona is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with libmona; if not, see <https://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#include <mona/core.h>

/* Auxiliary functions */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static double
degrees(double rad)
{
    return fmod(rad * (180.0 / M_PI) + 360.0, 360.0);
}

static double
radians(double deg)
{
    return (deg * M_PI) / 180.0;
}

static double
polar(double x0, double y0, double x1, double y1)
{
    return atan2(y1 - y0, x1 - x0);
}

static time_t
epoch(double j2k)
{
    double jd = j2k + 2451545.0 - 0.0008;
    return (time_t)((jd - 2440587.5) * 86400.0);
}

static double
j2000(time_t utc)
{
    double jd = ((double)utc / 86400.0) + 2440587.5;
    return jd - 2451545.0 + 0.0008;
}

static double
variable_T(double j2k)
{
    return j2k / 365250.0;
}

/*  Planetary position functions
    VSOP87C with A > 0.001 and ELP2000 "low" precision */

static void
ref_earth(double *x0, double *y0, double t)
{
    double X = 0.0, Y = 0.0;

    double X0 = 0.0;
    X0 += 0.99986069925 * cos(1.75347045757 +   6283.31966747490 * t);
    X0 += 0.02506324281 * cos(4.93819429098 +      0.24381748350 * t);
    X0 += 0.00835274807 * cos(1.71033525539 +  12566.39551746630 * t);
    X += X0;

    double X1 = 0.0;
    X1 += 0.00154550744 * cos(0.64605836878 +      0.24381748350 * t);
    X += X1 * t;

    double Y0 = 0.0;
    Y0 += 0.99986069925 * cos(0.18267413078 +   6283.31966747490 * t);
    Y0 += 0.02506324281 * cos(3.36739796418 +      0.24381748350 * t);
    Y0 += 0.00835274807 * cos(0.13953892859 +  12566.39551746630 * t);
    Y += Y0;

    double Y1 = 0.0;
    Y1 += 0.00154550744 * cos(5.35844734917 +      0.24381748350 * t);
    Y += Y1 * t;

    *x0 = X, *y0 = Y;
}

static double
pos_moon(double t)
{
    t *= 10.0;

    double nv0 = 0.0, nv1 = 0.0, nv2 = 0.0, nv3 = 0.0;

    double coeff0[29][6] = {
    { 6.28877383, 134.9634114,  477198.8676313,   89.970,  14.348,  -6.797},
    { 1.27401064, 100.7369970,  413335.3554020, -122.571, -10.684,   5.028},
    { 0.65830943, 235.7004084,  890534.2230335,  -32.601,   3.664,  -1.769},
    { 0.21361825, 269.9268228,  954397.7352627,  179.941,  28.695, -13.594},
    {-0.18511586, 357.5291092,   35999.0502909,   -1.536,   0.041,   0.000},
    {-0.11433213, 186.5441986,  966404.0350546,  -68.058,  -0.567,   0.232},
    { 0.05879321, 325.7735856,  -63863.5122292, -212.541, -25.031,  11.826},
    { 0.05706551, 103.2078878,  377336.3051112, -121.035, -10.724,   5.028},
    { 0.05332117,  10.6638198, 1367733.0906648,   57.370,  18.011,  -8.566},
    { 0.04575792, 238.1712992,  854535.1727426,  -31.065,   3.623,  -1.769},
    {-0.04092258, 222.5656978, -441199.8173404,  -91.506, -14.307,   6.797},
    {-0.03471892, 297.8502042,  445267.1115168,  -16.300,   1.832,  -0.884},
    {-0.03038341, 132.4925206,  513197.9179223,   88.434,  14.388,  -6.797},
    { 0.01532696,  49.1562098,  -75869.8120211,   35.458,   4.321,  -2.001},
    {-0.01252767, 321.5076100, 1443602.9026859,   21.912,  13.780,  -6.566},
    { 0.01098147, 308.4192127, -489205.1674233,  158.029,  14.915,  -7.029},
    { 0.01067495, 336.4374054, 1303869.5784357, -155.171,  -7.020,   3.259},
    { 0.01003439,  44.8902341, 1431596.6028940,  269.911,  43.043, -20.392},
    { 0.00854794, 201.4739940,  826670.7108043, -245.142, -21.367,  10.057},
    {-0.00788808,  98.2661062,  449334.4056931, -124.107, -10.643,   5.028},
    {-0.00676617, 233.2295176,  926533.2733244,  -34.136,   3.705,  -1.769},
    {-0.00516242, 162.8867928,  -31931.7561146, -106.271, -12.516,   5.913},
    { 0.00498735, 295.3793134,  481266.1618077,  -17.836,   1.873,  -0.884},
    { 0.00403619,  13.1347106, 1331734.0403739,   58.906,  17.971,  -8.566},
    { 0.00399436, 145.6272312, 1844931.9582962,  147.340,  32.359, -15.363},
    { 0.00386085, 111.4008168, 1781068.4460670,  -65.201,   7.328,  -3.538},
    { 0.00366502, 190.8101743, -541062.3798605, -302.511, -39.379,  18.623},
    {-0.00268863,  87.6022864, -918398.6849717, -181.476, -28.654,  13.594},
    {-0.00260163, 287.2811957, 1379739.3904568, -190.629, -11.251,   5.260}};

    for (int i = 0; i < 29; i++)
        nv0 += coeff0[i][0] * sin(radians(coeff0[i][1] + coeff0[i][2] * t +
                                          coeff0[i][3] * 10E-4 * t * t +
                                          coeff0[i][4] * 10E-6 * t * t * t +
                                          coeff0[i][5] * 10E-8 * t * t * t * t
                                         ));

    double coeff2[6][3] = {{ 0.46578, 357.529,   35999.050},
                           {-0.14345, 103.208,  377366.305},
                           {-0.11495, 238.171,  854535.173},
                           { 0.10310, 222.566, -441199.817},
                           { 0.07656, 132.493,  513197.918},
                           {-0.07062,  27.775,     131.849}};

    double coeff1[] = {3.95801, 119.7524, 131.8489};
    nv1 += coeff1[0] * sin(radians(coeff1[1] + coeff1[2] * t));

    for (int i = 0; i < 6; i++)
        nv2 += coeff2[i][0] * sin(radians(coeff2[i][1] + coeff2[i][2] * t));

    double coeff3[] = {13.53, 357.5, 35999.1};
    nv3 += coeff3[0] * sin(radians(coeff3[1] + coeff3[2] * t));

    double l = 218.31665 + 481267.88134 * t - 13.268E-4 * t * t +
               1.856E-6 * t * t * t - 1.534E-8 * t * t * t * t +
               nv0 + 10E-3 * (nv1 + t * nv2 + t * t * 10E-4 * nv3);
    l = fmod(l, 360.0);

    return l;
}

static double
pos_sun(double x0, double y0)
{
    return degrees(polar(0.0, 0.0, -x0, -y0));
}

static double
pos_mercury(double x0, double y0, double t)
{
    double X = 0.0, Y = 0.0;

    double X0 = 0.0;
    X0 += 0.37749277893 * cos(4.40259139579 +  26088.14695905770 * t);
    X0 += 0.11918926148 * cos(4.49027758439 +      0.24381748350 * t);
    X0 += 0.03840153904 * cos(1.17015646101 +  52176.05010063190 * t);
    X0 += 0.00585979278 * cos(4.22090402969 +  78263.95324220609 * t);
    X0 += 0.00305833424 * cos(2.10298673336 +  26087.65932409069 * t);
    X0 += 0.00105974941 * cos(0.98846517420 + 104351.85638378029 * t);
    X += X0;

    double X1 = 0.0;
    X1 += 0.00328639517 * cos(6.04028758995 +      0.24381748350 * t);
    X1 += 0.00106107047 * cos(5.91538469937 +  52176.05010063190 * t);
    X += X1 * t;

    double Y0 = 0.0;
    Y0 += 0.37749277893 * cos(2.83179506899 +  26088.14695905770 * t);
    Y0 += 0.11918926148 * cos(2.91948125760 +      0.24381748350 * t);
    Y0 += 0.03840153904 * cos(5.88254544140 +  52176.05010063190 * t);
    Y0 += 0.00585979278 * cos(2.65010770289 +  78263.95324220609 * t);
    Y0 += 0.00305833424 * cos(3.67378306016 +  26087.65932409069 * t);
    Y0 += 0.00105974941 * cos(5.70085415459 + 104351.85638378029 * t);
    Y += Y0;

    double Y1 = 0.0;
    Y1 += 0.00328639517 * cos(4.46949126315 +      0.24381748350 * t);
    Y1 += 0.00106107047 * cos(4.34458837257 +  52176.05010063190 * t);
    Y += Y1 * t;

    return degrees(polar(x0, y0, X, Y));
}

static double
pos_venus(double x0, double y0, double t)
{
    double X = 0.0, Y = 0.0;

    double X0 = 0.0;
    X0 += 0.72268045621 * cos(3.17614669179 +  10213.52936369450 * t);
    X0 += 0.00733886107 * cos(5.43699242686 +      0.24381748350 * t);
    X0 += 0.00244692613 * cos(4.05605630888 +  20426.81490990550 * t);
    X += X0;

    double Y0 = 0.0;
    Y0 += 0.72268045621 * cos(1.60535036499 +  10213.52936369450 * t);
    Y0 += 0.00733886107 * cos(3.86619610007 +      0.24381748350 * t);
    Y0 += 0.00244692613 * cos(2.48525998209 +  20426.81490990550 * t);
    Y += Y0;

    return degrees(polar(x0, y0, X, Y));
}

static double
pos_mars(double x0, double y0, double t)
{
    double X = 0.0, Y = 0.0;

    double X0 = 0.0;
    X0 += 1.51664432758 * cos(6.20347631684 +   3340.85624418330 * t);
    X0 += 0.21337343470 * cos(2.72390342700 +      0.24381748350 * t);
    X0 += 0.07067734657 * cos(0.25841679630 +   6681.46867088311 * t);
    X0 += 0.00494034875 * cos(0.59654023167 +  10022.08109758290 * t);
    X0 += 0.00135189385 * cos(0.59603904502 +   3340.36860921629 * t);
    X += X0;

    double X1 = 0.0;
    X1 += 0.01668487239 * cos(4.16976892466 +      0.24381748350 * t);
    X1 += 0.00551520815 * cos(5.09364818449 +   6681.46867088311 * t);
    X += X1 * t;

    double Y0 = 0.0;
    Y0 += 1.51664432758 * cos(4.63267999004 +   3340.85624418330 * t);
    Y0 += 0.21337343470 * cos(1.15310710021 +      0.24381748350 * t);
    Y0 += 0.07067734657 * cos(4.97080577669 +    6681.4686708831 * t);
    Y0 += 0.00494034875 * cos(5.30892921206 +  10022.08109758290 * t);
    Y0 += 0.00135189385 * cos(2.16683537182 +   3340.36860921629 * t);
    Y += Y0;

    double Y1 = 0.0;
    Y1 += 0.01668487239 * cos(2.59897259786 +      0.24381748350 * t);
    Y1 += 0.00551520815 * cos(3.52285185770 +   6681.46867088311 * t);
    Y += Y1 * t;

    return degrees(polar(x0, y0, X, Y));
}

static double
pos_jupiter(double x0, double y0, double t)
{
    double X = 0.0, Y = 0.0;

    double X0 = 0.0;
    X0 += 5.19591755961 * cos(0.59954672200 +    529.93478257810 * t);
    X0 += 0.37839498798 * cos(3.39164799011 +      0.24381748350 * t);
    X0 += 0.12593400247 * cos(0.94916456487 +   1059.62574767270 * t);
    X0 += 0.01504469362 * cos(0.72934997067 +    522.82123557730 * t);
    X0 += 0.01476016965 * cos(3.61748058581 +    537.04832957890 * t);
    X0 += 0.00457751771 * cos(1.29886948102 +   1589.31671276730 * t);
    X0 += 0.00301186623 * cos(5.17699571796 +      7.35736448430 * t);
    X0 += 0.00381996183 * cos(1.98605213956 +    102.84895673509 * t);
    X0 += 0.00193179179 * cos(5.02674864982 +    426.84200835950 * t);
    X0 += 0.00148291722 * cos(6.15618324076 +    110.45013870291 * t);
    X0 += 0.00142171400 * cos(5.56556633177 +    633.02755679670 * t);
    X0 += 0.00133443745 * cos(0.89287673636 +    213.54291292150 * t);
    X0 += 0.00102719450 * cos(6.17016418155 +   1052.51220067191 * t);
    X += X0;

    double X1 = 0.0;
    X1 += 0.01912556490 * cos(4.23275123829 +      0.24381748350 * t);
    X1 += 0.00634902259 * cos(0.10706507632 +   1059.62574767270 * t);
    X1 += 0.00600483021 * cos(2.42939944495 +    522.82123557730 * t);
    X1 += 0.00589005176 * cos(1.91564604125 +    537.04832957890 * t);
    X += X1 * t;

    double X2 = 0.0;
    X2 += 0.00202462055 * cos(2.16872090770 +    529.93478257810 * t);
    X2 += 0.00132313738 * cos(5.46375601476 +      0.24381748350 * t);
    X2 += 0.00123752958 * cos(4.12933545743 +    522.82123557730 * t);
    X2 += 0.00121682952 * cos(0.20529040863 +    537.04832957890 * t);
    X += X2 * t * t;

    double Y0 = 0.0;
    Y0 += 5.19591755961 * cos(5.31193570238 +    529.93478257810 * t);
    Y0 += 0.37839498798 * cos(1.82085166331 +      0.24381748350 * t);
    Y0 += 0.12593400247 * cos(5.66155354525 +   1059.62574767270 * t);
    Y0 += 0.01504469362 * cos(5.44173895105 +    522.82123557730 * t);
    Y0 += 0.01476016965 * cos(2.04668425902 +    537.04832957890 * t);
    Y0 += 0.00457751771 * cos(6.01125846140 +   1589.31671276730 * t);
    Y0 += 0.00301186623 * cos(3.60619939116 +      7.35736448430 * t);
    Y0 += 0.00381996183 * cos(3.55684846636 +    102.84895673509 * t);
    Y0 += 0.00193179179 * cos(3.45595232302 +    426.84200835950 * t);
    Y0 += 0.00148291722 * cos(4.58538691397 +    110.45013870291 * t);
    Y0 += 0.00142171400 * cos(3.99477000498 +    633.02755679670 * t);
    Y0 += 0.00133443745 * cos(5.60526571675 +    213.54291292150 * t);
    Y0 += 0.00102719450 * cos(4.59936785476 +   1052.51220067191 * t);
    Y += Y0;

    double Y1 = 0.0;
    Y1 += 0.01912556490 * cos(2.66195491149 +      0.24381748350 * t);
    Y1 += 0.00634902259 * cos(4.81945405671 +   1059.62574767270 * t);
    Y1 += 0.00600483021 * cos(0.85860311815 +    522.82123557730 * t);
    Y1 += 0.00589005176 * cos(0.34484971445 +    537.04832957890 * t);
    Y += Y1 * t;

    double Y2 = 0.0;
    Y2 += 0.00202462055 * cos(0.59792458091 +    529.93478257810 * t);
    Y2 += 0.00132313738 * cos(3.89295968796 +      0.24381748350 * t);
    Y2 += 0.00123752958 * cos(2.55853913064 +    522.82123557730 * t);
    Y2 += 0.00121682952 * cos(4.91767938901 +    537.04832957890 * t);
    Y += Y2 * t * t;

    return degrees(polar(x0, y0, X, Y));
}

static double
pos_saturn(double x0, double y0, double t)
{
    double X = 0.0, Y = 0.0;

    double X0 = 0.0;
    X0 += 9.52312533591 * cos(0.87401491487 +    213.54291292150 * t);
    X0 += 0.79501390398 * cos(4.76580713096 +      0.24381748350 * t);
    X0 += 0.26427074351 * cos(0.12339999915 +    426.84200835950 * t);
    X0 += 0.06836881382 * cos(4.14537914189 +    206.42936592071 * t);
    X0 += 0.06628914946 * cos(0.75057317755 +    220.65645992230 * t);
    X0 += 0.02340967916 * cos(2.01979283929 +      7.35736448430 * t);
    X0 += 0.01250581159 * cos(2.17392657526 +    110.45013870291 * t);
    X0 += 0.01141539711 * cos(3.03345312296 +    419.72846135871 * t);
    X0 += 0.01098217124 * cos(5.65720860592 +    640.14110379750 * t);
    X += X0;

    double X1 = 0.0;
    X1 += 0.09285877988 * cos(0.61678993503 +      0.24381748350 * t);
    X1 += 0.03086501680 * cos(4.27493632359 +    426.84200835950 * t);
    X1 += 0.02728479923 * cos(5.84476389020 +    206.42936592071 * t);
    X1 += 0.02644990371 * cos(5.33256382404 +    220.65645992230 * t);
    X1 += 0.00629201988 * cos(0.32457757988 +      7.35736448430 * t);
    X1 += 0.00256626023 * cos(3.52453971595 +    640.14110379750 * t);
    X1 += 0.00312353889 * cos(4.82956308970 +    419.72846135871 * t);
    X1 += 0.00189297041 * cos(4.48614588896 +    433.95555536030 * t);
    X1 += 0.00204465020 * cos(1.07744722386 +    213.54291292150 * t);
    X1 += 0.00118113185 * cos(1.16350724193 +    110.45013870291 * t);
    X += X1 * t;

    double X2 = 0.0;
    X2 += 0.00862721930 * cos(2.44701762869 +    213.54291292150 * t);
    X2 += 0.00611630287 * cos(2.28479450822 +      0.24381748350 * t);
    X2 += 0.00561413805 * cos(1.25489421778 +    206.42936592071 * t);
    X2 += 0.00547271370 * cos(3.61668298145 +    220.65645992230 * t);
    X2 += 0.00234984093 * cos(2.44660941367 +    426.84200835950 * t);
    X += X2 * t * t;

    double Y0 = 0.0;
    Y0 += 9.52312533591 * cos(5.58640389526 +    213.54291292150 * t);
    Y0 += 0.79501390398 * cos(3.19501080417 +      0.24381748350 * t);
    Y0 += 0.26427074351 * cos(4.83578897954 +    426.84200835950 * t);
    Y0 += 0.06836881382 * cos(2.57458281509 +    206.42936592071 * t);
    Y0 += 0.06628914946 * cos(5.46296215793 +    220.65645992230 * t);
    Y0 += 0.02340967916 * cos(0.44899651249 +      7.35736448430 * t);
    Y0 += 0.01250581159 * cos(0.60313024847 +    110.45013870291 * t);
    Y0 += 0.01141539711 * cos(1.46265679616 +    419.72846135871 * t);
    Y0 += 0.01098217124 * cos(4.08641227912 +    640.14110379750 * t);
    Y0 += 0.00773784455 * cos(5.07365052128 +    213.05527795450 * t);
    Y0 += 0.00708587042 * cos(1.13843336592 +    316.63568714010 * t);
    Y0 += 0.00434227030 * cos(5.42862996601 +    529.93478257810 * t);
    Y0 += 0.00373110880 * cos(4.71342390707 +    433.95555536030 * t);
    Y0 += 0.00413446294 * cos(0.41153807474 +    102.84895673509 * t);
    Y0 += 0.00169743473 * cos(1.48210526398 +    205.94173095370 * t);
    Y0 += 0.00122184772 * cos(2.79745491359 +    103.33659170210 * t);
    Y0 += 0.00141571884 * cos(0.13687533326 +    419.24082639170 * t);
    Y += Y0;

    double Y1 = 0.0;
    Y1 += 0.09285877988 * cos(5.32917891541 +      0.24381748350 * t);
    Y1 += 0.03086501680 * cos(2.70413999679 +    426.84200835950 * t);
    Y1 += 0.02728479923 * cos(4.27396756341 +    206.42936592071 * t);
    Y1 += 0.02644990371 * cos(3.76176749725 +    220.65645992230 * t);
    Y1 += 0.00629201988 * cos(5.03696656027 +      7.35736448430 * t);
    Y1 += 0.00256626023 * cos(1.95374338915 +    640.14110379750 * t);
    Y1 += 0.00312353889 * cos(3.25876676291 +    419.72846135871 * t);
    Y1 += 0.00189297041 * cos(2.91534956216 +    433.95555536030 * t);
    Y1 += 0.00204465020 * cos(5.78983620425 +    213.54291292150 * t);
    Y1 += 0.00118113185 * cos(5.87589622232 +    110.45013870291 * t);
    Y += Y1 * t;

    double Y2 = 0.0;
    Y2 += 0.00862721930 * cos(0.87622130189 +    213.54291292150 * t);
    Y2 += 0.00611630287 * cos(0.71399818143 +      0.24381748350 * t);
    Y2 += 0.00561413805 * cos(5.96728319816 +    206.42936592071 * t);
    Y2 += 0.00547271370 * cos(2.04588665465 +    220.65645992230 * t);
    Y2 += 0.00234984093 * cos(0.87581308687 +    426.84200835950 * t);
    Y += Y2 * t * t;

    return degrees(polar(x0, y0, X, Y));
}

/* Other objects functions */

static double
tilt_earth(double j2k)
{
    double t = j2k / 36500.0;
    double tilt = 84381.448 - 46.84024 * t - 0.00059 * t * t +
                  0.001813 * t * t * t;
    return 2.0 * M_PI * ((tilt / 3600.0) / 360.0);
}

static double
angle_earth(double j2k)
{
    return fmod(2.0 * M_PI * (0.7790572732640 + 1.00273781191135448 * j2k),
                2.0 * M_PI);
}

static double
local_sidereal(double j2k, double longitude)
{
    return fmod(angle_earth(j2k) + radians(longitude), 2.0 * M_PI);
}

static double
pos_descendant(double j2k, double latitude, double longitude)
{
    double tilt = tilt_earth(j2k);
    double lst = local_sidereal(j2k, longitude);
    return degrees(atan2(-cos(lst), tan(radians(latitude)) * sin(tilt) +
                                    sin(lst)               * cos(tilt)));
}
static double
pos_ascendant(double j2k, double latitude, double longitude)
{
    return fmod(pos_descendant(j2k, latitude, longitude) + 180.0, 360.0);
}

static double
pos_imum_coeli(double j2k, double longitude)
{
    return degrees(atan2(tan(local_sidereal(j2k, longitude)),
                   cos(tilt_earth(j2k))));
}

static double
pos_medium_coeli(double j2k, double longitude)
{
    return fmod(pos_imum_coeli(j2k, longitude) + 180.0, 360.0);
}

/* Astrologic conversions */

extern struct mona_waypoint
mona_waypoint(time_t utc, double latitude, double longitude)
{
    struct mona_waypoint ret = {0};

    ret.latitude = latitude;
    ret.longitude = longitude;
    ret.j2k = j2000(utc);

    ret.T = variable_T(ret.j2k);
    ref_earth(&(ret.X), &(ret.Y), ret.T);

    return ret;
}

extern struct mona_zodiac
mona_zodiac(struct mona_waypoint p, enum mona_object o)
{
    /* Zodiac object position */
    struct mona_zodiac ret = {0};

    double deg = 0;
    switch (o)
    {
        case MONA_OBJECT_MOON:
            deg = pos_moon(p.T);
            break;
        case MONA_OBJECT_MERCURY:
            deg = pos_mercury(p.X, p.Y, p.T);
            break;
        case MONA_OBJECT_VENUS:
            deg = pos_venus(p.X, p.Y, p.T);
            break;
        case MONA_OBJECT_SUN:
            deg = pos_sun(p.X, p.Y);
            break;
        case MONA_OBJECT_MARS:
            deg = pos_mars(p.X, p.Y, p.T);
            break;
        case MONA_OBJECT_JUPITER:
            deg = pos_jupiter(p.X, p.Y, p.T);
            break;
        case MONA_OBJECT_SATURN:
            deg = pos_saturn(p.X, p.Y, p.T);
            break;

        case MONA_OBJECT_ASCENDANT:
            deg = pos_ascendant(p.j2k, p.latitude, p.longitude);
            break;
        case MONA_OBJECT_DESCENDANT:
            deg = pos_descendant(p.j2k, p.latitude, p.longitude);
            break;
        case MONA_OBJECT_MEDIUM_COELI:
            deg = pos_medium_coeli(p.j2k, p.longitude);
            break;
        case MONA_OBJECT_IMUM_COELI:
            deg = pos_imum_coeli(p.j2k, p.longitude);
            break;
        default:
            break;
    }
    ret.position = deg;

    ret.sign = floor(deg / 30.0);
    deg = fmod(deg, 30.0);

    ret.decan = floor(deg / 10.0);
    ret.degrees = floor(deg);
    deg = fmod(deg, 1.0);

    ret.seconds = round(deg * 60.0);

    return ret;
}

extern struct mona_lunar
mona_lunar(struct mona_waypoint p)
{
    /* Moon phase calculations */
    struct mona_lunar ret = {0};

    double length = 29.53059;

    double age = fmod(p.j2k + 142.25, length);
    age = (age < 0.0) ? age + length : age;
    ret.age = age;

    double deg = (age / length) * 360.0;
    ret.phase = floor(deg / 45.0);
    ret.degree = deg;

    ret.beginning = epoch(p.j2k - age);
    ret.ending = epoch(p.j2k + (length - age));

    return ret;
}

extern struct mona_solar
mona_solar(struct mona_waypoint p)
{
    /* Solar calculations */
    struct mona_solar ret = {0};

    double mst = floor(p.j2k) - (p.longitude / 360.0);
    double M = fmod(357.5291 + 0.98560028 * mst, 360.0);
    double C = 1.9148 * sin(radians(M)) +
                 0.02 * sin(radians(2.0 * M)) +
               0.0003 * sin(radians(3.0 * M));
    double el = fmod((M + C + 180.0 + 102.9372), 360.0);

    ret.degree = el;

    uint8_t phase = el / 90.0;
    double age = (el / 360.0) * 365.25;
    ret.age = age;

    double coeffs[4][5] = {
        {2451623.80984, 365242.37404,  0.05169, -0.00411, -0.00057},
        {2451716.56767, 365241.62603,  0.00325,  0.00888, -0.00030},
        {2451810.21715, 365242.01767, -0.11575,  0.00337,  0.00078},
        {2451900.05952, 365242.74049, -0.06223, -0.00823,  0.00032}
    };

    time_t beg = 0.0;
    time_t end = 0.0;

    uint8_t sources[] = {phase, (phase + 1) % 4};
    time_t *targets[] = {&(beg), &(end)};
    for (int i = 0; i < 2; i++)
    {
        double delta = ((i == 0) ? 90.0 : 90.0) - fmod(age, 90.0);

        uint8_t x = sources[i];
        double T = floor((p.j2k + delta) / 365.25) / 1000.0;

        double date = coeffs[x][0] + coeffs[x][1] * T +
                                     coeffs[x][2] * T * T +
                                     coeffs[x][3] * T * T * T +
                                     coeffs[x][4] * T * T * T * T;
        *(targets[i]) = epoch(date - 2451545.0);
    }

    ret.season = fmod(phase + 2.0 * (p.latitude < 0.0), 4.0);
    ret.beginning = beg;
    ret.ending = end;

    /* Sunrise / Sunset */
    double jt = mst + 0.0053 * sin(radians(M)) -
                      0.0069 * sin(radians(2.0 * el));

    double dec = asin(sin(radians(el)) * 0.39778370349);
    double hr = acos((-0.0145380805 - sin(radians(p.latitude)) * sin(dec)) /
                                     (cos(radians(p.latitude)) * cos(dec)));
    hr /= (2.0 * M_PI);

    ret.sunrise = epoch(jt - hr);
    ret.sunset  = epoch(jt + hr);

    /* Planetary day and hour */
    enum mona_day d = fmod(fmod(floor(p.j2k - 1.0), 7.0) + 7.0, 7.0);
    ret.day = d;

    switch (d)
    {
        case MONA_DAY_SUN:     ret.hour = MONA_HOUR_SUN;     break;
        case MONA_DAY_MOON:    ret.hour = MONA_HOUR_MOON;    break;
        case MONA_DAY_MARS:    ret.hour = MONA_HOUR_MARS;    break;
        case MONA_DAY_MERCURY: ret.hour = MONA_HOUR_MERCURY; break;
        case MONA_DAY_JUPITER: ret.hour = MONA_HOUR_JUPITER; break;
        case MONA_DAY_VENUS:   ret.hour = MONA_HOUR_VENUS;   break;
        case MONA_DAY_SATURN:  ret.hour = MONA_HOUR_SATURN;  break;
    }
    ret.hour = fmod((p.j2k - (jt - hr)) / (hr / 6.0) + ret.hour, 7.0);

    return ret;
}
